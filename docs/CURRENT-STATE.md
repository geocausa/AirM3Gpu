# Current G15 Bring-up State

Research state: 2026-08-28

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
14. A signed, one-shot Compute QueueInfo registration using one already-satisfied firmware Barrier record, followed by clean native G15 `ReleaseResource` teardown.

No normal render workload is enabled at this checkpoint.

## Historical pipe transport boundary

The original bounded empty QueueInfo probe exposed no command-ring entry. For Compute priority 2 it advanced the G15 TX `WriteIndex` from 0 to 1 and rang EP21, but firmware initially left Read/CFI at zero. That historical boundary drove E041-E056.

The exact G15 TX descriptor remains:

1. ReadIndex
2. CFIIndex
3. WriteIndex
4. ring GPU pointer

The Linux PipeChannel descriptor and RuntimePointers-exported descriptor reference the same objects, and the Compute/priority-2 work doorbell is `0x008300000000000a`.

This boundary is now superseded. E057-E060 reach scheduler acceptance and retire the barrier-only registration to `(Read, CFI, Write, Shadow) = (1,1,1,1)`, then complete native context/resource release. No RunVertex/RunFragment/RunCompute command is used by that proof.

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

## E045 closure

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

## E048-E051 cold-power handshake closure

E048 added passive q21 snapshots around the bounded publication. During the failed first Compute publication q21 changes from `busy=0, unk10=0, ready=1, power=0` to `busy=0, unk10=1, ready=1, power=1`. The scheduler `busy` bit never asserts.

E049 attempted broad class-2 power-dispatch logging but amplified a hot firmware trace stream into millions of host log records; that experiment is retained only as an instrumentation failure. E050 corrected the receiver by draining KTrace without generic per-record logging and promoting only bounded decision IDs. In the clean run, callback `0x100` and pstate `0x200` appeared; the state-1 KTrace timestamp matched statistics tag `0x0f/state=1`, while no callback exit or scheduler marker appeared.

E051 then admitted `FUN_447ec` arg `1` into the bounded `0x20b/0x20c` filter. The live sequence was:

- callback entry `0x100`, arg 1;
- power-dispatch entry `0x20b`, arg 1;
- pstate `0x200`, state 1;
- power-dispatch exit `0x20c`, arg 1;
- q21 settles at `busy=0, unk10=1, ready=1, power=1`;
- TX Read/CFI remain zero and the bounded probe times out fail-closed.

Exact firmware ordering matters: `FUN_447ec(case 1)` calls `FUN_14f98(..., state=1)`, then executes `FUN_4a560(&event_slot_1)`, and only afterward emits `0x20c`. Therefore E051 proves both the state-1 power worker and the event-1 post primitive return successfully. `FUN_18864` and `FUN_d300` in the later `FUN_3d330` tail are non-blocking bookkeeping. The current target is the remaining short post-handshake tail before `FUN_6a0c`, beginning with the always-fired `0x10078` notification path and the q22 `+0x4030` optional branch.

## E052-E060 — scheduler registration lifecycle closure

E052 confirms q22 `+0x4030` remains zero across the old timeout, ruling out the optional CPMS tail as the blocker. E056 then identifies the material host-layout error: G15 HwDataA `+0x4188..+0x41db` is a DPE leakage-update image, not the sparse SoCHot structure previously assigned there. Apple's exact J615/C0 image includes HwDataA `+0x41d8 = 1` and mirrored q23 `+0x1a8 = 1`. Firmware case `0x14` consumes this state when arming its DPE timer.

After restoring the DPE image, E057 crosses the long-standing pre-scheduler boundary:

- callback `0x100` is observed;
- q21 `busy` changes `0 -> 1`;
- scheduler `0x112` snapshots appear;
- one `0x111` record accepts the Compute/2 QueueInfo.

E057 then crashes because the old diagnostic exposed untouched stamp-index sentinel `0x80`. E058 proves that merely substituting EventManager slot 0 is insufficient: firmware's internal per-slot state pointer is still null. Static reconstruction identifies the missing edge in the G15 Barrier/type-4 helper, which binds the RunWorkQueue stamp slot to QueueInfo `+0x18` (`gpu_buf`).

E059 publishes exactly one already-satisfied firmware Barrier record at QueueInfo ring entry 0 and uses `wptr=1`. There is still no RunVertex, RunFragment, or RunCompute object and no shader command stream. The result is the first complete registration:

- `0x111` accepts the QueueInfo;
- callback exit `0x101` appears;
- the pipe reaches `(Read, CFI, Write, Shadow) = (1,1,1,1)`;
- QueueInfo registration returns success;
- context/resource bytes become `(02,00,00,0f)`.

E060 closes teardown. Apple's G15 `submitReleaseResource()` opcode is `0x11`; Linux's versioned enum accidentally encoded `0x12` because it retained a legacy V13.3 `Unk0d` placeholder on G15. Excluding that placeholder from G15 numbering, plus a fail-closed pre-send `0x11` assertion, makes native `ReleaseResource` return success. The temporary command-submission gate returns `1 -> 0`, software-state telemetry returns `1 -> 0`, and no RTKit/GPU/DART/kernel fault is observed.

The current boundary is therefore **after scheduler registration and native resource retirement**. The next work is static reconstruction of the smallest real G15 scheduler command contract. Live shader/render/compute execution remains blocked until its pointer, stamp, dependency, and completion prerequisites are closed.

See `research/g15/G15-QUEUE-REGISTRATION-LIFECYCLE.md`.

## E061-E063 — first Compute execution contract

E061 statically closes the next scheduler boundary: a normal G15 RunCompute/type-3 command is inherently hardware-facing. RTKit installs command `+0x760` as the engine-2 execution stream, and Apple unconditionally produces the G15 Compute RegisterArray plus SKU stream. There is no mechanically justified parser-only RunCompute shortcut.

E062 closes the SKU framing ambiguity. The ordinary no-feature stream is `0x2b8` bytes before alignment and exactly `0x2c0` after Apple's 0x40-byte rounding. Its fixed skeleton is opcode `0xb` + 0x1b8 payload, a 0x3c-byte start timestamp record, one Compute WFI dword `1`, a 0x3c-byte end timestamp record, a 0x7c-byte opcode-`0xc` record, and finish dword `0x40000002`. The optional paired feature records make the aligned stream `0x300`.

The two remaining G15G dynamic register IDs are also fixed for J615: accelerator vslots `+0x10a8/+0x1090` return `0x101d8/0x107a0`. Register `0x1a420` still receives the raw Compute control-stream pointer, so reproducing SKU framing alone does not make a live command inert. The current target is a mechanically proven harmless CDM control stream plus its UMA, stamp, timestamp, and completion prerequisites. No live RunCompute is enabled.

E063 closes the next host-side layer. `generateRegisterList()` directly programs CDM register `0x1a420` from the raw Compute control-stream pointer. Apple's Gen4 `patchCDMControlStreamAndReset()` emits exact `0x60000160` and `0x60000960` token forms through separate CDM token pools, with 16-byte pointer/state patch records carrying the `0x20000000` address encoding, then clears cached stream state. `endComputePass()` invokes this framing before its normal `0x40000000` end-of-pass command. These are exact patch/reset records, not yet proven harmless standalone payloads. The current target is therefore semantic classification of the smallest no-threadgrid/no-shader CDM token sequence before any live RunCompute.

E064 cross-checks those constants against Mesa's independent CDM grammar: `0x60000160/0x60000960` are Barrier blocks, `0x20000000` is Stream Link, and `0x40000000` is Stream Terminate. Apple G15 emits the terminate token as one dword. A terminate-only root therefore contains no Launch block by construction; the remaining proof is that a normal Apple zero-dispatch Compute container can carry that root with valid associated objects. No live RunCompute is enabled.

See `research/g15/G15-COMPUTE-LAUNCH-BOUNDARY.md`, `research/g15/G15-COMPUTE-SKU-STREAM.md`, and `research/g15/G15-COMPUTE-CONTROL-STREAM.md`.

E065 then captures Apple’s normal direct Compute CDM packet: one 1x1x1 launch is 10 dwords (`Launch Word0`, pipeline/Launch Word1 state, global size, local/workgroup size) followed by the ordinary `0x60000160` Barrier. This independently identifies the first 40 bytes as the actual dispatch block rather than patch/reset framing.

E066 moves the outer-container proof onto the exact macOS 14.8.3 / 23J220 ABI used by the M3/J615 Linux target. Apple’s own `AGXFirmware::configurePoolElementSizes()` fixes Compute/CLE at **0x880 bytes**, exactly matching the independently reconstructed Linux `RunComputeG15V14_7`. The exact 23J220 accelerator-ring entry is **0x18 bytes** with fields at `+0x00/+0x08/+0x10/+0x14/+0x16/+0x17`, also exactly matching Linux; Compute is data-master/pipe type **2**. Exact 23J220 `submitReleaseResource()` again uses opcode **0x11**.

E067 closes the cross-build RegisterArray identity question on the exact target. The matching 23J220 KDK exposes kernel-side `AGXCLChannelG15::generateRegisterList()`, and its exact order, descriptor offsets, J615 dynamic IDs, `0x1a440`/`0x1a458` synthesis and optional tail match the independently reconstructed 25F84 program.

E068 then captures Apple’s stock no-dispatch Compute container and closes the remaining ordinary-list values. Its exact 0x1d0 raw Compute payload has all RegisterArray-fed raw state zero. Host-driver reconstruction further proves descriptor `+0x460/+0x468/+0x470` come from raw Compute `+0x158/+0x15c/+0x160`; these are also zero in the empty oracle. The resulting J615/G15G list is exactly 20 entries / `0xf0` bytes, with `0x1a440=0x154024201`, `0x1a458=0x10c08860`, and `0x107a0=0x00ff0000`. Linux now models this exact **empty-path** list compile-only. Ordinary G15 submission remains rejected with `ENODEV`, and no live Linux RunCompute is enabled.

E069 then proves on the exact 23J220 KDK that this empty descriptor is **not host-elided**. `processCompute()` proceeds through `processComputeSetup()`, `addComputeToWorkqueue()`, `AGXCLWorkQueue::submitCommand()`, and the ordinary CL-channel vslot `+0x148`; the exact G15 vtable resolves that slot to `AGXCLChannelSKU::submitBuffer()`. The same exact-target image also confirms the E062 SKU grammar (`0xb + 0x1b8`, 0x3c timestamps, WFI dword `1`, trailing `0xc`, final `0x40000002`; sizes 0x2c0/0x300). Compute data-master 2 selects `AGXAccelerator::submitCLChannel()`. Apple therefore carries the stock `0x1a420=0` no-dispatch descriptor through normal G15 Compute command construction/submission rather than dropping it on the host. Linux still does not issue RunCompute; UMA/context/timestamp/stamp/completion/recovery prerequisites remain the live boundary.

E070 closes the SKU source map on that same 23J220 image. `submitBuffer()` calls G15 register generation first and SKU encoding second, passing descriptor `+0x350` as the 0x880 command GPU/FW address. Compute timestamp packets use command-relative `+0x810/+0x818/+0x820/+0x828/+0x868`. Accelerator `+0x2488` is `AGXPerfCtrSamplerGen15`, proving the optional `0x90000004/0x10000004` pair is performance-counter state. Crucially, Linux must not use the inherited StartCompute/WaitForIdle/FinalizeCompute microsequence as G15 RunCompute `+0x760`: Apple publishes the SKU execution stream there.

E071 closes the exact inactive stock-empty SKU gate and the first UMA handoff byte. The relevant PerfCtr feature state and the empty descriptor input are zero, so the optional pair is absent and the stock empty path is exactly `0x2c0`. After `AGXUMAPool::prepareLocked()`, exact 23J220 `submitBuffer()` copies descriptor `+0x624` into RunCompute `+0x846`, requiring `+0x846=1`; Linux commit `f73b9e551658` corrects its previous zero. RunCompute `+0x810/+0x860/+0x870/+0x878` remain exact empty-path zeros, while `+0x85f` is dynamic and deliberately not hard-coded. Live RunCompute remains blocked on the Page-Pool/FW-uncached pointer, min/ideal/metrics values, dynamic context generation, and stamp/notifier completion/recovery ownership.

E072 closes the remaining stock-empty UMA value ambiguity and identifies the two live pointer producers. The raw empty UMA record remains zero at the qwords copied to descriptor `+0x628/+0x630`, so RunCompute `+0x847/+0x84f` are **exactly zero**; Apple's `getMinPoolSize()/getIdealPoolSize()` results govern backing allocation instead. RunCompute `+0x83e` is the live FW address of the FList-owned 0x70-byte `UMA Page Pool State`, and `+0x857` is the base FW address of the one-page `AGXUMAHWMetrics` mapping because descriptor offset `+0x620` remains zero. Linux already models the 0x70 ABI and 256-entry Page Pool descriptor table but has no G15 HardwareBuffer-ID allocator or Compute metrics owner, so those pointers remain zero/fail-closed rather than guessed.

E073 closes the managed context-ID/generation lifecycle on the exact 23J220 target. G15 has 64 context IDs with ID 0 reserved; user IDs 1..63 carry an 8-bit generation byte. Fresh assignment registers the GART then increments the generation, while sticky reuse preserves it. Linux already had the same 64-slot/GPTBAT context-ID mechanism, so commit `03fdbb86230f` adds the missing per-slot generation and exports it at RunCompute `+0x85f`. The same oracle pass proves the separate G15 HardwareBuffer manager has 0x100 IDs and owns FList mapping preparation/completion, so `+0x83e` still cannot be made live with a constant ID or naked pointer.

E074 closes the next ownership boundary for that live Page-Pool-State pointer. Exact 23J220 host reconstruction places the 0x70-byte `UMA Page Pool State` in eGartRange 8, immediately after the six already-proven range-7 parents in shared bank 1. A one-shot read-only Linux diagnostic at commit `4e26fc089860` verified shared-L2 entries 6 and 7 are empty on this exact J615, then stopped ASC and returned `ENODEV` before persistent InitData/RTKit/DRM. The machine returned to Golden with no GPU/DART/kernel fault and the experimental module/initrd restored byte-for-byte. The final range-8 leaf/PTE protection encoding remains unresolved, so no range-8 child table or `RunCompute +0x83e` pointer is published.

E075 closes that leaf/PTE encoding. The existing `0x00c0000000000447` range-7 PM leaf belongs to the ParameterManagement/IOGPUResource class; the FList SecureMemoryMap path is distinct. Exact 23J220 derives compact option `0x003` for the range-8 Page-Pool State, yielding exact leaf `0x00c0000000000443`. Linux commit `e9f50fcc17d5` published two bounded range-8 L3 parents plus one temporary leaf at `0xfffffc200c000000`, read the exact PTE back, cleared it, and detached all eight range-7/range-8 parents cleanly. ASC was stopped and the driver returned `ENODEV` before persistent runtime; no RunCompute or GPU command was issued. The persistent allocator remains range-7-only pending the next checkpoint.

E076 promotes the proven range-8 class into compile-only allocator ownership. Linux now keeps separate range-7 and range-8 `mm::Allocator` arenas beneath the same bank-1 page-table owner, tags each small-object allocator with an explicit aperture, and provides a range-8 constructor that fixes `PROT_G15_RANGE8_FW` rather than accepting PM/range-7 attributes from callers. All existing callers remain range 7 and the new constructor is intentionally unused, so this checkpoint creates no new persistent mapping and does not change the RunCompute pointer.

E077 closes the persistent FList resource constructor on exact 23J220. `AGXUMAFList::init()` creates four resources: Page Pool List and Backup Page List in eGartRange 5, a fixed 0x70-byte Page-Pool State in eGartRange 8, and a fixed 8-byte FW-Uncached State in eGartRange 7. The exact list-size formulas are `align_up(M >> 9, P)` and `align_up((M / B) * 64, P)` for pool maximum `M`, block size `B`, and host page size `P`. The corresponding G15G vslots are closed, but the override-sensitive `M/B` initialization is not, so the 4 MiB/32 KiB fallback results are intentionally not claimed as exact J615 values. A read-only stock user-client calibration (`type 5`, selector `0x10f`) succeeds but returns zero throughout the empty Compute lifecycle, proving it reports live accounting rather than those static limits.

E078 adds only the independently exact fixed mapping class. Exact 23J220 FList FW-Uncached State uses compact SecureGart option `0x00b`, giving range-7 leaf `0x00c000000000044b`; the existing PM/q22 class remains `0x00c0000000000447`. Linux commit `886820e1f460` adds an exact 8-byte `G15UMAFWUncachedState`, a dedicated FList range-7 aperture/allocator constructor, and hard aperture/protection pairing while both range-7 classes share one collision-safe VA allocator. This remains compile-only: neither FList fixed object is instantiated persistently and RunCompute `+0x83e` remains zero.

E079 reconstructs the separate 256-entry `AGXHardwareBufferIDManager` allocation state rather than substituting Linux's generic LRU slot allocator. Linux commit `865f24f2a9fc` models sticky per-object IDs, per-ID reference counts, the 256-bit zero-reference bitmap, Apple's initial LIFO stack arranged to allocate IDs 0 through 255, lowest-set-bit bitmap fallback for stealing dormant sticky IDs, stale-owner mismatch handling, and the exact prepare/complete transition booleans. The object is intentionally only a compile-time host state machine: it has no lock, no runtime instance and no FList callback wiring yet.

E080 adds the missing synchronization/host-owner boundary without activating it. Linux commit `ed17ac035ad2` wraps the exact E079 manager state in the driver's existing `Arc<Mutex<...>>` pattern and adds an FList-side sticky binding object whose prepare/complete methods expose the exact transition booleans. No manager is instantiated in GpuManager, InitData, queues or an FList resource owner, and no callback or GPU mapping is executed.

E081 adds a complete but side-effect-free FList resource plan. `G15FListGeometry` takes the unresolved exact producers `M/B/P` and computes Apple's checked formulas `align_up(M >> 9, P)` and `align_up((M / B) * 64, P)`; the fixed Page-Pool/FW-Uncached objects are compile-time pinned to 0x70/8 bytes. `G15FListResourcePlan` combines that geometry with the synchronized sticky HardwareBuffer owner but deliberately contains no GPU allocation type or allocator handle. It therefore cannot allocate, map or publish a Page-Pool State.

E082 closes the remaining FList list-mapping class on exact 23J220. The Page Pool List and Backup Page List callers use `mov x5,#3; movk x5,#0x200,lsl #32`, i.e. exact caller option `0x20000000003`; older E074/E077 prose had omitted two zeroes. Range 5 produces outer option `0x20500000007`, `SecureMemoryMap +0x58 = 0x20`, and exact compact UAT option `0x300`. Its protection bits are `0x0080000000000000`, bit-identical to the existing cached range-5 class. Linux commit `724674ad034e` adds a dedicated semantic FList constant and exposes it through the no-allocation resource plan. No allocator or GPU object is instantiated.

See `research/g15/G15-23J220-COMPUTE-ABI.md`, `research/g15/G15-23J220-COMPUTE-REGISTERARRAY.md`, `research/g15/G15-EMPTY-COMPUTE-REGISTERARRAY.md`, `research/g15/G15-EMPTY-COMPUTE-CONTAINER.md`, and `research/g15/G15-23J220-COMPUTE-SKU-SOURCES.md`.

## E061 — first real Compute is an execution boundary

Static E061 reconstruction identifies the previously unlabelled RTKit Compute launch hook at `0xfffffc00000251e8..0x256db`. After type-3 parser/DAG binding it registers RunCompute `+0x83e` as the UMA Page Pool State, propagates context-generation/selector state, then copies RunCompute `+0x760` into the per-stamp execution record at `+0x28/+0x30`. The common scheduler marks the command dispatched only after this hook returns.

Apple's matching producer is equally explicit. `AGXCLChannelG15::generateRegisterList()` builds the real G15 CDM register list, while `encodeCLCommandSKUStream()` emits a leading type-`0xb` SKU packet whose payload references the command RegisterArray (`+0x20`) and JobParameters2 (`+0x76c`), surrounds Compute WFI with timestamps, and publishes the final stream at `+0x760/+0x768`. Therefore a normal Apple-style RunCompute cannot be treated as a parser-only diagnostic; it is already hardware-facing.

RTKit's UMA registration accepts a zero-page descriptor, but the 0x70-byte Page Pool State must be real and later refresh requires a valid FW-uncached-state pointer/mirror. The next live command remains blocked until the minimum G15 register, SKU, UMA, notifier/context, stamp/completion, and recovery contracts are closed together.

See `research/g15/G15-COMPUTE-LAUNCH-BOUNDARY.md`.

## Source checkpoint

The current clean Linux checkpoint head is:

`724674ad034ee3502aa9448324cd0056b7b0c670`

Patch 0004 remains the squashed delta through the scheduler-registration boundary. Patch 0005 is the focused compile-only E068 delta from `2f08f68` to `1d264651`, adding only the exact stock empty-Compute G15 RegisterArray and its `0x1a440` mirror. Patch 0006 is the focused E071 delta from `1d264651` to `f73b9e55`, correcting only the exact stock empty-Compute UMA prepared byte at RunCompute `+0x846` from `0` to `1`. Patch 0007 is the E073 delta from `f73b9e55` to `03fdbb86`, wiring the exact managed context-generation byte to RunCompute `+0x85f`. Patch 0008 is the E074 one-shot read-only range-8 parent-ownership diagnostic from `03fdbb86` to `4e26fc08`. Patch 0009 is the E075 range-8 leaf-PTE proof from `4e26fc08` to `e9f50fcc`. Patch 0010 is the E076 compile-only allocator split from `e9f50fcc` to `6e3850dc`. Patch 0011 is the E078 compile-only FList range-7 state delta from `6e3850dc` to `886820e1`, adding the exact `0x...044b` class and 8-byte FW-Uncached ABI without instantiating persistent FList resources. Patch 0012 is the E079 compile-only HardwareBuffer-ID state delta from `886820e1` to `865f24f2`, modeling Apple's exact 256-ID sticky/refcount/free-state behavior without a runtime manager. Patch 0013 is the E080 compile-only ownership wrapper from `865f24f2` to `ed17ac03`, adding kernel synchronization and an FList-side sticky binding while still creating no runtime instance. Patch 0014 is the E081 compile-only FList resource plan from `ed17ac03` to `affdd1fb`, adding exact symbolic geometry and fixed-object assertions without any GPU allocator. Patch 0015 is the E082 compile-only range-5 list-class delta from `affdd1fb` to `724674ad`, pinning exact compact `0x300` under a dedicated FList semantic protection constant without allocating GPU memory. Runtime G15 submission remains fail-closed.

## Explicitly not enabled

- no production G15 render path claim
- no general command-buffer submission
- no RunVertex/RunFragment/RunCompute or shader payload enabled by the registration-only diagnostic
- no direct experimental PMGR writes
- no upstream AsahiLinux push implied by this public checkpoint

The project remains a staged clean-room bring-up with one-shot live gates and Golden-kernel recovery.
