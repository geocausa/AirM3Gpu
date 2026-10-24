# AirM3Gpu

Experimental clean-room bring-up and reverse-engineering work for Apple M3 / T8122 J615 G15G C0 on Linux.

This repository is the canonical public research checkpoint. Compilable Linux implementation history lives in `geocausa/linux`; raw proprietary and machine-local evidence remains private.

## Current state — 2026-10-25

The physical MacBook Air M3 has crossed real Compute, graphics execution, native Apple DCP/KMS display takeover, simultaneous GPU+display operation, and direct GPU-produced DMA-BUF presentation through native Apple DRM/DCP.

- **E1075** restores the exact Apple-derived T8122 HwDataA `+0x418c..+0x41db` block and moves the reconstructed J615 runtime to `firmware-running` with `/dev/dri/renderD128` present.
- **E1076** proves correct Vulkan Compute/readback: `65536 invocations x 256 steps`, `788 runs in 1.0 s`, `0 wrong results`.
- **E1077** proves correct Vulkan graphics and triangle readback: `64x64 clear + 48x48 triangle`, `1174 runs in 1.0 s`, `0 wrong pixels`; kernel timing includes non-zero TA and fragment work.
- **E1078** carries the exact GPU-produced triangle pixels into the active internal-panel simpledrm framebuffer through a verified CPU-copy bridge, then restores the original framebuffer byte-for-byte. This is visible output, but not native Asahi/DCP/KMS scanout.
- **E1086** tightens the native-presentation frontier. Independent Vulkan and Zink/Honeykrisp triangle reproductions remain pixel-perfect. GNOME Wayland runs on `card0/simpledrm`, while Honeykrisp `vkcube --wsi wayland` selects the real `Apple M3 (G15G C0)` and causes GPU work but does not complete the bounded first-present sequence; Mutter concurrently reports `Invalid sequence for VSYNC frame info`, with no GPU/DART/RTKit fault.
- **E1091** crosses native display ownership. The T8122 PMP starts with the recovered J615 request set, acknowledges display power, DCP RTKit/DCPLink and PIODMA come up, `apple-drm` registers, `eDP-1` is connected/enabled at 2880x1800@60, and `fb0` becomes `appledrmfb`. The Asahi GPU was intentionally blacklisted for this display-only proof.
- **E1092** combines that exact native-display stack with the qualified G15G runtime on one boot. `vk-render-check` completes `988 runs in 1.0 s`, `0 wrong pixels`; kernel first Render time is `168708 ns`; `apple-drm/eDP-1` remains healthy.
- **E1093** removes the E1078 CPU-copy bridge. Honeykrisp exports a full-mode linear GPU DMA-BUF; `apple-drm` PRIME-imports it and creates an XRGB8888 KMS FB. During the native commit the DCP oracle moves `swaps 7 / active_fb 42 / imported 0 -> swaps 8 / active_fb 44 / imported 1`, then restore returns to `swaps 9 / active_fb 42 / imported 0`. This is the first mechanically verified direct G15G-buffer -> native DCP/KMS presentation on J615.

The current frontier is therefore **native compositor/window-system integration and repeated synchronization/presentation qualification**. Basic GPU execution, correct Render, native DCP/KMS ownership, simultaneous GPU+display operation, PRIME import and one-buffer native presentation are closed milestones for the repaired lineage.

Historical E972/E974/E989–E1005 token5→token7 failures remain valid for the older E933/E950-style lineage and must not be erased, but they are not the current repaired-lineage wall.

After E1093 the machine was restored to Golden Linux `7.1.6-ubuntu-m3-usbpd-gc5037a961e4d`, recovery boot ID `db06411a-f5ab-450f-a799-8694ab6d9674`, exact known-good m1n1 and original GRUB, Ubuntu persistent/next boot and no pending GRUB one-shot.

Start with `docs/CANONICAL-CHECKPOINT.md`, then `docs/NEW-CHAT-HANDOFF.md` and `docs/RECENT-EXPERIMENTS.md`.
