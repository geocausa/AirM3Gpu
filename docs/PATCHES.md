# Patch Bases

The patch files in this repository are reproducible exports from separate component worktrees.

## Linux / Asahi

Base commit:

`d8c844ca7c34abb01b93851cb405fb42b2650f0a`

Checkpoint commit:

`b3d38d16c076a5b41031cbe5cec31c3cc1a894f2`

Local checkpoint tag:

`airm3gpu-2026-08-24-rtkit-management`

Patch:

`patches/linux/0001-drm-asahi-checkpoint-T8122-G15-bring-up-through-RTKi.patch`

The temporary built-in `drivers/soc/apple/rtkit.c` endpoint logger used to identify the EP1 crashlog request was intentionally excluded from this component checkpoint.

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
