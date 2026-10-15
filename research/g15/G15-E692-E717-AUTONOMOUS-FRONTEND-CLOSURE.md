# G15 E692–E717 autonomous frontend closure

Public research checkpoint: **2026-10-15**

Target: MacBook Air M3 / J615 / T8122 / G15G C0.

This note records only derived, reproducible conclusions. Proprietary Apple binaries, firmware, disassembly databases and raw captures remain outside this repository.

## Executive result

The selected direct-Compute failure is now below every known software-authored command, shared-memory, startup/reset and normal engine-activation state that has been mechanically tied to the path.

The current causal cut is:

`exact RunCompute/SKU/RegisterArray + exact selected ChannelState/DAG + exact UAT/PagePool/UMA -> coherent c040/c048/c050/c058 publication -> common Compute activation -> entry VA translation/fetch-visible -> [autonomous G15 entry-interpreter admission/execute transition fails] -> instruction 0 not interpreted -> context-store/quiesce -> replay-1`

The strongest discriminator remains the first-opcode oracle established before this checkpoint: a structurally validated Apple-proven entry sequence reaches its first semantic instruction on working M3 hardware, while Linux reaches the entry-address/visibility boundary but never produces that instruction's effect.

E692–E717 therefore do not add another speculative workaround. They remove the remaining plausible software/control planes around that boundary and make the surviving problem substantially more specific.

## E692–E701 — broad startup/platform closure

A static sweep revisited the remaining retained-firmware startup branches, G15 Fender/power controller state, SecureMonitor/GPTBAT setup, firmware mapping lifecycle, small G15 boot overrides, public scheduler-state interpretations, context-save-as-observer ideas, and possible Linux pre-GPU platform contamination.

None supplied a working-M3-only state producer between active-slot publication and first entry-instruction interpretation.

This phase is important because it prevents a common failure mode in bring-up work: repeatedly converting every newly named register or boot helper into a live mutation. The surviving search space must now have a demonstrated consumer edge at the actual failure boundary.

## E702/E703 — real upstream-quality bug, but not the Compute root cause

E702 found a genuine G15 startup parity defect unrelated to the first-opcode stall: two startup YUV-matrix records were left zero in the Linux research lineage while exact-target and independently working-M3 evidence agree on nonzero initialization.

E703 restores those matrices on an isolated kernel branch and passes compile/checkpatch validation. The fix is worth retaining for driver correctness, but no causal chain connects YUV conversion state to direct Compute entry-interpreter admission, so it was deliberately not used as a speculative live diagnostic.

Kernel-side research commit: `347ea0444159` (`drm: asahi: restore G15 startup YUV matrices`).

## E704–E710 — software publication plane exhausted

The retained firmware path after a selected Compute command was reconstructed again at the narrow failure boundary.

The final hardware publisher writes the already-known slot state (`c048`, optional dependency state, `c040`, `c050`, then `c058`) and returns. There is no hidden second firmware call, host callback, geometry transaction, cache command, or state-loader-enable packet after `c058`.

The remaining G15 command-local and policy fields were also re-audited. Queue priority, execute-permission hypotheses, soft-fault policy, q4 runtime policy and remaining opaque/default RunCompute fields do not produce a missing one-variable activation prerequisite for the selected command.

The consequence is strong: once `c058` publishes the slot, the next relevant transition is autonomous hardware behavior rather than another software packet that Linux forgot to send.

## E711 — exact engine lifecycle below c058

Retained firmware shows four important lifecycle registers:

- `c120`: hardware engine active/busy bitmap;
- `c140`: firmware engine mode/request control;
- `c148`: hardware acknowledgement of that request;
- `c128`: hardware per-slot completion/release bitmap.

For Compute's mode-2 path, firmware requests the corresponding mode through the `c140` field and waits for the matching `c148` acknowledgement before the later completion path consumes `c128`.

This is not the missing real-Launch-only transition. Earlier controls prove the same Compute-class activation envelope is traversed by a terminate-only command that completes normally. The failing command therefore diverges later, when its actual entry program should begin being interpreted.

Directly poking or polling these adjacent MMIO registers would add risk without a target-backed discriminator and is not justified by this result.

## E712/E714/E715 — Apple/firmware diagnostic routes exhausted

Several promising-sounding diagnostic interfaces turned out not to expose the missing stage.

The exact `dumpFirmwareRegisters` route resolves to an already-known shared-memory diagnostic request on one branch and a power/Fender-controller diagnostic on the other. The useful USC/CDM debug-status readers are behind a housekeeping/timer path that is not naturally armed for the failing launch; forcing that path would change the state machine rather than passively observe it.

The exact host's named scheduler-status decoder is also a stub in the target build. Firmware ring/memory dumps describe shared firmware state rather than an independent state-loader hardware snapshot. The J615 register-override list is empty in normal bootstrap, so there is no hidden Apple patch list to reproduce.

Private G15 DCDM/GCDM/USC performance-counter tables do exist, but no exact host path was found that exposes them as a passive, launch-resolution observer. Their existence alone is not evidence that Linux should program private counter MMIO.

## E713 — convergence with XDC 2026 and current public M3 work

Current public M3 work was re-ingested rather than treated as a stale reference.

Two apparent discrepancies collapse cleanly:

1. XDC describes G15 as having a 43-bit GPU address space. Public m1n1 and the Linux research model describe two bank-local 42-bit walkers selected by VA bit 42. These are the same geometry, not competing interpretations.
2. XDC's G15 page-manager and StartCompute pointer families map onto resource classes already reconstructed in the local path. They do not expose an extra hidden host prerequisite.

The XDC entry-shader model is especially useful independent confirmation. G15 no longer relies on the older USC-descriptor model: the pipeline reaches an entry/pre-shader which must itself execute the state-loading/allocation instructions that establish the real shader/resources. That aligns directly with the local failure cut: Linux reaches the entry mapping but does not interpret the first entry instruction.

The public XDC kernel demo reaches a G15 RunWorkQueue/doorbell path and then times out. No public source recovered by this checkpoint demonstrates a later positive activation state that contradicts the narrower local boundary.

## E716 — reset/default state closed

A final reset/default audit rediscovered a small cluster of exact retained-firmware G15 startup register writes. They initially looked interesting because several had been catalogued without a dedicated result note.

They are not Linux-missing host writes: the same retained RTKit image performs those startup writes on Linux before the selected command. Existing live firmware evidence proves that startup sequence completes, while separate audits close the hardware-read/RMW inputs, GFX power-retention behavior, Apple Fender/GPC reset path, successful-m1n1 direct-MMIO history, and pre-GPU platform ownership.

Thus no software-visible reset/default producer remains that distinguishes the working Apple/m1n1 path from the failing Linux path at entry-interpreter admission.

## E717 — selected ChannelState closed end to end

The selected `_AGFIChannelState` was revisited specifically to avoid overlooking a large shared object after earlier work had concentrated on QueueInfo and one DAG record.

Prior exact work already proves:

- the host clears the complete `0x24c0` ChannelState before rebuilding it;
- the QueueInfo prefix occupies `0xb0` bytes;
- QueueInfo `+0x18` points to `ChannelState +0xb0`;
- retained firmware uses that pointer as the base of 128 fixed `0x48`-byte stamp/DAG entries.

The arithmetic is exact:

`0x24c0 - 0xb0 = 0x2410`

`128 * 0x48 = 0x2400`

leaving only `0x10` trailing bytes.

A fresh enumeration of the retained firmware consumer family confirms entry indices are bounded/wrapped to seven bits, the stride is always `0x48`, and recovered field accesses stay within the entry record. No recovered consumer reaches the final 16 spare bytes.

So the ChannelState does not hide a second command buffer, a forgotten activation flag, or a passive autonomous-progress mirror. Its host image and firmware runtime use are already accounted for.

## Closed root-cause families at this checkpoint

Do not reopen these without contradictory evidence:

- Queue/WorkQueue transport, stamp registration and scheduler acceptance;
- RunCompute/SKU/RegisterArray contents and remaining command-local defaults;
- selected QueueInfo/ChannelState/DAG/stamp-state ownership and contents;
- UAT/GPTBAT/PTE, mapping classes, cache/TLBI and publication ordering;
- PagePool/FList/UMA and persistent command-resource backing;
- q4 runtime policy, soft-fault state and normal register-override state;
- retained startup fixed writes and startup RMW/read-branch inputs;
- PMGR/GFX retention and Apple Fender/GPC reset/power paths;
- c040/c048/c050/c058 software publication;
- c120/c140/c148 common engine activation and c128 completion;
- primary context-save and later replay/quiesce machinery;
- currently known Apple host/firmware diagnostic interfaces.

## Current research rule

A new protected live experiment now requires genuinely new evidence about the autonomous hardware interval itself.

Useful evidence would be one of:

1. a newly published or independently working G15 trace that exposes state after entry fetch and before instruction interpretation;
2. a hardware/debug semantic that can be tied mechanically to that interval and observed through an already-safe transport;
3. a concrete working-M3/Linux parity difference with a proven consumer edge at entry-interpreter admission.

Merely finding another register name, performance-counter table, shared-memory field or recovery status is not sufficient.

Runtime activation remains research-only and fail-closed.
