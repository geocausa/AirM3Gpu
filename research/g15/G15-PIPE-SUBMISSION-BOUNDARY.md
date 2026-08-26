# G15 Pipe Submission Boundary — J615 / T8122

Research checkpoint: 2026-08-26

This note records the clean-room boundary reached after first G15 DRM registration and a deliberately bounded empty-queue publication probe. It contains derived facts only; no Apple firmware, kernelcache, or raw proprietary capture is included.

## Live boundary reached

The isolated J615 candidate now reaches all of the following without executing a GPU command buffer:

- native G15 `MSG_INIT` handoff and post-init bootstrap;
- persistent RTKit/GpuManager lifetime;
- DRM registration and `/dev/dri/renderD128` creation;
- safe GET_PARAMS, VM lifecycle, GEM host lifecycle, and unbound VM mapping;
- passive queue creation/destruction;
- native q22 mapping notification and shared-bank1/range-7 page-table handling;
- a signed, single-shot empty Compute QueueInfo publication with work-queue `wptr = 0`.

The empty publication is intentionally incapable of exposing a command-ring entry. It is used only to test the accelerator transport boundary.

## Exact G15 TX descriptor

Apple's G15 runtime wrapper exports four qwords for each TX pipe:

1. `ReadIndex`
2. `CFIIndex`
3. `WriteIndex`
4. ring GPU pointer

`CFIIndex` is firmware-owned cache-invalidation progress. It is not a host mirror of `WriteIndex`.

Linux's G15 descriptor ordering matches this contract. Runtime telemetry also proved that the PipeChannel-owned descriptor and the descriptor exported through RuntimePointers reference the same exact Read/CFI/Write/ring objects. A stale descriptor copy is therefore ruled out.

## Publication and doorbell ordering

The host-side publication ordering matches the Apple path closely enough to rule out the obvious transport hypotheses:

- fill the 24-byte pipe entry;
- issue the required ordering barrier;
- advance `WriteIndex`;
- ring EP21.

For Compute priority 2, the work doorbell is:

`0x008300000000000a`

Apple G15's separate priority wake note is not the inherited pre-G15 `...0010` KICKFW message. Its exact priority-2 value is:

`0x0083000000000008`

A one-shot experiment sent that exact wake note before the bounded Compute publication. It did not change the result.

## Live result

At the decisive boundary the host publishes one Compute entry:

- before: `Read=0, CFI=0, Write=0`
- after host publication: `Read=0, CFI=0, Write=1`
- after one-second bounded wait: `Read=0, CFI=0, Write=1`

Firmware emits statistics tag `0x0f` during the wait, proving the doorbell is not simply disappearing at the mailbox layer. However, neither `ReadIndex` nor `CFIIndex` advances and the publication times out.

No MTR alarm, DART/IOMMU fault, RTKit crash, kernel panic, watchdog, command parser execution, or GPU command execution is observed in the bounded run. QueueInfo backing is retained fail-closed after uncertain publication.

## Submission-time power hypothesis — closed

`AGXAccelerator::notifyFirmware(priority, false)` calls `ensurePoweredHardware(false)` before the G15 wake note. That initially looked like a missing Linux power transition. Exact J615 reconstruction closes that hypothesis:

- `isPowerManagedInAGX()` is accelerator configuration bit 10 at `+0x650`;
- J615 G15/G15G configuration leaves that bit clear;
- base `AGXAccelerator::configureDevice()` executes `strh 5` at accelerator `+0x6c0`;
- on little-endian ARM64, that initializes `+0x6c0 = 5` and feature gate `+0x6c1 = 0`;
- the later `+0x5d1` static-power gate is consequently also false;
- with boolean argument `false`, `ensurePoweredHardware()` therefore bypasses `changePowerStateTo(1)` and returns.

So normal J615 submission does **not** perform the suspected `state 2 -> 1` power transition. The exact G15 priority wake experiment already matched the normal submission-time Apple path.

## Ruled out at this checkpoint

The current evidence rules out these explanations for the first pipe timeout:

- wrong TX qword ordering;
- treating CFI as a host WriteIndex shadow;
- stale RuntimePointers descriptor copy;
- missing generic host memory barrier before EP21;
- wrong Compute/priority-2 work-doorbell encoding;
- missing G15 priority wake note;
- missing normal J615 `ensurePoweredHardware(false)` transition;
- MTR sensor initialization failure;
- shared-bank1/range-7 page-table absence.

## Remaining boundary

The next target is earlier than submission: reconstruct the RTBuddy/RTKit runtime-state initialization and the firmware state-machine condition that permits the EP21 pipe callback to enter the real pipe consumer.

RTKit-2419's pipe work callback can receive/schedule the doorbell while still skipping the real consumer under firmware power/runtime-state bits. That behavior matches the observed combination of `stats tag 0x0f` plus unchanged Read/CFI indexes.

No direct PMGR register write is justified by the current evidence. The next experiment should remain offline until the relevant RTBuddy/firmware state transition is mechanically identified.
