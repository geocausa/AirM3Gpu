# Curated G15 Research Notes

This directory contains original, curated reverse-engineering summaries and clean-room generators used by the T8122/G15 bring-up.

Raw Apple binaries, kernelcaches, firmware extracts, raw decompiler output, and Ghidra project databases are deliberately excluded. Symbol names, offsets, derived constants, and independently reconstructed layouts are recorded where needed to make the source work reproducible.

Current high-level state: `../../docs/CURRENT-STATE.md`.

Key current boundary notes:

- `G15-MAPPED-GLOBAL-FIRMWARE-RESOURCE-BACKINGS.md` — E150 zero-caller real range-7/range-8 backing factory plus selected-FWVA derivation bound to E149 dynamic global slots; runtime constructor still host-only.
- `G15-FIRMWARE-RESOURCE-MODE0-LIFETIME.md` — E149 exact mode-0 backing use-count seed, last-release destruction, backing-hole and delayed-regrowth semantics; host-only bookkeeping correction, no selected FWVA or RunCompute.
- `G15-GLOBAL-FIRMWARE-RESOURCE-LEASES.md` — E148 device-global host selector/use-count bookkeeping plus queue/channel RAII global-index leases; no global backing FWVA or RunCompute.
- `G15-GLOBAL-FIRMWARE-RESOURCE-STACKS.md` — E147 exact device-global firmware resource-stack placement, eager backing and dynamic global selection/release; proves the five E127 local indices cannot become live constants.
- `G15-NORMAL-COMPUTE-POOL-CLASS.md` — E146 exact normal-J615 class-1 shared Compute pool selection; dormant assembly no longer accepts an arbitrary pool class.
- `G15-DORMANT-COMPUTE-CHANNEL-ASSEMBLY.md` — E145 type-connected device/client/shared-pool and channel-local owner tiers behind one zero-caller unpublished bundle; no live pool creation or RunCompute.
- `G15-LAZY-COMPUTE-CHANNEL-CONTEXT.md` — E144 private Queue retention of per-client range-5 cached/uncached plus bank1/q22 handles required by future lazy pool/channel construction; no constructor or RunCompute.
- `G15-CLIENT-UMA-QUEUE-CONTEXT.md` — E143 exact command-queue-to-AGXShared container provenance plus host-only per-VM container Arc retained in G15 QueueInner; no pool selection/construction or RunCompute.
- `G15-SHARED-UMAPOOL-SELECTION-TRANSACTION.md` — E142 exact container-lock span across failed promotion, pool construction and slot publication plus compile-only logically weak actual-pool coupling; no Queue caller or RunCompute.
- `G15-WEAK-UMAPOOL-PROMOTION.md` — E141 safe logical nonzero-only weak-slot promotion/direct+active lifetime model with identity-checked final clearing; no pool owner coupling or live Queue consumer.
- `G15-CLIENT-UMA-WEAK-SLOT-PLACEMENT.md` — E140 exact AGXShared container destruction scope plus compile-only four-slot non-owning state at Linux client-VM lifetime; no pool promotion or RunCompute consumer.
- `G15-FLIST-CLIENT-ADDRESS-SPACE.md` — E139 exact AGXShared/client IOGPUTask provenance for both persistent range-5 FList lists and per-client shared-pool-container scope; Linux per-VM range-5 allocator confirmed correct, no Linux delta.
- `G15-DEVICE-GLOBAL-UMA-HOST-STATE.md` — E138 exact lazy TA/3D/CL channel/pool construction and G15 device-global zero-seeded UMA host-state placement; no pool or RunCompute consumer.
- `G15-SHARED-UMAPOOL-LIFECYCLE.md` — E137 exact weak-slot try-retain/replacement, channel-direct vs active-epoch references, pointer-identity finalization, and pool-owned container lifetime; no Linux delta.
- `G15-SHARED-COMPUTE-UMA-OWNER-SPLIT.md` — E136 definition-only device-global UMA state + reusable shared Compute pool/FList lifetime; dormant channel/command owner no longer contains the FList, with no live Queue/RunCompute path.
- `G15-SHARED-COMPUTE-UMA-PLACEMENT.md` — E135 exact accelerator-global UMAPool HardwareBuffer-ID namespace, shared/reusable Compute-pool selection, global-order pool ID, and the resulting block on naïve per-Queue owner-graph placement.
- `G15-TWO-PHASE-FENCE-ARM.md` — E134 private RAII JobFence command arm carried across dormant two-phase prepare/finalize; failure/abort/completion pending-count lifetime closed without a live RunCompute writer.
- `G15-STOCK-EMPTY-PRE-MICRO-RAW-SOURCES.md` — E133 exact correction of pre-micro `+0x740/+0x748/+0x750` to raw Compute `+0xc0/+0xd8`; stock-empty zeros proven exact, integration still dormant.
- `G15-STOCK-EMPTY-JOBPARAMS2-23J220.md` — E132 exact same-build 23J220 stock-empty JobParameters2/Compute-JobMeta defaults; E133 subsequently corrects and closes the then-open pre-micro source.
- `G15-STOCK-EMPTY-RUNCOMPUTE-SOURCES.md` — E131 exact command-local SKU-source bridge from the typed RunCompute image; target scalar finalize-input bag removed, runtime writer still absent.
- `G15-STOCK-EMPTY-COMPUTE-STATS-OWNER.md` — E130 exact `AGXFirmware +0x228/+0x268` 0xe10 Compute-statistics ownership/zero-init closure; raw target finalize FWVA removed, generic serializer still parameterized.
- `G15-STOCK-EMPTY-GART-SOFT-FAULT.md` — E129 exact ordinary type-5 IOGPU chain proves normal-J615 `gart_soft_fault_enabled=false`; target finalize input removed, generic serializer still parameterized.
- `G15-STOCK-EMPTY-FIXED-SKU-INPUTS.md` — E128 exact first-CL `evctl_index=0` and normal-J615 accelerator packed-feature bit 39 = 0; dormant finalize inputs narrowed, live RunCompute unchanged.
- `G15-CHANNEL-OWNER-GRAPH.md` — E127 coherent unreachable timestamp/scheduler/channel-state/cached/uncached owner integration with independent indices; external SKU `channel_state_fwva` removed.
- `G15-SCHEDULER-STATE-BACKING.md` — E126 exact `_AGFISchedulerState` range-8 0x4000/0x40 backing, QueueInfo `+0xa4` provenance and private selected reset; live generic GpuContext unchanged.
- `G15-TIMESTAMP-QUEUE-STATE.md` — E125 exact QueueInfo `+0x10` timestamp-state provenance, range-7 0x4000/0x18 backing and private reset model; live notifier path unchanged.
- `G15-UNCACHED-CHANNEL-RESET.md` — E124 exact normal-J615 selected uncached channel-memory six-word reset header; private local-slot token only, no FWVA.
- `G15-CHANNEL-MEMORY-BACKING.md` — E122 exact cached/uncached J615 channel-memory resource-stack geometry and E123 independent unreachable range-7/range-8 block owners; no selected FWVA or live channel integration.
- `G15-CHANNEL-STATE-GPU-BUFFER.md` — E121 exact QueueInfo `+0x18 = selected-state +0xb0` source; compile-only private derivation, live WorkQueue unchanged.
- `G15-SELECTED-CHANNEL-STATE.md` — E118-E120 exact selected `_AGFIChannelState` reset/priority model; private prepared-state token only, no SKU/RunCompute conversion.
- `G15-CL-CONSTRUCTOR-PRIORITY.md` — E119 exact CL constructor/priority source chain; first `evctl_index=0`, J615 second integer `0x50`, runtime effective priority/QoS remain explicit.
- `G15-CHANNEL-STATE-RESET-PRIORITY.md` — E118 exact selected `_AGFIChannelState` reset/priority ABI; normal CL constructor/priority inputs remain gated.
- `G15-CHANNEL-STATE-BACKING.md` — E116 exact 0x24c0/0x8000 special-range-8 channel-state backing and E117 unreachable compile-only block owner; selected FWVA remains gated.
- `G15-CL-COMMAND-RESOURCE.md` — E114 exact J615 CL command-resource geometry/source audit and E115 unreachable range-5 owner; no RunCompute publication.
- `G15-TWO-PHASE-COMPUTE-ASSETS.md` — E113 definition-only two-phase reservation/finalization fixing the command/SKU address dependency without a RunCompute writer.
- `G15-RUNCOMPUTE-INTEGRATION-GAPS.md` — E112 static audit blocking direct E111 → RunCompute mutation and defining the required two-phase integration boundary.
- `G15-RUNCOMPUTE-FIELD-STAGE.md` — E111 host-only staging of exact stock-empty RunCompute-facing asset fields with no firmware-command writer.
- `G15-GUARDED-COMMAND-ASSETS.md` — E110 definition-only armed-fence integration of exact event/SKU retirement guards with the private E109 asset materializer.
- `G15-UNPUBLISHED-COMMAND-ASSETS.md` — E109 private non-Copy coherent event/SKU/Page-Pool/HWMetrics/HardwareBuffer assets token with no RunCompute consumer.
- `G15-STOCK-EMPTY-OWNER-GRAPH.md` — E108 unreachable construction graph for event-control, HWMetrics, FList/HardwareBuffer and SKU backing owners; no FWVA/RunCompute consumer.
- `G15-SKU-PREPARED-SLOT.md` — E107 unreachable prepared-slot copy/FWVA token after exact E106 retirement selection; RunCompute `+0x760` remains disconnected.
- `G15-SKU-SLOT-RETIREMENT.md` — E105 exact SKU host-event slot reuse/selected-FWVA lifetime and E106 conservative unreachable JobFence guard.
- `G15-SKU-BACKING-OWNER.md` — E103 exact 0xf0 × 0x300 / 0x30000 range-8 SKU backing lifetime and E104 unreachable compile-only owner; no selected FWVA.
- `G15-STOCK-EMPTY-SKU-SERIALIZER.md` — E101 exact stock-empty 23J220 SKU source map and E102 unreachable byte-exact `0x2c0` serializer; RunCompute `+0x760` remains unwired.
- `G15-STOCK-EMPTY-FLIST-FIRST-ACTIVATION.md` — E100 exact stock-empty zero-page/zero-Backup-extent first activation and unreachable compile-only helper; RunCompute `+0x83e` remains zero.
- `G15-FLIST-FIRMWARE-STATE-POPULATION.md` — E099 exact compile-only post-`populatePagePool()` 0x70 state population; Page/Backup List contents and RunCompute `+0x83e` remain gated.
- `G15-23J220-HWMETRICS.md` — E097 exact one-page/0x40-record HWMetrics mapping class and E098 unreachable compile-only owner; RunCompute `+0x857` remains zero.
- `G15-EVENT-SLOT-RETIREMENT-GUARD.md` — E096 compile-only JobFence-backed 36-slot reuse guard and corrected `+0x08` shared-state-sequence semantic.
- `G15-EVENT-SLOT-FINISH-REUSE.md` — E095 exact host-event finish-before-reuse lifetime and `+0x08` semantic correction.
- `G15-EVENT-CONTROL-SELECTED-STATE.md` — E094 exact normal-J615 selected-state `+0x10=80` seed and still-gated event-slot finish/reuse boundary.
- `G15-EVENT-CONTROL-BACKING-OWNER.md` — E093 unreachable compile-only owner for the exact range-7 stamp and range-8 event-control shared backings.
- `G15-EVENT-CONTROL-BACKING.md` — E092 exact 0x90 range-7 stamp backing, 0x1b00 range-8 event-control backing, construction linkage, and still-gated rotation config.
- `G15-EVENT-CONTROL-POOL-GEOMETRY.md` — E091 exact 36-state × 0xc0 rotating event-control ownership geometry, compile-only with no allocator/mapping class chosen.
- `G15-COMPUTE-ERROR-RECOVERY.md` — E090 exact recovery ingress, destructive forced-stamp convergence, and separate context/FList/request/channel reset ownership.
- `G15-COMPUTE-EVENT-CONTROL.md` — E089 exact G15 RunCompute `+0x14` event-control semantics and fail-closed removal of the legacy queue-wide notifier.
- `G15-COMPUTE-COMPLETION-STAMPS.md` — E088 exact RunCompute event-control FWVA and normal stamp→scheduler→descriptor-retirement chain.
- `G15-FLIST-PREPARE-COMPLETE.md` — E087 exact FList HardwareBuffer prepare/complete residency lifetime and separate PTE teardown boundary.
- `G15-FLIST-PERSISTENT-OWNER.md` — E086 unreachable compile-only owner for all four exact persistent FList backings.
- `G15-Q22-RANGE8-SPECIAL.md` — E085 exact range-8 q22 special-aperture flags (`3/2`) with range-7 `1/0` preserved, compile-only.
- `G15-PUBLIC-M1N1-SEMANTIC-CROSSCHECK.md` — E084 current-public m1n1 concept cross-check; semantic clues only, no old G15 offsets imported.
- `G15-J615-UMA-POOL-GEOMETRY.md` — E083 exact J615 M/B defaults and resolved 4-MiB/32-KiB FList list geometry, compile-only.
- `G15-FLIST-RANGE5-LIST-PTE.md` — E082 exact FList Page/Backup List range-5 compact `0x300` class, corrected caller option, and compile-only semantic PTE constant.
- `G15-FLIST-RESOURCE-PLAN.md` — E081 side-effect-free FList resource geometry/HardwareBuffer ownership plan with no GPU allocator.
- `G15-HARDWARE-BUFFER-ID-OWNER.md` — E080 synchronized manager wrapper plus FList sticky owner, compile-only and uninstantiated.
- `G15-HARDWARE-BUFFER-ID-STATE.md` — E079 exact 0x100-entry sticky/refcount/bitmap/free-stack HardwareBuffer-ID state machine, compile-only and uninstantiated.
- `G15-23J220-FLIST-RESOURCE-LIFETIME.md` — E077/E078 exact FList four-resource constructor, symbolic list-size formulas, distinct `0x...044b` FW-Uncached range-7 leaf class, and compile-only Linux mapping ownership.
- `G15-RANGE8-DEDICATED-ALLOCATOR.md` — E076 compile-only split of range-7/range-8 bank-1 VA ownership and hard-wired range-8 protection class.
- `G15-23J220-RANGE8-LEAF-PTE.md` — E075 exact range-8 Page-Pool-State leaf class, one-shot PTE PASS, and clean eight-parent teardown.
- `G15-23J220-RANGE8-PAGE-POOL-STATE.md` — E074 exact-target range-8 Page-Pool-State aperture plus one-shot shared-parent ownership PASS and the remaining leaf/PTE boundary.
- `G15-23J220-CONTEXT-ID-GENERATION.md` — E073 exact-target 64-ID context/generation lifecycle, 256-ID HardwareBuffer lifetime, and Linux `+0x85f` implementation.
- `G15-23J220-COMPUTE-UMA-TAIL.md` — E072 exact-target stock-empty UMA value closure plus Page-Pool State and HWMetrics pointer producers.
- `G15-23J220-COMPUTE-SKU-SOURCES.md` — E070/E071 exact-target SKU packet/source map, inactive PerfCtr closure, UMA prepared-state handoff, timestamp geometry, and the Linux +0x760/+0x846 corrections.
- `G15-EMPTY-COMPUTE-CONTAINER.md` — E069 exact-23J220 proof that the stock empty Compute descriptor reaches normal `submitBuffer()`/firmware submission rather than being host-elided.
- `G15-EMPTY-COMPUTE-REGISTERARRAY.md` — E068 stock empty-Compute raw oracle, final descriptor-source closure, and exact 20-entry J615/G15G list.
- `G15-23J220-COMPUTE-REGISTERARRAY.md` — E067 exact-target G15 Compute RegisterArray order/source/synthesis validation and the remaining Linux-producer boundary.
- `G15-23J220-COMPUTE-ABI.md` — E066 exact 23J220 cross-check of the 0x880 Compute/CLE command, 0x18 accelerator-ring entry, Compute pipe ID 2, and ReleaseResource opcode 0x11.
- `G15-COMPUTE-SKU-STREAM.md` — E062 exact Compute SKU packet grammar, fixed WFI dword, timestamp record geometry, aligned stream sizes, and J615 dynamic register-ID closure.
- `G15-COMPUTE-CONTROL-STREAM.md` — E063 macOS-oracle proof of the `0x1a420` raw CDM stream edge and exact Gen4 patch/reset token/address-record grammar.
- `G15-COMPUTE-LAUNCH-BOUNDARY.md` — E061 proof that normal type-3 RunCompute is inherently hardware-facing; exact RTKit stream/UMA handoff and remaining first-command prerequisites.
- `G15-QUEUE-REGISTRATION-LIFECYCLE.md` — E056-E060 DPE correction, scheduler acceptance, stamp-state binding, pipe retirement, and native G15 ReleaseResource closure.
- `G15-PIPE-SUBMISSION-BOUNDARY.md` — historical transport/TX/doorbell proof chain that led to the now-closed registration boundary.

Earlier startup notes remain useful as the proof chain for the now-closed InitData/RTKit/`MSG_INIT` stages.
