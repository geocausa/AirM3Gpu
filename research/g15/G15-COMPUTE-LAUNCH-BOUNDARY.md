# G15 Compute launch boundary

Research state: 2026-08-27

Target: J615 / T8122 G15G C0, RTKit-2419.140.12.release.

E060 closes the scheduler-registration lifecycle without executing GPU work. E061 asks the next safety question: can a G15 RunCompute/type-3 command be used only to exercise parser/scheduler bookkeeping, or does Apple's normal type-3 contract become hardware-facing immediately?

## RTKit parser and DAG contract

RTKit parses RunCompute as an exact 0x880-byte object. The type-3 parser feeds the command's stamp slot/value into the common per-stamp DAG binder before engine dispatch. The recovered Compute launch hook is the previously unlabelled function at `0xfffffc00000251e8..0x256db`.

For type 3 it:

1. performs the normal engine-2 cache/notifier preparation;
2. registers the command's UMA Page Pool State from `+0x83e`, using the prepared byte at `+0x846`;
3. records context-generation byte `+0x85f`;
4. propagates the command's `+0x7e7` selector into scheduler state;
5. selects `RunCompute +0x760` as the engine-2 execution stream;
6. stores that stream address in the per-stamp execution record at `+0x28/+0x30`.

The common scheduler only marks the DAG entry dispatched after this engine hook succeeds. Thus `+0x760` is an execution pointer, not optional trace metadata.

## Apple host contract

Apple's normal G15 Compute producer also does not expose a metadata-only type-3 shape.

`AGXCLChannelG15::generateRegisterList()` unconditionally builds the generation-specific CDM register array. It includes the CDM control-stream register `0x1a420` and the G15 dynamic register set; it does not reuse the old G14X register list.

`AGXCLChannelG15::encodeCLCommandSKUStream()` then builds the SKU execution stream. Its leading packet has low-six-bit type `0xb` and a 0x1b8-byte payload containing, among other state, the firmware addresses of:

- the command RegisterArray at `RunCompute +0x20`;
- `JobParameters2` at `RunCompute +0x76c`.

The encoder surrounds the G15 Compute WFI operation with timestamp records, finalizes the stream, and writes its FWVA/size to `RunCompute +0x760/+0x768`.

Consequently a normal Apple-style RunCompute is already an engine-execution object before RTKit marks it dispatched. There is no mechanically supported basis for a "parser-only RunCompute" experiment.

## Minimal UMA prerequisite

The UMA registration helper accepts a structurally zero-page descriptor: it does not require nonzero page count or list capacity. However the Page Pool State itself cannot be null. Later refresh code dereferences the state object's FW-uncached-state pointer at `+0x48` and compares its first qword with the cached mirror at `+0x50`.

A future minimum lab pool therefore still needs a real 0x70-byte Page Pool State, a unique 0..255 hardware-buffer/descriptor ID, a valid FW-visible uncached qword, and the already-published 256-entry descriptor table. Zero page count may be usable, but that alone does not make the Compute command inert.

## Safety boundary

No live RunCompute was attempted in E061. Before the first hardware-facing type-3 experiment, the following must be closed together:

- exact harmless G15 RegisterArray values;
- the SKU execution/retirement sequence;
- minimal UMA state and ID ownership;
- notifier + VM/context generation;
- sequential JobMeta stamp semantics and completion retirement;
- fail-closed first-command recovery.

The Barrier-only E060 path remains the latest live-safe registration checkpoint.
