# Current G15 Bring-up State

Research state: 2026-08-26

Last live checkpoint: 2026-08-26

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

## Current boundary

The next target is **pre-submission RTBuddy/RTKit runtime-state initialization** or an equivalent firmware state-machine gate.

RTKit-2419's pipe work callback can be scheduled by a doorbell yet skip the real pipe consumer under firmware runtime/power-state conditions. That is consistent with the observed `stats tag 0x0f` plus unchanged Read/CFI indexes.

No direct PMGR register poke and no real command-buffer submission is justified yet. The next live experiment must wait until the relevant RTBuddy/firmware state transition is mechanically identified offline.

See `research/g15/G15-PIPE-SUBMISSION-BOUNDARY.md` for the exact transport closure.

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
