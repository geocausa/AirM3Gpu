# Patch Bases

The patch files in this repository are reproducible exports from separate component worktrees.

## Linux / Asahi

Base commit:

`d8c844ca7c34abb01b93851cb405fb42b2650f0a`

Previous public checkpoint head:

`1b57b289af96973badfbb8489ef379a1b3a96f07`

Current clean checkpoint head:

`a244a846e3014b9eebdec580f852e39d27a1a50d`

Local checkpoint tags:

- `airm3gpu-2026-08-24-rtkit-management` — management handshake boundary
- `airm3gpu-2026-08-24-rtkit-crashlog-prealloc` — EP1 physical preallocation PASS
- `airm3gpu-2026-08-24-rtkit-app-endpoints` — EP20/EP21 start PASS
- `airm3gpu-2026-08-26-pipe-boundary` — clean source checkpoint through bounded empty QueueInfo publication

Series:

1. `patches/linux/0001-drm-asahi-checkpoint-T8122-G15-bring-up-through-RTKi.patch`
2. `patches/linux/0002-drm-asahi-map-G15-RTKit-preallocated-crashlog-memory.patch`
3. `patches/linux/0003-drm-asahi-stage-G15-RTKit-app-endpoint-start.patch`
4. `patches/linux/0004-drm-asahi-checkpoint-G15-through-empty-queue-boundary.patch`
5. `patches/linux/0005-drm-asahi-model-G15-empty-Compute-register-list.patch`
6. `patches/linux/0006-drm-asahi-mark-G15-empty-Compute-UMA-prepared.patch`
7. `patches/linux/0007-drm-asahi-track-G15-context-ID-generation.patch`
8. `patches/linux/0008-drm-asahi-add-G15-range-8-parent-preflight.patch`
9. `patches/linux/0009-drm-asahi-validate-G15-range-8-leaf-PTE.patch`
10. `patches/linux/0010-drm-asahi-split-G15-bank-1-range-allocators.patch`
11. `patches/linux/0011-drm-asahi-model-G15-FList-range-7-state.patch`
12. `patches/linux/0012-drm-asahi-model-G15-HardwareBuffer-ID-state.patch`
13. `patches/linux/0013-drm-asahi-wrap-G15-HardwareBuffer-ID-ownership.patch`
14. `patches/linux/0014-drm-asahi-plan-G15-FList-persistent-resources.patch`
15. `patches/linux/0015-drm-asahi-model-G15-FList-range-5-list-class.patch`
16. `patches/linux/0016-drm-asahi-pin-J615-G15-UMA-pool-geometry.patch`
17. `patches/linux/0017-drm-asahi-encode-G15-range-8-mapping-notifications.patch`
18. `patches/linux/0018-drm-asahi-own-G15-FList-persistent-backings.patch`
19. `patches/linux/0019-drm-asahi-fail-closed-G15-Compute-event-control.patch`
20. `patches/linux/0020-drm-asahi-model-G15-event-control-pool-geometry.patch`
21. `patches/linux/0021-drm-asahi-own-G15-event-control-shared-backings.patch`
22. `patches/linux/0022-drm-asahi-seed-G15-event-control-selected-state.patch`
23. `patches/linux/0023-drm-asahi-guard-G15-event-control-slot-reuse.patch`
24. `patches/linux/0024-drm-asahi-own-G15-UMA-HWMetrics-backing.patch`
25. `patches/linux/0025-drm-asahi-model-G15-FList-firmware-state-population.patch`
26. `patches/linux/0026-drm-asahi-model-stock-empty-G15-FList-activation.patch`
27. `patches/linux/0027-drm-asahi-serialize-stock-empty-G15-Compute-SKU-stream.patch`
28. `patches/linux/0028-drm-asahi-own-G15-Compute-SKU-backing.patch`
29. `patches/linux/0029-drm-asahi-guard-G15-Compute-SKU-slot-reuse.patch`
30. `patches/linux/0030-drm-asahi-prepare-G15-Compute-SKU-slot-payload.patch`
31. `patches/linux/0031-drm-asahi-group-G15-stock-empty-Compute-owners.patch`
32. `patches/linux/0032-drm-asahi-materialize-unpublished-G15-Compute-assets.patch`
33. `patches/linux/0033-drm-asahi-bind-G15-Compute-asset-retirement-guards.patch`
34. `patches/linux/0034-drm-asahi-stage-G15-RunCompute-asset-fields.patch`
35. `patches/linux/0035-drm-asahi-split-G15-Compute-asset-preparation-phases.patch`
36. `patches/linux/0036-drm-asahi-own-G15-CL-command-resource-backing.patch`
37. `patches/linux/0037-drm-asahi-own-G15-channel-state-backing-geometry.patch`
38. `patches/linux/0038-drm-asahi-model-G15-selected-channel-state.patch`
39. `patches/linux/0039-drm-asahi-derive-G15-channel-state-gpu-buffer.patch`
40. `patches/linux/0040-drm-asahi-own-G15-channel-memory-backing-geometry.patch`
41. `patches/linux/0041-drm-asahi-model-G15-uncached-channel-reset-header.patch`
42. `patches/linux/0042-drm-asahi-model-G15-timestamp-queue-state.patch`
43. `patches/linux/0043-drm-asahi-model-G15-scheduler-state-backing.patch`
44. `patches/linux/0044-drm-asahi-group-G15-channel-lifetime-owners.patch`
45. `patches/linux/0045-drm-asahi-fix-J615-stock-empty-SKU-inputs.patch`
46. `patches/linux/0046-drm-asahi-close-J615-GART-soft-fault-input.patch`
47. `patches/linux/0047-drm-asahi-own-J615-compute-statistics-SKU-input.patch`
48. `patches/linux/0048-drm-asahi-source-J615-SKU-inputs-from-RunCompute.patch`
49. `patches/linux/0049-drm-asahi-correct-J615-pre-micro-Compute-sources.patch`
50. `patches/linux/0050-drm-asahi-arm-J615-two-phase-fence-transaction.patch`

Patch 0004 is intentionally a squashed checkpoint delta from `1b57b289af96` to `2f08f68bb2ef`. Patch 0005 is the focused compile-only delta from `2f08f68bb2ef` to `1d264651a204`, adding the exact E068 stock empty-Compute G15 RegisterArray while leaving ordinary G15 submission fail-closed. Patch 0006 is the focused E071 delta from `1d264651a204` to `f73b9e551658`, changing only RunCompute `+0x846` to the exact prepared-state value `1`. Patch 0007 is the focused E073 delta from `f73b9e551658` to `03fdbb86230f`, adding the exact managed G15 context-generation lifetime and exporting it at RunCompute `+0x85f`. Patch 0008 is the focused E074 diagnostic delta from `03fdbb86230f` to `4e26fc089860`: it performs only a read-only range-8 shared-parent ownership check and then fails closed before persistent G15 runtime. Patch 0009 is the focused E075 delta from `4e26fc089860` to `e9f50fcc17d5`: it live-validates the exact range-8 leaf class and reversible eight-parent bank-1 teardown. Patch 0010 is the focused E076 compile-only delta from `e9f50fcc17d5` to `6e3850dcdd51`: it splits bank-1 VA allocation into disjoint range-7/range-8 arenas and hard-wires the range-8 constructor. Patch 0011 is the focused E078 compile-only delta from `6e3850dcdd51` to `886820e1f460`: it adds the exact FList FW-Uncached range-7 leaf class, fixed 8-byte ABI and hard aperture/protection pairing without creating a persistent FList object. Patch 0012 is the focused E079 compile-only delta from `886820e1f460` to `865f24f2a9fc`: it reconstructs the 256-ID HardwareBuffer manager state machine but deliberately leaves synchronization/runtime ownership unwired. Patch 0013 is the focused E080 compile-only delta from `865f24f2a9fc` to `ed17ac035ad2`: it adds the synchronized manager wrapper and FList sticky owner without instantiating either at runtime. Patch 0014 is the focused E081 compile-only delta from `ed17ac035ad2` to `affdd1fba79d`: it adds exact symbolic FList geometry and a no-allocation resource plan. Patch 0015 is the focused E082 compile-only delta from `affdd1fba79d` to `724674ad034e`: it pins the exact FList Page/Backup List compact-`0x300` range-5 class under a dedicated semantic protection constant while preserving the no-allocation resource plan. Patch 0016 is the focused E083 compile-only delta from `724674ad034e` to `b88369c26ffe`: it pins exact J615 M/B/P defaults and resolves the checked 4-MiB/32-KiB list geometry without creating GPU resources. Patch 0017 is the focused E085 compile-only delta from `b88369c26ffe` to `9b21157497bb`, fixing q22 range-8 special-aperture map/unmap flags to exact `3/2` while preserving range-7 `1/0`. Patch 0018 is the focused E086 compile-only delta from `9b21157497bb` to `3cd3f336d9f4`, owning the exact four FList persistent backings but leaving prepare/populate and command publication unreachable. Patch 0019 is the focused E089 compile-only delta from `3cd3f336d9f4` to `167c037a91a0`, replacing the inherited G15 queue-wide notifier export at RunCompute `+0x14` with an exact event-control FWVA field that remains zero/fail-closed until the 36-state owner is reconstructed. Patch 0020 is the focused E091 compile-only geometry delta from `167c037a91a0` to `97bd7129c3f0`. Patch 0021 is the focused E093 compile-only backing-owner delta from `97bd7129c3f0` to `ddcbd85be239`. Patch 0022 is the focused E094 compile-only selected-state delta from `ddcbd85be239` to `42bca8d2e9dc`, pinning the exact J615 effective-record-count value and post-finish reset/seed boundary while leaving selected FWVA publication unreachable. Patch 0023 is the focused E096 compile-only slot-reuse delta from `42bca8d2e9dc` to `4ff63937d4fc`, correcting event-control `+0x08` and retaining a submission fence until each rotated slot is retired. Patch 0024 is the focused E098 compile-only HWMetrics delta from `4ff63937d4fc` to `d18178f018ac`, adding the exact compact-`0x30b`/leaf-`0x...040b` range-7 HWMetrics class and one unreachable page-base owner while keeping RunCompute `+0x857` zero. Patch 0025 is the focused E099 compile-only FList-state delta from `d18178f018ac` to `47a8c0070a4b`, validating and populating the exact post-page-list 0x70 firmware image while keeping page-list contents/chained mappings and command FWVA unreachable. Patch 0026 is the focused E100 compile-only stock-empty activation delta from `47a8c0070a4b` to `bd95b9c01004`, encoding the exact zero-page/zero-Backup-extent stock-empty result behind a zero-list guard while keeping command publication unreachable. Patch 0027 is the focused E102 compile-only serializer delta from `bd95b9c01004` to `897d3ae41896`, encoding the exact inactive stock-empty 23J220 `0x2c0` SKU stream from explicit runtime-owned sources while leaving GPU backing and RunCompute `+0x760` integration absent. Patch 0028 is the focused E104 compile-only backing-owner delta from `897d3ae41896` to `d54dfc78d059`, owning the exact page-base 0x30000 range-8 SKU backing with 0xf0 × 0x300 geometry while exposing no selected slot/FWVA and leaving E102 disconnected. Patch 0029 is the focused E106 compile-only retirement-guard delta from `d54dfc78d059` to `69af01482ad8`, preserving Apple's exact next-slot modulo scan while using a conservative whole-submission `JobFence` completion predicate and leaving GPU-slot writes/RunCompute publication absent. Patch 0030 is the focused E107 compile-only prepared-slot delta from `69af01482ad8` to `f3464e7ec216`, clearing/copying one exact 0x300 slot and returning an unreachable prepared FWVA token while keeping RunCompute `+0x760` disconnected. Patch 0031 is the focused E108 compile-only owner-graph delta from `f3464e7ec216` to `ea707e8e7726`, grouping event-control, HWMetrics, FList/HardwareBuffer and SKU backing ownership under one unreachable construction graph with no FWVA publication or RunCompute consumer. Patch 0032 is the focused E109 compile-only unpublished-assets delta from `ea707e8e7726` to `a783f835b437`, materializing event/SKU/Page-Pool/HWMetrics addresses and HardwareBuffer ownership only into a private non-Copy token while keeping retirement guards and RunCompute publication separate. Patch 0033 is the focused E110 compile-only guard-integration delta from `a783f835b437` to `63bb6ffe4f22`, enforcing next-event/no-skip and next-free-SKU fence binding/rollback before E109 materialization while adding no Queue/submission call site. Patch 0034 is the focused E111 compile-only field-stage delta from `63bb6ffe4f22` to `139b745dbf00`, mapping the guarded private assets onto exact stock-empty RunCompute-facing offsets in a host-only staging record while exposing no firmware-command writer. Patch 0035 is the focused E113 compile-only two-phase delta from `139b745dbf00` to `b21300a1ad4d`, reserving the selected SKU address without writing bytes and deferring E102 serialization until a future command FWVA and explicit unresolved runtime inputs are supplied; it adds no RunCompute writer or Queue call site. Patch 0036 is the focused E115 compile-only CL command-resource delta from `b21300a1ad4d` to `059d34701e48`, adding the exact J615 `0x1f400` option-3 range-5 owner to the unreachable stock-empty graph and deriving `channel_command_region_base_fwva` from it instead of an external finalize input. Patch 0037 is the focused E117 compile-only channel-state backing delta from `059d34701e48` to `252ded3b6353`, pinning the exact 0x24c0 element / 0x8000 three-slot special-range-8 block geometry and owning one unreachable block without selected-slot FWVA publication. Patch 0038 models one private selected channel-state reset/priority image, patch 0039 derives its in-slot QueueInfo `+0x18` gpu-buffer pointer, and patch 0040 owns the exact E122 J615 cached/uncached channel-memory backing geometry without exposing selected addresses. Patch 0043 is the focused E126 compile-only scheduler-state delta from `adfbfca2e265` to `293ca6338017`, adding an exact 0x4000/0x40 range-8 scheduler backing and private selected-state reset while leaving live generic GpuContext/RunCompute ownership unchanged. Patch 0044 is the focused E127 compile-only channel-owner integration delta from `293ca6338017` to `8f6bd394ceb5`, grouping exact timestamp/scheduler/channel-state/cached/uncached owners under the dormant graph and removing the external SKU `channel_state_fwva` source while keeping all live producers unchanged. Patch 0050 is the focused E134 compile-only fence-arm delta from `f09bed89530e` to `a244a846e301`, carrying one RAII `JobFence` command reference across the dormant two-phase prepare/finalize transaction while leaving the live compute path and RunCompute bytes unchanged. The local development history between those revisions contains many staged bring-up commits; publishing the validated end-state delta keeps this repository reproducible without presenting every one-shot experiment as an upstream-ready commit series.

Validation performed on 2026-08-26:

- `git apply --check` against `1b57b289af96`: PASS
- temporary-index application tree: `e985490062efc4baf5847b6e73cd7cf799a5ffaa`
- expected `2f08f68bb2ef^{tree}`: `e985490062efc4baf5847b6e73cd7cf799a5ffaa`
- exact tree match: PASS

Patch 0005 validation performed on 2026-08-28:

- `git apply --cached --check` against `2f08f68bb2ef`: PASS
- temporary-index application tree: `b913cff9035c398aa04c1a0bb737d9cdfc139e58`
- expected `1d264651a204^{tree}`: `b913cff9035c398aa04c1a0bb737d9cdfc139e58`
- exact tree match: PASS

Patch 0006 validation performed on 2026-08-28:

- `git apply --check` against `1d264651a204`: PASS
- temporary-index application tree: `a833d07fe3bbcd6103090c8cbb9dfa378614521e`
- expected `f73b9e551658^{tree}`: `a833d07fe3bbcd6103090c8cbb9dfa378614521e`
- exact tree match: PASS


Patch 0007 validation performed on 2026-08-28:

- `git apply --check` against `f73b9e551658`: PASS
- temporary-index application tree: `999f7f6cc80a2e5cfd92c8de88dbc1a05d6c3de3`
- expected `03fdbb86230f^{tree}`: `999f7f6cc80a2e5cfd92c8de88dbc1a05d6c3de3`
- exact tree match: PASS
- strict checkpatch: 0 errors, 0 warnings, 0 checks
- Asahi module build: PASS; no install or RunCompute execution

Patch 0008 validation performed on 2026-08-28:

- `git apply --check` against `03fdbb86230f`: PASS
- temporary-index application tree: `4a844f6c6a6077b9658a4d3e3b3c16cd6c89ba42`
- expected `4e26fc089860^{tree}`: `4a844f6c6a6077b9658a4d3e3b3c16cd6c89ba42`
- exact tree match: PASS
- strict checkpatch: PASS
- Asahi module build: PASS at the existing 24-warning bring-up baseline
- one-shot live diagnostic: PASS; read-only shared-L2[6..8) ownership check only, then fail-closed `ENODEV`; no RunCompute


Patch 0009 validation performed on 2026-08-28:

- base: `4e26fc0898606f09b9bf726ebba2c5452ee957f2`
- expected commit tree: `0bb806b6dc8361763144fccba2fd3dcf37b323f7`
- patch SHA-256: `0280f3bd00932887698d9ebb24a18b10c34f0101622f20b8296fbe473559321c`
- strict checkpatch: 0 errors, 0 warnings, 0 checks
- Asahi module build: PASS at the established 24-warning bring-up baseline
- one-shot live diagnostic: exact range-8 leaf `0x00c0000000000443` PASS, clean leaf clear and eight-parent detach, then fail-closed `ENODEV`; no RunCompute


Patch 0010 validation performed on 2026-08-28:

- base: `e9f50fcc17d58244740360e484ae9904c0cd8d6c`
- expected commit tree: `aa7d564fd4cfb6efd6ca4674df74d64663c6f13b`
- patch SHA-256: `0288180a8a951ad3705fda84654bf2bb8df2c6ffb2931d61be0103b3c46cf439`
- strict checkpatch: 0 errors, 0 warnings, 0 checks
- Asahi module build: PASS at the exact existing 24-warning bring-up baseline
- runtime: not installed/not executed; compile-only allocator topology


Patch 0011 validation performed on 2026-08-29:

- base: `6e3850dcdd51d4bd912b9c02b0ea9633c7fd7060`
- expected commit tree: `0e95ec2eacd9151962a73a951efdc6e728b9c810`
- patch SHA-256: `0b3b8c77fb440da5e152d60e3a1330fee92eb7c5f3bfd802940de139fec1feb2`
- strict checkpatch: 0 errors, 0 warnings, 0 checks
- Asahi module build: PASS at the exact 24-warning bring-up baseline
- runtime: not installed/not executed; fixed FList mapping-class scaffolding only


Patch 0012 validation performed on 2026-08-29:

- base: `886820e1f460d4af0e6bbd4d33812d89d5adddd1`
- expected commit tree: `f88bf712fb898afb3f5a436acb62f18eadf3dc87`
- patch SHA-256: `28d46d3d827587f174600c3c7055191a63bbe1758a2add25532d1aa479f9b7e9`
- strict checkpatch: 0 errors, 0 warnings, 0 checks
- Asahi module build: PASS at the exact 24-warning bring-up baseline
- runtime: not installed/not executed; manager state only, no lock or FList instance


Patch 0013 validation performed on 2026-08-29:

- base: `865f24f2a9fc2be36db3ffae11eee22aa0ebc63c`
- expected commit tree: `0ab1640083173ac0070b3717bce4e8fbbee85ec3`
- patch SHA-256: `a99aa363f523ea201287efa5eabcb3b3df3e0a69eb19434c2d34db73cd5ca473`
- strict checkpatch: 0 errors, 0 warnings, 0 checks
- Asahi module build: PASS at the exact 24-warning bring-up baseline
- runtime: not installed/not executed; synchronized wrapper remains uninstantiated


Patch 0014 validation performed on 2026-08-29:

- base: `ed17ac035ad2422d4a85146e97c88cb3057eb174`
- expected commit tree: `2626c74fdd6b91a09e51a24b187e7510760dd3b3`
- patch SHA-256: `b20da8f6d4b4df1d6e58dd5e2eb90449a4da830f17acfa4837f4d78bd4bd9a33`
- strict checkpatch: 0 errors, 0 warnings, 0 checks
- Asahi module build: PASS at the exact 24-warning bring-up baseline
- runtime: not installed/not executed; resource plan contains no GPU allocator


Patch 0015 validation performed on 2026-08-29:

- base: `affdd1fba79d333530cac22d069212b6e65ef9d4`
- expected commit tree: `8d1327b8eb514abd506e751f578bab40ebe937c1`
- patch SHA-256: `cf5fa4401ec4e31c68df27f62bc470d680f7bcb76255b66da61c36b4be5a61d6`
- strict checkpatch: 0 errors, 0 warnings, 0 checks
- Asahi module build: PASS at the exact existing 24-warning bring-up baseline
- runtime: not installed/not executed; semantic PTE class only, no GPU allocation or RunCompute


Patch 0016 validation performed on 2026-08-29:

- base: `724674ad034ee3502aa9448324cd0056b7b0c670`
- expected commit tree: `44e9f3c3c84933d5679dd90a647ebcd377855ee8`
- patch SHA-256: `dfb98af1a80f7be36ef623476af27fe3b71db0638d5dc3f8e00d5c8f83bbc8ef`
- strict checkpatch: 0 errors, 0 warnings, 0 checks
- Asahi module build: PASS at the exact existing 24-warning bring-up baseline
- runtime: not installed/not executed; exact geometry only, no GPU allocation or RunCompute

The temporary runtime diagnostics used for E034 descriptor identity and E035 exact-wake testing are deliberately not included in the clean source checkpoint. Their conclusions are captured in `research/g15/G15-PIPE-SUBMISSION-BOUNDARY.md`.

No patch or tag in this repository implies an upstream AsahiLinux submission.

Patch 0024 validation performed on 2026-08-29:

- base: `4ff63937d4fcc1c4afc9b52c4e5cf1240f049716`
- expected commit tree: `69edfedc61b042a086bf1cd82386d3c09d60f18a`
- patch SHA-256: `674cc741ac89a47d05bcba1e9e8e3e83e5c9e98386cf30dd4cd20526de567584`
- exact-tree reconstruction: PASS
- strict checkpatch: 0 errors, 0 warnings, 0 checks
- Asahi module build: PASS at the established 24-individual-warning bring-up baseline
- module SHA-256: `3e4f66140e3c679932d953454505fb7df0a48fe3aa3f525120a21e7d620ef42e`
- module vermagic: `7.1.6-gc6fa9b794ba9 SMP preempt mod_unload aarch64`
- reachability: HWMetrics owner constructor has zero call sites; RunCompute `+0x857` remains zero
- runtime: not installed/not executed; no RunCompute

## Mesa / Honeykrisp

Base `origin/main` commit:

`6dd2f2919e74a1e038485b1dd08eb062c4230ebb`

Checkpoint head:

`d36216e783a3714eca0b734afb0df2301ece8739`

Series:

1. `1ca5933cb046140e222c9df3ec1e81dc4e9a150d` — initial load/clear state UAPI plumbing
2. `fa706f10b7c3d7fa9af91b02da62e664aa18f94c` — depth/stencil level-offset plumbing
3. `d36216e783a3714eca0b734afb0df2301ece8739` — structured G15 EOT state-loader path

These commits are research checkpoints and are not represented here as claims of upstream-ready general G15 support.

Patch 0017 validation performed on 2026-08-29:

- base: `b88369c26ffe82ce73765b8bbba64b2db771ef76`
- expected commit tree: `dcf3368ccd71d1166d5198cab9dd21b3daa2d6f8`
- patch SHA-256: `3a93a7f7398e5209ff710ea44f9a142179e070c5d7643d5beafc61383699c707`
- exact-tree reconstruction: PASS
- strict checkpatch: 0 errors, 0 warnings, 0 checks
- Asahi module build: PASS at the established 24-individual-warning bring-up baseline
- module vermagic: `7.1.6-gc6fa9b794ba9 SMP preempt mod_unload aarch64`
- runtime: not installed/not executed; no RunCompute

Patch 0018 validation performed on 2026-08-29:

- base: `9b21157497bbb1695b9bbdb43d6490c23520504e`
- expected commit tree: `65804f29fac725207b173f8e71f2f79f64fcf3f4`
- patch SHA-256: `0d123511937dd9f185cab93a4cc397a87ce16984a1dc2137ab8d92c360bc8bf7`
- exact-tree reconstruction: PASS
- strict checkpatch: 0 errors, 0 warnings, 0 checks
- Asahi module build: PASS at the established 24-individual-warning bring-up baseline
- module SHA-256: `ab9ea1342170bea6526b440d245a0bb4a88831db09018e0773d9673894dbdcf4`
- runtime: not installed/not executed; no RunCompute
Patch 0019 validation performed on 2026-08-29:

- base: `3cd3f336d9f4c103f2df7e284b9a517a42e7d90f`
- expected commit tree: `dbb40740b78de201ec0d1facd713628b9a5a5109`
- patch SHA-256: `7f6c87799aa8c79c4f8dacc8e91e20dd6564733735eb9bc41262a7548690f0ff`
- exact-tree reconstruction: PASS
- strict checkpatch: 0 errors, 0 warnings, 0 checks
- Asahi module build: PASS at the established 24-individual-warning bring-up baseline
- module SHA-256: `0bf6bc2f1329c58a0e048016315c2a0ff509d334bb467b7d3150430fa8bbe3a6`
- module vermagic: `7.1.6-gc6fa9b794ba9 SMP preempt mod_unload aarch64`
- runtime: not installed/not executed; no RunCompute

Patch 0020 validation performed on 2026-08-29:

- base: `167c037a91a0b85fa10480b90e883de31fa88e0d`
- expected commit tree: `27efe6a7509e5e1ca41a1561e24139644dc6b6c3`
- patch SHA-256: `4b9bf4d04c067000317257cc3e1cddf79451b714d249a907b315a204a439bb4f`
- exact-tree reconstruction: PASS
- strict checkpatch: 0 errors, 0 warnings, 0 checks
- Asahi module build: PASS at the established 24-individual-warning bring-up baseline
- module SHA-256: `fe09dd7d5471c73a1febdeb31a1d7e6c185c216036a1a6cd975b72d621f268f6`
- module vermagic: `7.1.6-gc6fa9b794ba9 SMP preempt mod_unload aarch64`
- runtime: not installed/not executed; no RunCompute

Patch 0021 validation performed on 2026-08-29:

- base: `97bd7129c3f05a22cc604ceb04a9c9bc39893100`
- expected commit tree: `2301c705e7d31892c9e06861aa4eb179e3809c2e`
- patch SHA-256: `83f6ab33b0fe7bfc28a704addeff80ea8881a936581ffc6057b3c67e197ed2cc`
- exact-tree reconstruction: PASS
- strict checkpatch: 0 errors, 0 warnings, 0 checks
- Asahi module build: PASS at the established 24-individual-warning bring-up baseline
- module SHA-256: `dbb29792369f0a3ee8b66a5abd00501fd4a18d45d503ef701fb59c8e293319b4`
- module vermagic: `7.1.6-gc6fa9b794ba9 SMP preempt mod_unload aarch64`
- reachability: owner constructor has zero call sites; RunCompute `+0x14` remains zero
- runtime: not installed/not executed; no RunCompute
Patch 0022 validation performed on 2026-08-29:

- base: `ddcbd85be239fa4db5143826bc3ca0588bacbd34`
- expected commit tree: `a492ce367bee16fab63aa8c80f465b83614655d8`
- patch SHA-256: `b51f92f11bb18f5b65e0071e043509f7f9a67737aad0df5759bdec0b5d5a5120`
- exact-tree reconstruction: PASS
- strict checkpatch: 0 errors, 0 warnings, 0 checks
- Asahi module build: PASS at the established 24-individual-warning bring-up baseline
- module SHA-256: `617064c5fa58080aa45e03d9d3a5166a4b1554338a3d65d8c94071656f4d5b1e`
- module vermagic: `7.1.6-gc6fa9b794ba9 SMP preempt mod_unload aarch64`
- reachability: selected-state seed definition only; backing owner constructor has zero call sites; RunCompute `+0x14` remains zero
- runtime: not installed/not executed; no RunCompute

Patch 0023 validation performed on 2026-08-29:

- base: `42bca8d2e9dcc25682aa0aa87d48521fadd1a828`
- expected commit tree: `7174c9d766c79ed6679ff5d32eb0eb8f85dbc0a5`
- patch SHA-256: `7b7b23b6f787692918b46e659ec598102dbd219a7413f2368e9cb169bd44f80b`
- exact-tree reconstruction: PASS
- strict checkpatch: 0 errors, 0 warnings, 0 checks
- module SHA-256: `7368ee7ea5959ba0ac47edfcfd1d6bdf12e18859c12d39222d1c8d98f6e99391`
- runtime: not installed/not executed; no RunCompute


Patch 0025 validation performed on 2026-08-29:

- base: `d18178f018aca2b92249ab72c5c361e19b6f45dc`
- expected commit tree: `c97331359f2996c1c7d09bedde5b99eaea362079`
- patch SHA-256: `3856385c33bdd33ee4556f93511642f6dff8c0b50e779f527b6993a2fa96eb2b`
- strict checkpatch: 0 errors, 0 warnings, 0 checks
- Asahi module build: PASS at the established 24-individual-warning bring-up baseline
- module SHA-256: `0fb676474a24d9125df61dad53ed4611ec8bb3838775d7942aa226b4bedc422a`
- runtime: not installed/not executed; no RunCompute

Patch 0026 validation performed on 2026-08-29:

- base: `47a8c0070a4be796f27ed64f3ca7a7f198b8af1b`
- expected commit tree: `8f10c024ee2743f91112f920d37567979ef0a07c`
- patch SHA-256: `562c1194867b1d305af3aa8c5f41731385db1d4b1e905bee73e422cfb4bbf18e`
- exact-tree reconstruction: PASS
- strict checkpatch: 0 errors, 0 warnings, 0 checks
- Asahi module build: PASS at the established 24-individual-warning bring-up baseline
- module SHA-256: `fc6f5ae8eed0e2d6543ab3a488b94da9ba2cc310d693fab3d49faa1b9a07cd91`
- runtime: not installed/not executed; no RunCompute

Patch 0027 validation performed on 2026-08-29:

- base: `bd95b9c01004eeb8b35f6c7d06224bac635d2cfa`
- expected commit tree: `2bc3de6ea13b5115594305bb597f10df92a735bc`
- patch SHA-256: `7d19ec1154d99778587329ec3dc10304f141e11d0214a741a186ed4b94eb9adf`
- exact-tree reconstruction: PASS
- strict checkpatch: 0 errors, 0 warnings, 0 checks
- Asahi module build: PASS at the established 24-individual-warning bring-up baseline
- module SHA-256: `b6373f5b22796f11994d2292663bff0464f945488c09edccb666bdce81829778`
- layout/source invariant audit: PASS
- reachability: serializer has zero call sites; RunCompute `+0x760` producer unchanged
- runtime: not installed/not executed; no RunCompute

Patch 0028 validation performed on 2026-08-29:

- base: `897d3ae4189603b58438131724e8d481f03b6917`
- expected commit tree: `16d86879298422b44475805f6cbcc2a1502579e1`
- patch SHA-256: `f39e59e9138a406f9459799417856f64e5568e9a9d701d978094dca99d4550ee`
- exact-tree reconstruction: PASS
- strict checkpatch: 0 errors, 0 warnings, 0 checks
- Asahi module build: PASS at the established 24-individual-warning bring-up baseline
- module SHA-256: `4ca508de93085f783563ce7dbf0c8043dd7bdd759ba8cbeaf221aea37fd71215`
- reachability: backing owner constructor has zero call sites; no selected slot/FWVA accessor; RunCompute `+0x760` unchanged
- runtime: not installed/not executed; no RunCompute

Patch 0029 validation performed on 2026-08-29:

- base: `d54dfc78d0592e2b8bb239aac61b9bc0489e3a78`
- expected commit tree: `d9b6b43aec68a22ea9b84728dc410c418fca65f0`
- patch SHA-256: `3fcc1da7175d5c620af0da495df655cb6120a1293b448fcf47995481af6dbf72`
- exact-tree reconstruction: PASS
- strict checkpatch: 0 errors, 0 warnings, 0 checks
- Asahi module build: PASS at the established 24-individual-warning bring-up baseline
- module SHA-256: `77c7ca7cb034a416c45127c3eeec9822979da6d380f78fffa6740a69d3d5273b`
- reachability: retirement guard definition only; no backing/serializer/submission call sites; live G15 gate unchanged
- runtime: not installed/not executed; no RunCompute

Patch 0030 validation performed on 2026-08-29:

- base: `69af01482ad8fbcba160559c308b8905598e3c6e`
- expected commit tree: `5a6003c16ef70d8539cc948f03e949bf5c83458f`
- patch SHA-256: `a72416b0cff7b53ce6ab38b038c03162f8c6e62811bf81ee97e9e56e923338e9`
- exact-tree reconstruction: PASS
- strict checkpatch: 0 errors, 0 warnings, 0 checks
- Asahi module build: PASS at the established 24-individual-warning bring-up baseline
- module SHA-256: `0a8797840a9b64d40d62cbc8c302f463fd812283d55a0df92202b1e5716387c6`
- reachability: prepared-slot writer/token definitions only; RunCompute `+0x760` unchanged
- runtime: not installed/not executed; no RunCompute

Patch 0031 validation performed on 2026-08-29:

- base: `f3464e7ec21633631df5e942303146069a62737e`
- expected commit tree: `047a29b09eae77f4956ebe31a2fef1c3abf201a9`
- patch SHA-256: `7ace399664a792102f8ea8830d558bc657c3dbfce7f5a03abc721eda8a61453a`
- exact-tree reconstruction: PASS
- strict checkpatch: 0 errors, 0 warnings, 0 checks
- Asahi module build: PASS at the established 24-individual-warning bring-up baseline
- module SHA-256: `56de872a869d55f785295258c88d43ba51b15639b4742a6bd3d64cc0e1ff41d5`
- reachability: owner graph constructor definition only; no FWVA accessors/call sites; RunCompute fields unchanged
- runtime: not installed/not executed; no RunCompute

Patch 0032 validation performed on 2026-08-29:

- base: `ea707e8e7726c30da653a026384d097c876db0dc`
- expected commit tree: `21c8418854d9c10ac54cebf6b5b60fda24214619`
- patch SHA-256: `7e845a12e9f06aee6a9ec3180c9ae12944b24dde753b7bfeb7f396f7845f92f4`
- exact-tree reconstruction: PASS
- strict checkpatch: 0 errors, 0 warnings, 0 checks
- Asahi module build: PASS at the established 24-individual-warning bring-up baseline
- module SHA-256: `edc77c86d83582e57b7aac8f9e372ba279316740b4f04bd646c91fadb1857bc2`
- reachability: token/materializer definitions only; no outside-module call sites; live `+0x14/+0x83e/+0x857` zero and `+0x760` unchanged
- runtime: not installed/not executed; no RunCompute

Patch 0033 validation performed on 2026-08-29:

- base: `a783f835b437e43f019b58b00daa0935b6dacf84`
- expected commit tree: `3878c44828b1e20edb26d7ce98a3ca9ea8fdd84a`
- patch SHA-256: `3c5d7cb4389ac7a33d0dac07e9b0a8e492a6817bedf62d9a08524d868f0f2aa9`
- exact-tree reconstruction: PASS
- strict checkpatch: 0 errors, 0 warnings, 0 checks
- Asahi module build: PASS at the established 24-individual-warning bring-up baseline
- module SHA-256: `7482c77f7883530ebb6130baf72173a962c54a07bf77aefe57e6e603e945ce68`
- reachability: guarded materializer definition only; no Queue/submission call site; live pointer fields unchanged
- runtime: not installed/not executed; no RunCompute

Patch 0034 validation performed on 2026-08-29:

- base: `63bb6ffe4f22abcd86db40b86e349a5aaea99bc2`
- expected commit tree: `a9dd74c5b7a0acb8b43e120f5e72d7cb0be99894`
- patch SHA-256: `3ac519ee47769c346b5d82bb092b3d46b9ce742275949389b31981f05c019221`
- exact-tree reconstruction: PASS
- strict checkpatch: 0 errors, 0 warnings, 0 checks
- Asahi module build: PASS at the established 24-individual-warning bring-up baseline
- module SHA-256: `ec27e7ac2f4fdb9725813017baeb1431e9361bcf9bf7083322f4082b62b3f755`
- reachability: field stage / guarded materializer definitions only; no `RunCompute` writer/call site; live pointer fields unchanged
- runtime: not installed/not executed; no RunCompute

E112 is a static integration-gap audit only and has no Linux patch. It leaves checkpoint `139b745dbf0092825cad3c65bc18fb03385f2305` / patch 0034 unchanged and explicitly blocks a direct staged-field writer until two-phase ordering, fence rollback, missing SKU sources, and active pre-SKU/JobParameters2 producers are closed.

Patch 0035 validation performed on 2026-08-29:

- base: `139b745dbf0092825cad3c65bc18fb03385f2305`
- expected commit tree: `66a6a838a94275fe6352471b7c9c12804920ddba`
- patch SHA-256: `9c7ccea01b1102e6a100817f04d84ac80187cd3750eeaf9069231c47c9237645`
- exact-tree reconstruction: PASS
- strict checkpatch: 0 errors, 0 warnings, 0 checks
- Asahi module build: PASS at the established 24-individual-warning bring-up baseline
- module SHA-256: `067a3767ed6782a8ae0f22985de1d40f46bc995464e819f9df962c111c3630b6`
- two-phase source invariant audit: PASS
- reachability: phase-1/finalize/abort definitions only; no Queue/submit_compute call sites; live `+0x14/+0x83e/+0x857/+0x760` producers unchanged
- runtime: not installed/not executed; no RunCompute

Patch 0036 validation performed on 2026-08-29:

- base: `b21300a1ad4db8df298c493c98f014803d33ad5b`
- expected commit tree: `9ae728e61b03d505b5c9f06e0f3d38eb056fc640`
- patch SHA-256: `54bd94f69309e5e87ce9326eae90d8e82ac2ea809993d935468e38b502796e7d`
- exact-tree reconstruction: PASS
- strict checkpatch: 0 errors, 0 warnings, 0 checks
- Asahi module build: PASS at the established 24-individual-warning bring-up baseline
- module SHA-256: `e9c75081c07cf34d0234ec929d1a22ef7f34d393a660c66b1110c865b6df3ec4`
- reachability: CL backing exists only inside the unreachable stock-empty owner graph; no Queue/submission call site; live pointer fields unchanged
- runtime: not installed/not executed; no RunCompute


Patch 0037 validation performed on 2026-08-29:

- base: `059d34701e480252829c6b397dad5ee2eb8881dc`
- expected commit tree: `e212c71cc52ac68382d3d8dc9a8b022790ae53e8`
- patch SHA-256: `3e55bb96fd6a6260c2172c51420b763ffed78775d31ab50a8ace2fab2b054821`
- exact-tree reconstruction: PASS
- strict checkpatch: 0 errors, 0 warnings, 0 checks
- Asahi module build: PASS at the established 24-individual-warning bring-up baseline
- module SHA-256: `e33655defa2032facbecd28fc58a61aa14fffe2b3adea61067f0b6c24765386e`
- reachability: channel-state backing owner definition only; no selected FWVA accessor/call site; live RunCompute fields unchanged
- runtime: not installed/not executed; no RunCompute

Patch 0038 validation performed on 2026-08-29:

- base: `252ded3b63533ab89b45e965546f2f0b9d2e57a2`
- expected commit tree: `3be13579f13cdc311255cc86a2588446c516ff71`
- patch SHA-256: `121ba441698ce9ffaad418ff60e3d39553d5d6e2bf228b3085e2e475b5e66f1d`
- exact-tree reconstruction: PASS
- strict checkpatch: 0 errors, 0 warnings, 0 checks
- Asahi module build: PASS at the established 24-individual-warning bring-up baseline
- module SHA-256: `7bb1fbc5efdb135b1d72a5c31342689dc0381991629b28031ef4c3e060b672f0`
- independent selected-state layout audit: PASS
- reachability: prepared-state helper is definition-only/private; no SKU or RunCompute conversion
- runtime: not installed/not executed; no RunCompute

Patch 0039 validation performed on 2026-08-29:

- base: `98d829e05a896cd7b2fa05c626aaef84128c2c9e`
- expected commit tree: `b51a0dee9e73cd2f4960a4ac93a3d66f8cbdeeb4`
- patch SHA-256: `6b77ba2e318ce31c2ea1c60e3d76aff593bad617455ca2a3a0443f4b6f1e154f`
- exact-tree reconstruction: PASS
- strict checkpatch: 0 errors, 0 warnings, 0 checks
- Asahi module build: PASS at the established 24-individual-warning bring-up baseline
- module SHA-256: `091ed2873c01ac2ad11df958f5d0e672c61b90dc311ccc22e8d940d0dbc3d9e4`
- independent selected-state gpu-buffer source/offset audit: PASS
- reachability: private selected-state helper only; live WorkQueue and RunCompute producers unchanged
- runtime: not installed/not executed; no RunCompute

Patch 0040 validation performed on 2026-08-29:

- base: `563b93f4bce3bb8d1ddfb12f9737acf99905fa71`
- expected commit tree: `51444ce1b31131b88d8db0b0da2cd27d4b8e816c`
- patch SHA-256: `fa17051647a4fcd55b9c818624e9d0d91d0de6c0b200de324c7d42f239690f47`
- exact-tree reconstruction: PASS
- strict checkpatch: 0 errors, 0 warnings, 0 checks
- Asahi module build: PASS at the established 24-individual-warning bring-up baseline
- module SHA-256: `96a6a87b2b7c6cc91da93452f89bf428296ea058d4b3f3b784e913da77495f83`
- E122/E123 channel-memory geometry/mapping-class audit: PASS
- reachability: new backing owners definition-only; no selected FWVA accessor/owner-graph call site; live WorkQueue/RunCompute producers unchanged
- runtime: not installed/not executed; no RunCompute

Patch 0041 validation performed on 2026-08-29:

- base: `14954ffdd9c5fe47126b1af5cc99bf8a9f6a2140`
- expected commit tree: `81c687644b41f17b4c9764609cc47697b7d08e83`
- patch SHA-256: `fc18ea83ddd0b36e03f5ec968b89e58ce306a486257119bb510046c2ee7c1831`
- exact-tree reconstruction: PASS
- strict checkpatch: 0 errors, 0 warnings, 0 checks
- Asahi module build: PASS at the established 24-individual-warning bring-up baseline
- module SHA-256: `660eeb875f6abe13a860af455bd29b1ca0739d60870be79d62bf033d9c30a096`
- exact uncached selected-element reset-header audit: PASS
- reachability: helper returns local slot-index token only; no FWVA/cached-memory/live WorkQueue/RunCompute change
- runtime: not installed/not executed; no RunCompute

Patch 0042 validation performed on 2026-08-29:

- base: `1d9c455c0c0dfdb6a5b9703bfb44bb6b2877736c`
- expected commit tree: `ca21d51c7c50740e1e925ee6838c3634c06291dc`
- patch SHA-256: `9f609c0a9a06ef5e625072c254b3be52c614cf4161be664e285d4c7b13ad5dca`
- exact-tree reconstruction: PASS
- strict checkpatch: 0 errors, 0 warnings, 0 checks
- Asahi module build: PASS at the established 24-individual-warning bring-up baseline
- module SHA-256: `7630b231b211b6f71ebbc40be7cd3a07f16ef1d333091826761306b02d3aa7d7`
- exact QueueInfo +0x10/timestamp resource-stack/reset-state audit: PASS
- reachability: timestamp owner/reset definition-only; private semantic input corrected; live WorkQueue notifier path and RunCompute producers unchanged
- runtime: not installed/not executed; no RunCompute

Patch 0043 validation performed on 2026-08-29:

- base: `adfbfca2e26565ce974fd6c238c4910abf99533e`
- expected commit tree: `2a2089948c3d9795ee385d364e70a5c39b162c92`
- patch SHA-256: `9b52358b14a67ccccfb4244edf3264b5c9aac14f7d0e479d4ea8b78e3c64ee67`
- exact-tree reconstruction: PASS
- strict checkpatch: 0 errors, 0 warnings, 0 checks
- Asahi module build: PASS at the established 24-individual-warning bring-up baseline
- module SHA-256: `a5e31931d40181f8bb2aa2ce1bdc5a7c707b16603df87fd3209014554d817270`
- exact scheduler resource-stack/selected-state geometry-image audit: PASS
- reachability: scheduler owner/reset definition-only; not in combined owner graph; live generic GpuContext and RunCompute producers unchanged
- runtime: not installed/not executed; no RunCompute

Patch 0044 validation performed on 2026-08-29:

- base: `293ca63380179cca5bb8c6511a67349f90878b44`
- expected commit tree: `87431d2ef1ea1a35882d5a499117d73ae295ea30`
- patch SHA-256: `840155573f3adb6104ac1b3f37a150950f23f221befdee627b94d6dbd81c10da`
- exact-tree reconstruction: PASS
- strict checkpatch: 0 errors, 0 warnings, 0 checks
- Asahi module build: PASS at the established 24-individual-warning bring-up baseline
- module SHA-256: `a2d0cbf1d48639b4271f9136e60c70e8d80113848a3e13cda2db1b3a1d69a48a`
- independent distinct-index channel-owner/source audit: PASS
- reachability: combined owner graph/guard wrapper only; no Queue call site or RunCompute writer
- runtime: not installed/not executed; no RunCompute


Patch 0045 validation performed on 2026-08-29:

- base: `8f6bd394ceb5bdcd62bfa2ca3817c85bd7cabf9d`
- expected commit tree: `b02f2c4472e65afee9c404759da93622e3840db6`
- patch SHA-256: `d00b74399fec6bc6b8e14880ffa9b8a5261f9001ac79b732c2d8293582513070`
- exact-tree reconstruction: PASS
- strict checkpatch: 0 errors, 0 warnings, 0 checks
- Asahi module build: PASS at the established 24-individual-warning bring-up baseline
- module SHA-256: `ced66b40865979c5aa70686673ae939058ea04d4db789428285d87f2f19da66e`
- exact first-CL index / accelerator packed-feature bit-39 audit: PASS
- reachability: fixed values consumed only by the dormant owner-graph finalizer; generic serializer still parameterized; no live Queue call site or RunCompute writer
- runtime: not installed/not executed; no RunCompute

Patch 0046 validation performed on 2026-08-29:

- base: `ccc976c598ab40f9cd8ef24837af1139979bbc09`
- expected commit tree: `7d9dce6e7d31964e1510a989be75b410f25575ad`
- patch SHA-256: `5a25d57ee310108032636fa8cca8b6f53f92043660ad68151b089da570846741`
- exact-tree reconstruction: PASS
- strict checkpatch: 0 errors, 0 warnings, 0 checks
- Asahi module build: PASS at the established 24-individual-warning bring-up baseline
- module SHA-256: `2e0b960a2d69686696888523c03053693b3c20cc6f4af80d45c1d5fe736f7358`
- exact ordinary type-5 IOGPU / J615 GART HW-soft-fault audit: PASS (`isHWSoftFaultEnabled() = false`)
- reachability: target finalize boolean removed; generic serializer remains parameterized; dormant owner/wrapper only; no live Queue call site or RunCompute writer
- runtime: not installed/not executed; no RunCompute

Patch 0047 validation performed on 2026-08-29:

- base: `65451e3d5cc8ebc582433abf874811c7eada79b9`
- expected commit tree: `7ffe7e70a303ccefc7e46d9595fb33de09d6b1d4`
- patch SHA-256: `c92763d73d9587e0e7c4116e5721c8238c94e6e4c1acb35f4b00690b1b790796`
- exact-tree reconstruction: PASS
- strict source-diff checkpatch: 0 errors, 0 warnings, 0 checks
- Asahi module build: PASS at the established 24-individual-warning bring-up baseline
- module SHA-256: `f58313ed791123de197c9094199867cd50e5e3fc4c04d96eb8f00a67f5732039`
- exact AGXFirmware `+0x228/+0x268` / 0xe10 Compute-statistics ownership and zero-initialization audit: PASS
- reachability: raw target finalize FWVA removed; typed `G15StatsComp` owner pointer consumed only by dormant owner graph; generic serializer remains parameterized; no live Queue call site or RunCompute writer
- runtime: not installed/not executed; no RunCompute

Patch 0048 validation performed on 2026-08-29:

- base: `635fca8e84b3d6ba27f6f7dd760275c635cd3495`
- expected commit tree: `a54e4d9c9792d286e70607aa388f5c0dc61dbe9a`
- patch SHA-256: `6b3d93b5a39003e650fb6ddfe4ce8994358d3e2d9da0e1897d7cf0395a73ed66`
- exact-tree reconstruction: PASS
- strict source-diff checkpatch: 0 errors, 0 warnings, 0 checks
- Asahi module build: PASS at the established 24-individual-warning bring-up baseline
- module SHA-256: `affcea840e5e49f362a562c4a20f0933a64741a7f44385b17ae4600ab77aec90`
- exact RunCompute source audit: counter/context/FW-stamp/stamp-value/UUID/event-sequence/user-timestamp-presence closure PASS
- reachability: target scalar finalize-input struct removed; typed RunCompute command consumed only by dormant finalizer; no live Queue caller or RunCompute writer
- runtime: not installed/not executed; no RunCompute

Patch 0049 validation performed on 2026-08-30:

- base: `251a60a085f3bb9bfdcb04f1d6fd28f24dbf2cbb`
- expected commit tree: `6e3a8f1ed53a8890f0f6c1437db18a5a541e221d`
- patch SHA-256: `9ecc99bd4273bd2e894c88dc73cfb95d80909cdd98987d96fb58bf233406eb14`
- exact-tree reconstruction: PASS
- strict source-diff checkpatch: 0 errors, 0 warnings, 0 checks
- Asahi module build: PASS at the established 24-individual-warning bring-up baseline
- module SHA-256: `b4f6dddbaa80a53ee853ac18d424f6e91cb13969849ff50912c26eba13a2444f`
- exact source correction: parsed Compute wrapper `+0x20/+0x38 <- raw +0xc0/+0xd8`; RunCompute `+0x740/+0x748/+0x750` stock-empty values all zero
- runtime: not installed/not executed; no RunCompute


Patch 0050 validation performed on 2026-08-30:

- base: `f09bed89530e5fa482240c55653545a3bed3e292`;
- expected commit tree: `179518d4f4d3d41ce658800f2f0e92ad7b2d176e`;
- patch SHA-256: `4c0ea24e0581fc0f4ce8e6de1fc9ed1fce25cffb91d68ae3a628fc58f3bacbd0`;
- exact-tree reconstruction: PASS;
- strict source-diff checkpatch: 0 errors, 0 warnings, 0 checks;
- Asahi module build: PASS at the established 24-individual-warning bring-up baseline;
- module SHA-256: `b6f65074d982472119d326a1eebb286a0680aa57c002fda0c9f0ec32ef9747e9`;
- transaction audit: fence arm acquired before dormant event/SKU binding and retained through finalize; explicit rollback/completion releases asset ownership before pending-count release;
- reachability: private dormant wrapper only; ordinary `submit_compute()` unchanged; no RunCompute writer;
- runtime: not installed/not executed; no RunCompute.

Patch 0051 validation performed on 2026-08-30:

- base: `a244a846e3014b9eebdec580f852e39d27a1a50d`;
- expected commit tree: `f6dc1e1f4a7bd308991dec1de29b41879a56f452`;
- patch SHA-256: `19cdffd3af93a7d0043572e23f9edac69dd02aca8228e2b476a4b802a52deb91`;
- exact-tree reconstruction: PASS;
- strict source-diff checkpatch: 0 errors, 0 warnings, 0 checks;
- Asahi module build: PASS at the established 24-individual-warning bring-up baseline;
- module SHA-256: `efe3e1ed324db6d3abf77ac6b930a5f7a16ef27c45ad9b187c10c123bb925d9b`;
- ownership audit: one definition-only device-global UMA manager/pool-ID state, separate reusable shared Compute pool/FList owner, and channel/command owner with no FList;
- reachability: no `GpuManager` instance, no live shared Compute pool, no Queue call site, ordinary `submit_compute()` unchanged, no RunCompute writer;
- runtime: not installed/not executed; no RunCompute.

Patch 0052 validation performed on 2026-08-30:

- base: `7c7e4d68082f8ff349c191bc3021c2f7d023009e`;
- expected commit tree: `1e10f2d211ddb16995ac73de2c7c5243f9f6d1b0`;
- patch SHA-256: `708529d42db6c1f162d8a8e37c754a23e7e25bef364c1349a9f4b11680488b57`;
- exact-tree reconstruction: PASS;
- strict source-diff checkpatch: 0 errors, 0 warnings, 0 checks;
- Asahi module build: PASS at the established 24-individual-warning bring-up baseline;
- module SHA-256: `3f9406c6b481c57e6d15082414968b128e330dc035c8a9fe1c0e8330586ead9e`;
- placement audit: one G15-only GpuManager UMA host state owns the 0x100 HardwareBuffer-ID namespace and zero-seeded pool-ID sequence; construction creates no UMAPool and consumes no pool ID;
- reachability: no shared-pool constructor call, no Queue accessor/call site, ordinary `submit_compute()` unchanged, no RunCompute writer;
- runtime: module not installed/not executed; no RunCompute.

Patch 0053 validation performed on 2026-08-30:

- base: `f2cb03001da02b1bde833550f38700b247ca30c2`;
- expected commit tree: `0975d080969e61298bb5b36f6f0d4e1748586a89`;
- patch SHA-256: `edc7855793bb335953815aa232e9b9552a37138f560cdf3a075f6617a9df3ad3`;
- exact-tree reconstruction: PASS;
- strict source-diff checkpatch: 0 errors, 0 warnings, 0 checks;
- Asahi module build: PASS at the established 24-individual-warning bring-up baseline;
- module SHA-256: `02821df79de9b52dc509c64a1a8e35ba3aa734921a20d27d2e8ab44c6e71ee68`;
- ownership audit: one empty four-slot non-owning client-VM UMA container state beside the existing range-5 allocators, with no strong shared-pool owner or Queue accessor;
- reachability: the only pool-ID allocation consumer remains the dormant E136 shared-pool constructor; no live submit/RunCompute path changed;
- runtime: not installed/not executed; no RunCompute.

Patch 0054 validation performed on 2026-08-30:

- base: `e48a1f854d3adf675374aad6b6bff352805904bf`;
- expected commit tree: `f607a93285a3f615d554d424b100d09094ecc69d`;
- patch SHA-256: `e90b953a3ef2e6549a7973ad2ef0e6b7d7b4fdb6e260fc6375522ae235a17f2d`;
- exact-tree reconstruction: PASS;
- strict source-diff checkpatch: 0 errors, 0 warnings, 0 checks;
- Asahi module build: PASS at the established 24-individual-warning bring-up baseline;
- module SHA-256: `411f9dd5f5e1cfc3f4f16fcdbef994e6234dfd15e1148d93f332211c3262de6c`;
- ownership audit: four client-VM slots remain identity-only/non-owning while logical direct references and the separate active-channel epoch enforce nonzero-only promotion and identity-checked final clearing;
- reachability: no Queue accessor/call site, no actual pool owner in the container, ordinary `submit_compute()` unchanged and no RunCompute writer;
- runtime: module not installed/not executed; no RunCompute.

Patch 0055 validation performed on 2026-08-30:

- base: `a301b5b72feff2e32f69dd9e5e560a4c61a2ed60`;
- expected commit tree: `1a469e6be6435ff56048209f298d57dc458f45dc`;
- patch SHA-256: `9ab879b373f5ff5c9191ab5bace4c8d32f11d325928c64d5c3094dd84c77f783`;
- exact-tree reconstruction: PASS;
- strict source-diff checkpatch: 0 errors, 0 warnings, 0 checks;
- Asahi module build: PASS at the established 24-individual-warning bring-up baseline;
- module SHA-256: `ba5c556ce93c9b50e1450e4e050b7ad43dae3618c23c75a2e8eae97e9d0f88ae`;
- transaction audit: exact 23J220 keeps the client-container lock across failed try-retain, new UMAPool factory/init, accelerator-list insertion and slot publication; Linux create closure is serialized under the same modeled transaction;
- reachability: per-VM container still has no Queue accessor, shared-pool constructor has zero callers, ordinary `submit_compute()` unchanged and no RunCompute writer;
- runtime: module not installed/not executed; no RunCompute.

Patch 0056 validation performed on 2026-08-30:

- base: `594d39b1b85e35ae315d048d60528cfe9a8b0d10`;
- expected commit tree: `198d50e59b1e8a238d8ff39d49f6627a118f05cf`;
- patch SHA-256: `d1e0d2724a77fe1a46d33b0af557d7023eed61b6f149ac4aadcb059ea4a4acb0`;
- exact-tree reconstruction: PASS;
- strict source-diff checkpatch: 0 errors, 0 warnings, 0 checks;
- Asahi module build: PASS at the established 24-individual-warning bring-up baseline;
- module SHA-256: `31e59ac611927530043b2352fe753faa4c178adaa9ec79498a50c6b2971164c3`;
- ownership audit: the per-file::Vm shared-pool container Arc is retained through GpuManager into private G15 QueueInner, matching exact command-queue -> AGXShared -> +0x1b8 provenance;
- reachability: no selection/create caller, no shared-pool constructor caller, no global pool-ID consumption and no Compute/RunCompute source change;
- runtime: module not installed/not executed; no RunCompute.

Patch 0057 validation performed on 2026-08-30:

- base: `848a9d426b2d1bb25a3f917ed37b15300b6d7f53`;
- expected commit tree: `c62064dd8399232215e5fb8328ead9061548e522`;
- patch SHA-256: `826d187ed1077baeb9a42c04725f81da20e3651f603644cbd09fa738790a7556`;
- exact-tree reconstruction: PASS;
- strict source-diff checkpatch: 0 errors, 0 warnings, 0 checks;
- Asahi module build: PASS at the established 24-individual-warning bring-up baseline;
- module SHA-256: `4dba06a8fd5958322d009342464858fb2f2a942d075065e50e9c6e46c0863e0e`;
- ownership audit: QueueInner privately retains the E143 container plus per-client range-5 cached/uncached and shared bank1/q22 handles required by the dormant pool/channel owners;
- reachability: no shared-pool selection/constructor call, no channel-owner constructor call, no pool-ID consumption and no Compute/RunCompute source change;
- runtime: module not installed/not executed; no RunCompute.

Patch 0058 validation performed on 2026-08-30:

- base: `6c65dc3a73c6c724d198015b6addfcf3bdfce5f3`;
- expected commit tree: `1d826ecb520897149f09a2b2336fee7c93ab38d2`;
- patch SHA-256: `815c348bb9a61aec72eacad6626a354c01641c04931928ddc0dabc406d3da74d`;
- exact-tree reconstruction: PASS;
- strict source-diff checkpatch: 0 errors, 0 warnings, 0 checks;
- Asahi module build: PASS at the established 24-individual-warning bring-up baseline;
- module SHA-256: `22f09c1223e162a6465c96b4d1ff6d8eabcbf37b52a4c0c3987b48b9b8aebb82`;
- ownership audit: client-container replacement transaction now reaches device-global pool identity plus cached range-5 FList construction; channel-local owners are assembled only after selection returns; host-only sticky-ID cookie derives from pool identity;
- reachability: assembly helper has zero callers, `queue/compute.rs` unchanged, no live pool-ID consumer and no RunCompute writer;
- runtime: module not installed/not executed; no RunCompute.

Patch 0059 validation performed on 2026-08-30:

- base: `899f1b6b8ba7bbf81b33c7aa75dcbdd56a1c9ed4`;
- expected commit tree: `604d859c910c383246f3316c9de1d5fe480388eb`;
- patch SHA-256: `40386ce8796e4e57927f7ad4df6a105b2220866491285de2a19baacc44ba89dc`;
- exact-tree reconstruction: PASS;
- strict source-diff checkpatch: 0 errors, 0 warnings, 0 checks;
- Asahi module build: PASS at the established 24-individual-warning bring-up baseline;
- module SHA-256: `27cca5cf07d8f2737cddb9160a3f87b1e43888517d02ad68b3b9d349ddbda302`;
- source audit: channel `+0x1e8 <- command queue +0x44c`; shared-pool class is 0 only for types 0/5 and 1 otherwise; normal exact types are 3 initially then 1/2;
- reachability: dormant assembly helper remains zero-caller, `queue/compute.rs` unchanged and no live pool-ID consumer/RunCompute writer is added;
- runtime: module not installed/not executed; no RunCompute.

Patch 0060 validation performed on 2026-08-30:

- base: `b06c8650159bf700c3d4766542da8c976ef19048`;
- expected commit tree: `8c1d9c20f68a199ae0ca777bb51ce75265da545c`;
- patch SHA-256: `dbe93a8bddf28239f270d5634d77d6b9d787d7b0a90a3c26a2ba7f85bd29baf7`;
- exact-tree reconstruction: PASS;
- strict source-diff checkpatch: 0 errors, 0 warnings, 0 checks;
- Asahi module build: PASS at the established 24-individual-warning bring-up baseline;
- module SHA-256: `5e7b8335fe27806e451a2dd9e34d4606529cbe959499ea7cec327272cc2ebd46`;
- selector audit: exact 0x800 global ceiling, two-level lowest-free selection, eager logical first backing, growth, per-backing use counts and inverse release; independent timestamp/scheduler/channel namespaces verified including non-32-aligned timestamp growth;
- lifetime audit: real Queue construction owns timestamp+scheduler logical leases; zero-caller unpublished Compute assembly owns independent channel-state/cached/uncached leases with RAII rollback;
- reachability: no resource-stack GPU backing/FWVA, E127 local proof indices remain separate, `queue/compute.rs` / `fw/compute.rs` / `workqueue.rs` unchanged and no RunCompute writer;
- runtime: module not installed/not executed; no RunCompute.

Patch 0061 validation performed on 2026-08-30:

- base: `1308998922f430f77c5e19ecee33dc5799bfdb8f`;
- expected commit tree: `cebe8170d2e83af5c51d82ed2a358a7093b63205`;
- patch SHA-256: `2bccb08fe73be8201512acedd095501b6664d2926f8f1c992d046c2e519717b7`;
- exact-tree reconstruction: PASS;
- strict source-diff checkpatch: 0 errors, 0 warnings, 0 checks;
- Asahi module build: PASS at the established 24-individual-warning bring-up baseline;
- module SHA-256: `9050511799b03ab7db6d81977309321aaeebc5f4e0dc8854edc39b6e6d6d3e15`;
- exact policy audit: all five target stacks use `AGXFWPoolShrinkMode=0`; backing count seeds zero; normal release uses `force=false`; final selected release removes the complete backing/global-index range;
- logical audit: lower backing holes stay unavailable while later present backings have free slots; growth then reuses the lowest absent backing slot; exact timestamp 0x800 partial-tail ceiling retained;
- reachability: only `buffer.rs` changes; no mapped resource-stack backing/FWVA; `queue/compute.rs`, `fw/compute.rs`, and `workqueue.rs` unchanged;
- runtime: module not installed/not executed; no RunCompute.

Patch 0062 validation performed on 2026-08-30:

- base: `368783dd61e2dd709d99b696feb4a8af9c7ec767`;
- expected commit tree: `592a70d8a512843c61cb5d36ad6f72d9ff62a03b`;
- patch SHA-256: `9405244ada3d6e47d7d120edc31a10520a7f7611cb4dc87e975633084e875f5a`;
- exact-tree reconstruction: PASS;
- strict source-diff checkpatch: 0 errors, 0 warnings, 0 checks;
- Asahi module build: PASS at the established 24-individual-warning bring-up baseline;
- module SHA-256: `952ee4dc51157f01de43ec1f404da8e71dba5ba12a2dab63c0f8fe09e7905e8a`;
- geometry audit: exact five resource kinds, backing sizes, per-backing element counts, 0x800 global ceiling/tails and selected `base + local_index * element_size` arithmetic;
- lifetime audit: optional real mapped backing is owned by E149's dynamic mode-0 backing slot and is dropped on final-use block removal;
- reachability: `new_mapped_device_global()` and `mapped_fwva()` are definition-only; live `GpuManager` construction remains on `new_device_global()`; `gpu.rs`, `queue/compute.rs`, `fw/compute.rs`, and `workqueue.rs` unchanged;
- runtime: module not installed/not executed; no new resource mapping and no RunCompute.

Patch 0063 validation performed on 2026-08-30:

- base: `f17014117d4a77e7029a7f61abf85bf8c5aea5cf`;
- expected commit tree: `fcf4097b98e20b6c341e010468039056d2b7c6f8`;
- patch SHA-256: `97645134b0095277f2b8c6a8130bd6b0a81d9dad7f7c949406aba9742e31e029`;
- exact-tree reconstruction: PASS;
- strict source-diff checkpatch: 0 errors, 0 warnings, 0 checks;
- Asahi module build: PASS at the established 24-individual-warning bring-up baseline;
- module SHA-256: `7de35a06a1a2f038cb4392f07429651d05d30499a089e068740b18a10a84e556`;
- exact ordering audit: all five global resource stacks eagerly create first backings while constructor-zero firmware `+0x1288` suppresses q22 mapping publication; successful `prepareFirmwareData()` sets that gate to one before `initFirmwareData()` and `bootFirmware()`;
- notification-policy audit: existing mappings remain immediate; only the dormant mapped firmware-resource factory uses `AfterActivation`, retaining post-activation growth/unmap publication without replaying bootstrap maps;
- reachability: notifier activation, `new_mapped_device_global()` and `mapped_fwva()` remain zero-caller; `gpu.rs` is unchanged and no new runtime mapping is created;
- runtime: module not installed/not executed; no RunCompute.
