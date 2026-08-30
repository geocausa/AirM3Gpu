# J615 two-phase Compute fence arm — E134

E134 closes the host-fence ordering prerequisite identified by the E112 integration audit without making the G15 Compute transaction live. No module was installed and no RunCompute or custom GPU command was issued.

## Problem

The dormant E110/E113 event-control and SKU retirement guards intentionally require `JobFence.pending != 0` before binding an in-flight command. The ordinary Linux compute constructor, however, still builds the complete `RunCompute` object before calling `fence.add_command()`.

Moving that increment earlier without ownership would be unsafe: any later constructor, serializer, callback-registration, or explicit abort failure would have to remember to decrement it exactly once.

## E134 model

Linux commit `a244a846e3014b9eebdec580f852e39d27a1a50d` adds private versioned `G15CommandFenceArm` around the existing `JobFence` API:

- construction clones the submission `UserFence<JobFence>` and executes the existing `add_command()` once;
- `Drop` executes the existing `command_complete()` once;
- E113 phase 1 creates the arm before event-control/SKU guard binding;
- both guards bind to the arm-owned fence, so the in-flight predicate is guaranteed at that dormant boundary.

The same arm is carried through two private host-only tokens:

- `G15ArmedUnpublishedStockEmptyPrepare` for phase 1;
- `G15ArmedUnpublishedRunComputeFieldStage` after command-aware SKU finalization.

Existing rollback remains explicit for the GPU/resource state. Phase-1/finalize failures roll fresh event/SKU bindings and FList ownership before the arm is dropped. Explicit abort does event/SKU/FList rollback first, then drops the arm. Explicit completion releases the FList HardwareBuffer epoch first and only then drops the arm, after which retained slot guards can observe a completed submission fence and scrub/reuse normally.

The arm is not claimed to make arbitrary raw token dropping a complete asset rollback mechanism; these types remain private and unreachable and are consumed only by the controlled dormant transaction API.

## Live path remains unchanged

The normal `submit_compute()` source still contains its original successful-path sequence:

`fence.add_command() -> comp_job.add_cb(...) -> fence.command_complete()`

E134 does not instantiate the dormant owner graph or guards, does not add a Queue call site, does not write the host staging record into `RunCompute`, and does not alter any command bytes.

## Validation

- base `f09bed89530e5fa482240c55653545a3bed3e292`;
- Linux commit `a244a846e3014b9eebdec580f852e39d27a1a50d`;
- Linux tree `179518d4f4d3d41ce658800f2f0e92ad7b2d176e`;
- strict checkpatch `0/0/0`;
- external module build PASS at the established 24-individual-warning baseline;
- module SHA-256 `b6f65074d982472119d326a1eebb286a0680aa57c002fda0c9f0ec32ef9747e9`;
- patch 0050 exact-tree reconstruction PASS;
- no install / no RunCompute.

The remaining integration boundary is persistent placement/co-ownership of the owner graph and retirement guards under the real queue/channel lifetime, followed by a controlled command writer only after that lifetime has an exact rollback/completion home.
