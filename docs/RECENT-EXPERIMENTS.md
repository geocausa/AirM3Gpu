# Recent experiment index

Current authority: `docs/CANONICAL-CHECKPOINT.md`.

## 2026-10-24/25 — repaired-lineage progression

| Experiment | Evidence class | Result | Decision |
|---|---|---|---|
| E1075 | Same-J615 live causal pass | Exact T8122 HwDataA `+0x418c..+0x41db` crosses startup barrier; firmware-running; render node | Startup barrier crossed. |
| E1076 | Physical J615 Compute/readback | 788 runs / 0 wrong; first Compute 773625 ns | Real Compute crossed. |
| E1077 | Physical J615 Vulkan Render/readback | 1174 runs / 0 wrong pixels; first Render 127875 ns; non-zero TA/fragment | Real graphics crossed. |
| E1078 | Active-panel framebuffer bridge | Exact GPU triangle copied to simpledrm panel and framebuffer restored | Visible GPU-derived pixels, but CPU-copy only. |
| E1086 | Native-presentation differential | Vulkan/Zink remain correct; simpledrm Wayland path localizes cross-device presentation/VSYNC wall | Display/presentation problem, not GPU execution. |
| E1091 | Native J615 DCP/KMS live pass | PMP ack; DCP RTKit/DCPLink + PIODMA; `apple-drm`; `eDP-1`; `appledrmfb`; simpledrm displaced | Native display ownership crossed. |
| **E1092** | **Simultaneous native display + G15G** | Native `apple-drm/eDP-1` healthy while Vulkan Render completes 988 runs / 0 wrong; first Render 168708 ns | **Combined-device gate crossed.** |
| **E1093** | **Direct G15G DMA-BUF -> native DCP/KMS** | Linear full-mode GPU DMA-BUF offset0/pitch11520; PRIME import + AddFB2 PASS; DCP `7/fb42/imported0 -> 8/fb44/imported1 -> 9/fb42/imported0`; no CPU pixel copy | **Direct one-buffer native presentation crossed.** |

## Current interpretation

`E056 Apple HwDataA reduction -> E1075 firmware-running -> E1076 correct Compute -> E1077 correct Render/triangle -> E1078 panel-visible CPU-copy bridge -> E1086 presentation localization -> E1091 native DCP/KMS -> E1092 simultaneous GPU+display -> E1093 direct native GPU-buffer presentation`

Current hard boundary:

`G15G Render PASS + native DCP/KMS PASS + simultaneous operation PASS + direct native GPU-buffer presentation PASS -> [compositor/window-system + repeated sync/pageflip/vblank/VSYNC] -> endurance/performance`

Historical E972/E974/E989-E1005 token5->token7 results remain valid for the older failing lineage, but they are not the current repaired-lineage frontier.

Current next sequence: **bounded native compositor/window-system qualification -> repeated fence/pageflip/vblank/VSYNC presentation -> desktop integration -> endurance/performance qualification**.
