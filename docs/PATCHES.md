# Patch Bases

The patch files in this repository are reproducible exports from separate component worktrees.

## Linux / Asahi

Base commit:

`d8c844ca7c34abb01b93851cb405fb42b2650f0a`

Previous public checkpoint head:

`1b57b289af96973badfbb8489ef379a1b3a96f07`

Current clean checkpoint head:

`d18178f018aca2b92249ab72c5c361e19b6f45dc`

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

Patch 0004 is intentionally a squashed checkpoint delta from `1b57b289af96` to `2f08f68bb2ef`. Patch 0005 is the focused compile-only delta from `2f08f68bb2ef` to `1d264651a204`, adding the exact E068 stock empty-Compute G15 RegisterArray while leaving ordinary G15 submission fail-closed. Patch 0006 is the focused E071 delta from `1d264651a204` to `f73b9e551658`, changing only RunCompute `+0x846` to the exact prepared-state value `1`. Patch 0007 is the focused E073 delta from `f73b9e551658` to `03fdbb86230f`, adding the exact managed G15 context-generation lifetime and exporting it at RunCompute `+0x85f`. Patch 0008 is the focused E074 diagnostic delta from `03fdbb86230f` to `4e26fc089860`: it performs only a read-only range-8 shared-parent ownership check and then fails closed before persistent G15 runtime. Patch 0009 is the focused E075 delta from `4e26fc089860` to `e9f50fcc17d5`: it live-validates the exact range-8 leaf class and reversible eight-parent bank-1 teardown. Patch 0010 is the focused E076 compile-only delta from `e9f50fcc17d5` to `6e3850dcdd51`: it splits bank-1 VA allocation into disjoint range-7/range-8 arenas and hard-wires the range-8 constructor. Patch 0011 is the focused E078 compile-only delta from `6e3850dcdd51` to `886820e1f460`: it adds the exact FList FW-Uncached range-7 leaf class, fixed 8-byte ABI and hard aperture/protection pairing without creating a persistent FList object. Patch 0012 is the focused E079 compile-only delta from `886820e1f460` to `865f24f2a9fc`: it reconstructs the 256-ID HardwareBuffer manager state machine but deliberately leaves synchronization/runtime ownership unwired. Patch 0013 is the focused E080 compile-only delta from `865f24f2a9fc` to `ed17ac035ad2`: it adds the synchronized manager wrapper and FList sticky owner without instantiating either at runtime. Patch 0014 is the focused E081 compile-only delta from `ed17ac035ad2` to `affdd1fba79d`: it adds exact symbolic FList geometry and a no-allocation resource plan. Patch 0015 is the focused E082 compile-only delta from `affdd1fba79d` to `724674ad034e`: it pins the exact FList Page/Backup List compact-`0x300` range-5 class under a dedicated semantic protection constant while preserving the no-allocation resource plan. Patch 0016 is the focused E083 compile-only delta from `724674ad034e` to `b88369c26ffe`: it pins exact J615 M/B/P defaults and resolves the checked 4-MiB/32-KiB list geometry without creating GPU resources. Patch 0017 is the focused E085 compile-only delta from `b88369c26ffe` to `9b21157497bb`, fixing q22 range-8 special-aperture map/unmap flags to exact `3/2` while preserving range-7 `1/0`. Patch 0018 is the focused E086 compile-only delta from `9b21157497bb` to `3cd3f336d9f4`, owning the exact four FList persistent backings but leaving prepare/populate and command publication unreachable. Patch 0019 is the focused E089 compile-only delta from `3cd3f336d9f4` to `167c037a91a0`, replacing the inherited G15 queue-wide notifier export at RunCompute `+0x14` with an exact event-control FWVA field that remains zero/fail-closed until the 36-state owner is reconstructed. Patch 0020 is the focused E091 compile-only geometry delta from `167c037a91a0` to `97bd7129c3f0`. Patch 0021 is the focused E093 compile-only backing-owner delta from `97bd7129c3f0` to `ddcbd85be239`. Patch 0022 is the focused E094 compile-only selected-state delta from `ddcbd85be239` to `42bca8d2e9dc`, pinning the exact J615 effective-record-count value and post-finish reset/seed boundary while leaving selected FWVA publication unreachable. Patch 0023 is the focused E096 compile-only slot-reuse delta from `42bca8d2e9dc` to `4ff63937d4fc`, correcting event-control `+0x08` and retaining a submission fence until each rotated slot is retired. Patch 0024 is the focused E098 compile-only HWMetrics delta from `4ff63937d4fc` to `d18178f018ac`, adding the exact compact-`0x30b`/leaf-`0x...040b` range-7 HWMetrics class and one unreachable page-base owner while keeping RunCompute `+0x857` zero. The local development history between those revisions contains many staged bring-up commits; publishing the validated end-state delta keeps this repository reproducible without presenting every one-shot experiment as an upstream-ready commit series.

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
