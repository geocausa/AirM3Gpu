# G15 E428–E508: real-Compute frontier is post-KickStart and pre-body

Checkpoint: 2026-10-07

Target: MacBook Air M3 / J615 / T8122 / G15G C0. Exact implementation authority remains macOS 14.8.3 build 23J220 and its matching G15 firmware. Current macOS is used only as a dynamic structural oracle.

## Executive result

The Linux bring-up now crosses generic G15 Compute transport and reaches the real-Launch firmware execution path far enough to pass the Apple-equivalent Compute KickStart boundary, but the selected shader body still never executes.

The durable boundary is:

1. normal queue/VM creation succeeds;
2. a real Compute launch is accepted and scheduler transport retires;
3. firmware parses Compute opcode `0x0b`, activates the selected hardware slot and publishes the expected G15 register/dependency state;
4. start-timestamp bookkeeping is parsed and firmware reaches WFI;
5. firmware-owned fault state reports no classified page/MMU fault;
6. host-visible channel state remains `wptr=1 / doneptr=0`;
7. the real shader result remains at its initial value, proving the useful shader-body store never executed;
8. no hardware completion/release reaches the firmware WFI path.

A successful Apple Metal trace was back-translated to the exact retained firmware and proves this Linux path is already **past Apple's Compute KickStart-equivalent event**. The unresolved prerequisite is therefore downstream of scheduler/KickStart admission but upstream of useful USC body execution or hardware completion.

## Milestones that establish this boundary

### E428 — exact-Golden-ABI G15 module milestone

A modern accumulated G15 Asahi module was built against the frozen Golden Linux ABI, bound J615/G15G C0, created DRM/render nodes and reached persistent manager/initdata/RTKit/Compute-ready state. This is the stable module-level starting point for later experiments.

### E451 / E463 — control-plane and initialization stability

A marker-only driver completed the full G15 init path without the earlier unsafe direct scheduler-state observations. A byte-identical resident control later proved OPEN, GET_PARAMS, VM_CREATE, QueueCreate and ordinary zero-Compute submission can proceed normally, rejecting several timing/instrumentation-induced apparent control-plane stalls.

### E475 — terminate-only positive control

A terminate-only Compute command completes immediately. This exonerates generic queue publication, RunWorkQueue scheduling, slot allocation and the basic firmware completion machinery.

### E480 / E481 — command is admitted but never retires

Non-perturbing CPU-side snapshots show the stuck real command remains `wptr=1 / doneptr=0`. Exact firmware parser reconstruction proves opcode `0x0b` continues through start-timestamp handling into WFI in the same parser invocation. Retirement/end-timestamp handling never happens.

### E483 / E484 — translation-visibility hypothesis rejected

Linux had no post-map UAT invalidate after per-job mappings, unlike a known working historical path. A single-variable live candidate added an ASID-wide invalidate and barrier after all selected G15 job mappings and before queue publication. The launch still stalled identically, rejecting translation visibility as the missing prerequisite.

### E487 — decisive pre-vs-post shader discriminator

The diagnostic was corrected to inspect the actual current `RunCompute.g15_result` object. On a real accepted launch the value remained `0xffffffff` immediately, at 250 ms and at 5 s. The shader body therefore never reached its first observable store.

### E503 — successful Apple KickStart back-translation

A fresh complete Metal System Trace on current macOS recorded a bounded successful Compute workload. Xcode's modeled events identify Compute KickStart, and exact retained firmware maps that semantic event to event ID 7 at the beginning of Compute case `0x0b`, before per-slot publication. Linux is already proven to reach beyond that point.

A separate first-store sentinel experiment showed CPU visibility of a shared buffer only near command completion, so CPU-visible shared memory is not a valid clock for first USC instruction on this path.

## E505–E508: first-use spill/UMA branch closed for the exact target

### E505 — current-macOS first-real-Compute allocation clue

A successful Apple workload shows a one-time first-real-Compute Wire Memory burst of roughly 47 MiB, including twenty consecutive 2 MiB regions. An empty command causes none, and later larger Compute dispatches do not repeat the allocation. This proves a first-use resource exists in the current Apple stack, but not its exact 23J220 identity.

### E506 — exact 23J220 UMAPool sizing does not impose that floor

Exact retained `AGXUMAPool::prepareLocked`, `getMinPoolSize`, `getIdealPoolSize` and `allocatePoolMemory` were reconstructed. For the selected bounded minimal diagnostic, raw Compute min/ideal requests remain zero and first-epoch FList policy/accounting values start at zero. Exact target desired growth therefore remains zero; there is no target rule requiring a ~40 MiB UMA allocation on the first real Compute.

### E507 — current Apple spill/UMA request machinery is real but cross-build

A live current-macOS LLDB oracle shows `checkSpillParamsForCompute` and `allocateUSCSpillBuffer` execute twice per real Compute and not for an empty command. Two stable current-build descriptors are regenerated on tiny, large and repeated large dispatches. This is useful structural evidence for repeated request production plus reusable backing, but the descriptor ABI is current-build private state.

### E508 — exact-target back-translation rejects transplantation

Exact 23J220 already defines the firmware-facing request chain:

`endComputePass raw +0x138/+0x140`
→ parsed Compute wrapper
→ CL descriptor `+0x640/+0x648`
→ UMA prepare
→ RunCompute `+0x847/+0x84f`.

For the selected diagnostic those target requests remain zero. Therefore the current-build spill descriptor values and the tempting ~40 MiB arithmetic must not be copied into Linux, and Linux must not synthesize a 40 MiB FList/UMAPool for this command.

## Current engineering frontier

Do not spend the next live candidate on completion-side polling, direct scheduler MMIO, post-bind TLBI, or synthetic UMA growth. Those branches are closed or lower value.

The next discriminator should target **pre-body program/USC execution activation or another exact-23J220 non-UMA first-use global/context resource**. The preferred workflow is:

1. ask current macOS one precise dynamic question when it can separate a successful path from Linux;
2. map that observation back to retained 23J220 authority;
3. only then make one exact-target Linux delta;
4. test it as the first/only experimental module load on a fresh Golden boot with the established freeze/watchdog/evidence protocol.

The current-macOS first-use allocation remains useful only if its owner/tag/class can be mapped mechanically to an exact 23J220 resource. Otherwise continue directly with exact LoadShader/program-state publication, executable/program activation and USC-global-state reconstruction.

## Do-not-reopen list

- direct `c040` observations: closed as active scheduler/DAG ownership-dependency state;
- direct banked `d8c0` fault reads: semantically wrong for G15;
- MMU/page-fault hypothesis: firmware-owned classified state stays clear for the stuck launch;
- post-bind ASID/TLB invalidation: tested live and negative;
- generic queue/WorkQueue scheduling: terminate-only control passes;
- minimal 23J220 UMA/FList growth: exact target closure says zero demand for the selected diagnostic;
- current macOS private spill constants: discovery only, never target ABI.
