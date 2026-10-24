# Recent experiment index

Current authority: `docs/CANONICAL-CHECKPOINT.md`.

## 2026-10-24 — from semantic localization to native-presentation localization

| Experiment | Evidence class | Result | Decision |
|---|---|---|---|
| E972 | Offline exact Xcode16.3 replay | Apple/Linux older-lineage streams match through mode-1/raw-1 token5; Apple then emits token7 with initial outer-ESL PC, Linux does not | Historical failing-lineage semantic boundary. |
| E974 | Exact Xcode16.3 static semantics | Parser state `0x61` = ESL slot 1 | Close hidden-hardware-state interpretation. |
| E989-E1005 | Apple/Linux state-op controls | Working state operations share token5→token7→token6; older Linux remains token5-only | Historical failure is generic state-operation admission, not RLD-specific. |
| **E1075** | **Same-J615 live causal pass** | Exact E056-derived HwDataA `+0x418c..+0x41db` crosses first DeviceControl barrier; `magic=0xabcdabcd`; firmware-running; render node | **Startup barrier crossed.** |
| **E1076** | **Physical J615 Compute/readback** | `65536 x 256`, 788 runs, 0 wrong; kernel first Compute `773625 ns` | **Real Compute crossed.** |
| **E1077** | **Physical J615 Vulkan Render/readback** | `64x64 clear + 48x48 triangle`; 1174 runs, 0 wrong pixels; first Render `127875 ns`; non-zero TA/fragment | **Real graphics and triangle correctness crossed.** |
| E1078-A | Direct-display discriminator | Qualified Vulkan direct-display sees no display object | Render GPU does not own panel/KMS in this boot. |
| E1078-B | Vulkan map-boundary instrumentation | Passing render remains 1258 runs / 0 wrong; exact 64x64 RGBA8 GPU readback captured | Exact GPU triangle buffer identified. |
| **E1078-C** | **Active-panel framebuffer bridge** | Exact GPU triangle CPU-converted to panel format and copied to `/dev/fb0`; intended/readback hashes match; original framebuffer restored | **Visible-pixel bridge crossed.** Not native scanout. |
| E1086-A | Independent Render reproduction | Vulkan triangle 1084 runs / 0 wrong; first Render `127708 ns`; no GPU fault | Confirms repaired-lineage graphics remains stable. |
| E1086-B | Zink/Honeykrisp graphics control | GLES-through-Zink triangle 1028 runs / 0 wrong; renderer is Apple M3 G15G Honeykrisp | Zink graphics path itself works. |
| E1086-C | GNOME/simpledrm topology | GNOME Wayland runs on `card0/simpledrm`; old system Mesa is rejected on separate G15G render device by 592-vs-600 GET_PARAMS gate | Compositor/display device and render GPU are split. |
| E1086-D | Honeykrisp Wayland present | `vkcube --wsi wayland` selects Apple M3 G15G and causes GPU activity, but bounded present does not complete; Mutter logs `Invalid sequence for VSYNC frame info`; no GPU/DART/RTKit fault | **Current native-present boundary is cross-device dma-buf/fence/vsync integration.** |
| E1086-E | WSI controls | Direct-display has no display; immediate mode unsupported; Xwayland has no DRI3; Wayland EGL on simpledrm remains software even with client Zink/PRIME overrides | Close these as basic bypass routes; investigate native DCP/KMS + sync path. |

## Current interpretation

The repaired-lineage progression is:

`E056 Apple HwDataA reduction -> E1075 firmware-running -> E1076 correct Compute -> E1077 correct Render/triangle -> E1078 panel-visible CPU-copy bridge -> E1086 native-present boundary`

Current hard boundary:

`correct GPU render -> Honeykrisp Wayland GPU activity -> [simpledrm/render-GPU dma-buf + fence + VSYNC integration] -> native presented GPU buffer`

The older E972 token5→token7 boundary remains valuable historical evidence for the older failing lineage, but it is not the current wall.

## Current next sequence

**map native DCP/KMS ownership -> trace first-present formats/modifiers/fences/VSYNC -> bounded GPU-buffer scanout -> compositor/desktop integration -> endurance/performance qualification**.

Do not return to broad GPU frontend searches without contradictory evidence.

Historical pre-E718 experiments remain archived in `docs/HISTORICAL-EXPERIMENTS-THROUGH-E717.md`.
