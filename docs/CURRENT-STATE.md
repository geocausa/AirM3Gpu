# Current G15 Bring-up State

Research state: 2026-08-27

Last live checkpoint: 2026-08-27

## Hardware identity

Target: J615 MacBook Air M3 / T8122.

Runtime identity registers:

- `ID_VERSION = 0x07022000`
- `ID_COUNTS_1 = 0x0011010a`
- `ID_COUNTS_2 = 0x00040404`
- active core mask: `0x3ff`
- topology: 1 MGPU/cluster, 10 cores, 10 fragment units, 4 GPs

## Closed runtime gates

The following stages have been independently exercised on real hardware with fail-closed recovery:

1. Device discovery and G15G C0 identity validation.
2. GFX ASC start/stop and 42-bit G15 UAT handoff.
3. Exact J615 14-state `PwrConfig` validation.
4. Byte-validated G15 startup InitData/HwData images.
5. RTKit management protocol v12, EP1 crashlog backing, and EP20/EP21 application endpoint startup.
6. Native G15 `MSG_INIT` handoff and post-init bootstrap.
7. Persistent GpuManager/RTKit lifetime and DRM registration.
8. `/dev/dri/renderD128` exposure with submission paths still explicitly lab-gated.
9. Safe GET_PARAMS, VM creation/destruction, GEM host lifecycle, and unbound VM mappings.
10. Passive queue/context lifecycle with host-only fail-closed cleanup.
11. Native q22 mapping notification/pressure handling.
12. G15 shared-bank1 and range-7 page-table spine/L3/leaf handling.
13. J615 MTR sensor initialization sufficient to remove the earlier MTR alarm boundary.
14. A signed, one-shot empty Compute QueueInfo publication used only to test accelerator transport.

No normal render workload is enabled at this checkpoint.

## Current pipe transport boundary

The bounded empty QueueInfo probe publishes no GPU command-ring entry. For Compute priority 2 it advances the G15 TX `WriteIndex` from 0 to 1 and rings EP21.

The exact G15 TX descriptor is:

1. ReadIndex
2. CFIIndex
3. WriteIndex
4. ring GPU pointer

The Linux PipeChannel descriptor and the RuntimePointers-exported descriptor were proven to reference the same exact objects. The host-side publication barrier/order and the Compute priority-2 work doorbell (`0x008300000000000a`) also match the reconstructed Apple contract.

The live result remains:

- before publication: `Read=0, CFI=0, Write=0`
- after host publication: `Read=0, CFI=0, Write=1`
- after bounded wait: `Read=0, CFI=0, Write=1`
- firmware statistics tag `0x0f` is emitted during the wait
- publication times out and backing is retained fail-closed

The tag proves that the EP21 activity is reaching firmware far enough to generate firmware-side telemetry, but the real pipe consumer does not retire the entry.

## Submission-time wake/power closure

Apple G15 has a priority wake note separate from the work doorbell. For priority 2 the exact note is `0x0083000000000008`. Sending that note before the bounded Compute publication does not advance Read/CFI.

`AGXAccelerator::notifyFirmware(priority, false)` also calls `ensurePoweredHardware(false)`, but exact J615 reconstruction proves that this function performs no submission-time power transition on the target:

- `isPowerManagedInAGX()` is `+0x650` bit 10 and J615 leaves it clear;
- base `configureDevice()` writes `strh 5` at accelerator `+0x6c0`;
- little-endian layout therefore initializes feature byte `+0x6c1 = 0`;
- the associated `+0x5d1` static-power gate is consequently false;
- `ensurePoweredHardware(false)` bypasses `changePowerStateTo(1)` and returns.

The missing boundary is therefore not a normal submission-time PMGR or GFX wake operation.

## Ruled out at this checkpoint

Current evidence rules out:

- incorrect G15 TX qword ordering;
- CFI being a host WriteIndex shadow;
- a stale RuntimePointers pipe descriptor;
- an obvious missing CPU/DMA ordering barrier before EP21;
- wrong Compute/priority-2 work-doorbell encoding;
- missing G15 priority wake note;
- missing normal J615 `ensurePoweredHardware(false)` transition;
- the earlier MTR sensor initialization failure;
- absence of the shared-bank1/range-7 mapping backend.

## Callback-gate closure — E041 through E043

E041 passively decoded statistics tag `0x0f`: the record contains a software-state transition and reports `state=1` during the failed empty publication. The pipe nevertheless remains `Read=0, CFI=0, Write=1`, so the tag is not a retirement acknowledgement.

E042 then reconstructed the real G15 firmware callback boundary. `g15_pipe_work_callback` checks an internal runtime-power byte at `DAT_fffffc000010e528` with mask `0x78` before calling scheduler `FUN_fffffc0000006a0c`. Compute maps to callback argument 2. Its narrow bypass calls `FUN_fffffc000000cf58()`, which returns nonzero only when one of sixteen firmware work-state slots is already non-idle.

The callback can emit KTrace `0x100` on entry and `0x207` when the internal gate blocks scheduler entry. Both are guarded by trace-class bit 2 copied from q21 `host_flags`, and the records are written into the exact KTrace ring Linux already publishes.

E043 enabled only that trace class after all existing pre-RTKit exactness checks and before `MSG_INIT`. The boot remained healthy and the signed empty Compute publication reproduced the same timeout with no crash or IOMMU fault. Selected `0x100/0x207` records were not observed, but that result is not yet sufficient to claim the callback was skipped because the KTrace class itself had not been independently calibrated live.

## E045 closure and current boundary

E045 enabled q21 trace bits 1+2 (`host_flags=0x6`) and repeated one signed empty Compute/priority-2 publication. Firmware emitted one callback-entry `0x100` record (`args[0]=1`), no callback-blocked `0x207`, and a healthy class-2 calibration stream. The TX pipe still remained `Read=0, CFI=0, Write=1` and timed out fail-closed.

The callback argument is a dispatch/wake class, not PipeType: `FUN_6a0c` subsequently scans all four priorities and all V/F/C channels. Thus arg 1 does not imply a Fragment doorbell.

The outer RTBuddy runtime-power gate is exonerated for this failure because callback entry is observed and the `0x207` blocked path is not taken.

## E046/E047 scheduler-entry control

E046 exposed the existing bit-1 scheduler markers (`0x111`, `0x112`, `0x128`) but retained trace bit 2, whose high-rate `0x213` stream could pressure the 512-entry KTrace ring. E047 therefore repeated the same experiment with **bit 1 only** (`host_flags=0x2`). This removed the class-2 pressure confounder without changing queue, power, PMGR, doorbell, MMU, or submission semantics.

The E047 signed empty Compute/priority-2 publication produced exactly one callback-entry `0x100` record (`args[0]=1`) and then no callback exit `0x101`, no scheduler snapshot `0x112`, no accepted-entry `0x111`, and no bounded scan `0x128`. These counts remained unchanged more than a minute later. The TX pipe remained `Read=0, CFI=0, Write=1` and timed out fail-closed.

Exact callback control flow is: entry trace -> runtime-power gate -> `FUN_fffffc000003d330()` -> `FUN_fffffc0000006a0c(param_1)` -> exit trace. Because entry is observed but neither the first `FUN_6a0c` trace nor callback exit ever appears, the active firmware thread does not return from **`FUN_3d330()`**, the cold power/setup transaction.

The callback argument is a dispatch/wake class, not PipeType; `FUN_6a0c` itself scans all priorities and V/F/C rings. The earlier assumption that Compute must imply callback arg 2 is therefore rejected.

The next experiment should remain passive: correlate q21/q4 host-visible shared power/runtime fields with the `FUN_3d330` transaction at EP20 receive times. No direct PMGR register poke and no real command-buffer submission is justified yet.

See `research/g15/G15-PIPE-SUBMISSION-BOUNDARY.md` and `research/g15/G15-PIPE-CALLBACK-GATE.md`.

## Source checkpoint

The current clean Linux checkpoint head is:

`2f08f68bb2efdadf2d337441553c1f682152a748`

`patches/linux/0004-drm-asahi-checkpoint-G15-through-empty-queue-boundary.patch` is a squashed delta from the previous public checkpoint head `1b57b289af96973badfbb8489ef379a1b3a96f07`. It has been validated in a temporary Git index to reconstruct the exact `2f08f68` tree.

## Explicitly not enabled

- no production G15 render path claim
- no general command-buffer submission
- no user workload execution through the experimental empty-publication gate
- no direct experimental PMGR writes
- no upstream AsahiLinux push implied by this public checkpoint

The project remains a staged clean-room bring-up with one-shot live gates and Golden-kernel recovery.
