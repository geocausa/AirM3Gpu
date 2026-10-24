# New-chat handoff — J615 / G15

Generated: **2026-10-24**

**First read:** `docs/CANONICAL-CHECKPOINT.md`.

## One-sentence state

**The physical J615 now executes correct Compute and graphics under Linux, produces pixel-correct Vulkan and Zink/Honeykrisp triangles, and has already shown those GPU-produced pixels on the internal panel through a verified CPU-copy bridge; the remaining major wall is native simpledrm/DCP/KMS GPU-buffer presentation and its dma-buf/fence/vsync integration.**

## Latest hard facts

- E1075 Linux source: `geocausa/linux:wip/g15-e1075-hwdata418c @ a4778ca2a3f4791245cf1f7f42f26ccf2b17f5f7`.
- E1075 startup: exact E056-derived T8122 HwDataA `+0x418c..+0x41db` plus exact identity-capable handoff; `magic=0xabcdabcd`, `firmware-running`, render node present.
- Qualified userspace: isolated `mesa-m3-g15c 26.1.4.g15c1-2`, SHA-256 `552c94de1a98a25cc5e0dbd9e3a4cf5fdd738fb5d95689b41428f9dee3aa0936`; admission gate `ASAHI_M3_EXPERIMENTAL=1`.
- **E1076 Compute:** `65536 x 256`, 788 runs, 0 wrong; first Compute GPU time `773625 ns`.
- **E1077 canonical Render:** `64x64 clear + 48x48 triangle`, 1174 runs, 0 wrong pixels; first Render GPU time `127875 ns`, non-zero TA/fragment timing.
- **E1078 panel bridge:** exact GPU triangle readback SHA-256 `949b6175a756379bf58f27c4543a1f15a294b259c54e9a0464fe860605e309cd`; CPU-converted simpledrm panel image/readback SHA-256 `23f4bf863f03c29bcc2832c625edaadd20f2e5cb656015b494358df2415898dc`; pre/restored framebuffer SHA-256 `1b86724c5caa46dc35dfca1d5a9976e9d43810c071422facc0c28e076ee2db95`. This is not native scanout.
- **E1086 independent graphics controls:** Vulkan Render 1084 runs / 0 wrong; Zink/Honeykrisp GLES 1028 runs / 0 wrong; renderer string identifies Apple M3 G15G C0.
- **E1086 native-presentation discriminator:** GNOME Wayland runs on `card0/simpledrm`. Honeykrisp Wayland `vkcube` selects Apple M3 G15G and causes GPU activity, but the bounded present does not complete; Mutter logs `Invalid sequence for VSYNC frame info` at the corresponding time. No GPU/DART/RTKit fault occurs.
- Direct Vulkan display exposes no panel object; immediate present mode is unsupported; Xwayland lacks DRI3; Wayland EGL follows simpledrm and uses software rendering rather than Zink on the render-only card.
- E1086 private result SHA-256: `0e78deb6a85e6ca3030ff712d44e160295d065739dd987b07379b35fac4b6a54`.

## Current boundary

`correct G15G Compute + TA/fragment triangle render/readback -> GPU Wayland swapchain/job activity -> [cross-device simpledrm/render-GPU dma-buf + fence + VSYNC integration] -> native presented GPU buffer`

This is a display/presentation integration problem. The GPU execution problem is closed for the repaired lineage.

## Next action

1. Map the present display stack exactly: simpledrm ownership, native Apple DCP/KMS driver/component availability, DT nodes and why a native display DRM device has not replaced simpledrm.
2. Trace the first Honeykrisp Wayland present through format/modifier negotiation and implicit/explicit sync/fence ownership, with special attention to the Mutter VSYNC sequence failure.
3. Build one bounded native KMS/DCP GPU-buffer scanout test that avoids E1078's CPU framebuffer copy.

Only then broaden into compositor/desktop acceleration and endurance/performance work.

Do **not** reopen instruction-0, token5→token7, shader, TA/fragment, generic Queue/RunCompute/UAT/PagePool/power/parser/MMIO hypotheses without contradictory evidence.

## Historical evidence

E972/E974/E989–E1005 remain valid for the older E933/E950-style failing lineage. Keep them as lineage-specific negatives; do not reinterpret them as the current wall.

## Recovery

The Mac is back on Golden Linux `7.1.6-ubuntu-m3-usbpd-gc5037a961e4d`, boot ID `548c1cb2-a5f6-4aa4-8fe9-69a3740a14c9`, exact known-good m1n1 and original GRUB hash-verified, persistent/next boot Ubuntu, no pending one-shot entry and no experimental render node.
