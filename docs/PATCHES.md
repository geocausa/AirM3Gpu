# Patch Bases

The patch files in this repository are reproducible exports from separate component worktrees.

## Linux / Asahi

Base commit:

`d8c844ca7c34abb01b93851cb405fb42b2650f0a`

Previous public checkpoint head:

`1b57b289af96973badfbb8489ef379a1b3a96f07`

Current clean checkpoint head:

`ed17ac035ad2422d4a85146e97c88cb3057eb174`

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

Patch 0004 is intentionally a squashed checkpoint delta from `1b57b289af96` to `2f08f68bb2ef`. Patch 0005 is the focused compile-only delta from `2f08f68bb2ef` to `1d264651a204`, adding the exact E068 stock empty-Compute G15 RegisterArray while leaving ordinary G15 submission fail-closed. Patch 0006 is the focused E071 delta from `1d264651a204` to `f73b9e551658`, changing only RunCompute `+0x846` to the exact prepared-state value `1`. Patch 0007 is the focused E073 delta from `f73b9e551658` to `03fdbb86230f`, adding the exact managed G15 context-generation lifetime and exporting it at RunCompute `+0x85f`. Patch 0008 is the focused E074 diagnostic delta from `03fdbb86230f` to `4e26fc089860`: it performs only a read-only range-8 shared-parent ownership check and then fails closed before persistent G15 runtime. Patch 0009 is the focused E075 delta from `4e26fc089860` to `e9f50fcc17d5`: it live-validates the exact range-8 leaf class and reversible eight-parent bank-1 teardown. Patch 0010 is the focused E076 compile-only delta from `e9f50fcc17d5` to `6e3850dcdd51`: it splits bank-1 VA allocation into disjoint range-7/range-8 arenas and hard-wires the range-8 constructor. Patch 0011 is the focused E078 compile-only delta from `6e3850dcdd51` to `886820e1f460`: it adds the exact FList FW-Uncached range-7 leaf class, fixed 8-byte ABI and hard aperture/protection pairing without creating a persistent FList object. Patch 0012 is the focused E079 compile-only delta from `886820e1f460` to `865f24f2a9fc`: it reconstructs the 256-ID HardwareBuffer manager state machine but deliberately leaves synchronization/runtime ownership unwired. Patch 0013 is the focused E080 compile-only delta from `865f24f2a9fc` to `ed17ac035ad2`: it adds the synchronized manager wrapper and FList sticky owner without instantiating either at runtime. The local development history between those revisions contains many staged bring-up commits; publishing the validated end-state delta keeps this repository reproducible without presenting every one-shot experiment as an upstream-ready commit series.

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

The temporary runtime diagnostics used for E034 descriptor identity and E035 exact-wake testing are deliberately not included in the clean source checkpoint. Their conclusions are captured in `research/g15/G15-PIPE-SUBMISSION-BOUNDARY.md`.

No patch or tag in this repository implies an upstream AsahiLinux submission.

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
