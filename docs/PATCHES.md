# Patch Bases

The patch files in this repository are reproducible exports from separate component worktrees.

## Linux / Asahi

Base commit:

`d8c844ca7c34abb01b93851cb405fb42b2650f0a`

Previous public checkpoint head:

`1b57b289af96973badfbb8489ef379a1b3a96f07`

Current clean checkpoint head:

`2f08f68bb2efdadf2d337441553c1f682152a748`

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

Patch 0004 is intentionally a squashed checkpoint delta from `1b57b289af96` to `2f08f68bb2ef`. The local development history between those revisions contains many staged bring-up commits; publishing the validated end-state delta keeps this repository reproducible without presenting every one-shot experiment as an upstream-ready commit series.

Validation performed on 2026-08-26:

- `git apply --check` against `1b57b289af96`: PASS
- temporary-index application tree: `e985490062efc4baf5847b6e73cd7cf799a5ffaa`
- expected `2f08f68bb2ef^{tree}`: `e985490062efc4baf5847b6e73cd7cf799a5ffaa`
- exact tree match: PASS

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
