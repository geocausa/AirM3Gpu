# Current G15 Bring-up State

Research state: **2026-10-24**

Target: MacBook Air M3 J615 / T8122 / G15G C0. Exact reference: macOS 14.8.3 / 23J220.

> Canonical resume point: `docs/CANONICAL-CHECKPOINT.md`.

## Headline

**J615 basic GPU execution is solved on the repaired E1075 lineage; native presentation is now the frontier.**

E1075 restores the exact E056-derived T8122 HwDataA `+0x418c..+0x41db` image and reaches `firmware-running` with `/dev/dri/renderD128`.

E1076 proves correct Compute/readback:

`65536 invocations x 256 steps; 788 runs in 1.0 s; 0 wrong results`

E1077 proves canonical correct Vulkan Render/triangle readback:

`64x64 clear + 48x48 triangle; 1174 runs in 1.0 s; 0 wrong pixels`

Kernel first Render completion is `127875 ns` GPU time with non-zero TA and fragment timing.

E1078 captures the exact GPU-produced 64x64 RGBA8 triangle and bridges it by CPU copy into the active 2880x1800 `x2r10g10b10` simpledrm framebuffer. Intended/displayed framebuffer SHA-256 is `23f4bf863f03c29bcc2832c625edaadd20f2e5cb656015b494358df2415898dc`; the original framebuffer is restored byte-for-byte. This is visible output but **not native Asahi/DCP/KMS scanout**.

## E1086 — presentation localization

E1086 independently reproduces correct graphics and then probes native presentation:

- Vulkan Render reproduction: `1084 runs / 0 wrong pixels`; first Render GPU time `127708 ns`.
- Zink/Honeykrisp GLES reproduction: `1028 runs / 0 wrong pixels`; renderer `zink Vulkan 1.4(Apple M3 (G15G C0) (MESA_HONEYKRISP))`.
- `vkcube --wsi display`: G15G admitted, then `Cannot find any display!`.
- GNOME Wayland runs on `card0/simpledrm`; old system Mesa is rejected on the separate G15G render device by the 592-vs-600-byte GET_PARAMS gate.
- Wayland EGL follows simpledrm and uses softpipe even with the isolated Mesa prefix, Zink override and `DRI_PRIME=1`.
- Honeykrisp `vkcube --wsi wayland --c 60` selects the real Apple M3 G15G and causes GPU work, but the 10-second bound expires; Mutter concurrently logs `Invalid sequence for VSYNC frame info`.
- No GPU, DART or RTKit fault occurs.
- Immediate present mode is unsupported; Xwayland has no DRI3.

Current boundary:

`correct Compute + TA/fragment Render -> GPU Wayland swapchain/job activity -> [simpledrm/render-GPU dma-buf/fence/vsync integration] -> native presented GPU buffer`

Private E1086 result SHA-256: `0e78deb6a85e6ca3030ff712d44e160295d065739dd987b07379b35fac4b6a54`.

## Next work

Map native Apple DCP/KMS/display ownership, then trace the first-present dma-buf/modifier/fence/vsync path and build one bounded direct GPU-buffer scanout discriminator. Do not modify GPU command construction merely to solve presentation.

Do not return to broad instruction-0, token5→token7, Queue/RunCompute/UAT/PagePool/power/parser/MMIO searches without contradictory evidence.

## Historical boundary retained

E972/E974/E989–E1005 remain valid for the older failing E933/E950-style lineage. Preserve those results, but do not present them as the current E1075/E1086 frontier.

## Safety state

After E1086, Fabric reconciled Golden Linux `7.1.6-ubuntu-m3-usbpd-gc5037a961e4d`, boot ID `548c1cb2-a5f6-4aa4-8fe9-69a3740a14c9`, known-good m1n1 SHA `a81993b...`, original GRUB SHA `cf011eee...`, Ubuntu persistent/next boot, no pending one-shot entry and no experimental render node.
