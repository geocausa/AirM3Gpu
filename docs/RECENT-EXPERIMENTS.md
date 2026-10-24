# Recent experiment index

Current authority: `docs/CANONICAL-CHECKPOINT.md`.

## 2026-10-24 — from semantic localization to real J615 graphics

| Experiment | Evidence class | Result | Decision |
|---|---|---|---|
| E972 | Offline exact Xcode16.3 replay | Apple/Linux same-program streams match semantically through mode-1/raw-1 token5; Apple then emits token7 with initial outer-ESL PC, Linux never emits token7 | Historical failing-lineage first semantic divergence is `token5 -> token7`. |
| E974 | Exact Xcode16.3 static semantics | Parser state `0x61` = ESL slot 1 in reserved `0x60..0x67` bank | Close “hardware state 0x61” interpretation. |
| E989-E1005 | Apple/Linux state-op controls | Working RLD/LoadDev and valid LoadShader share token5→token7→token6; older Linux lineage remains token5-only | Old failure is generic state-operation admission/commit, not RLD-specific. |
| E1022-E1044 | Independent Aurora/T8122 reconstruction | Reconstructs a second runtime/boot lineage and exposes concrete startup faults | Opens current J615 runtime path. |
| E1045-E1072 | Startup discriminators/oracles | MTR, HwData, IO-map, DeviceControl and Apple-oracle work localize startup; many clean negatives | Preserve exact corrections; do not reopen closed hypotheses. |
| **E1075** | **Same-J615 live causal pass** | Restore exact E056-derived T8122 HwDataA `+0x418c..+0x41db`; DeviceControl is consumed; `magic=0xabcdabcd`; firmware-running; DRM render node appears | **Startup barrier crossed.** |
| **E1076** | **Physical J615 live Compute/readback** | Isolated G15G Honeykrisp admits T8122. `65536 x 256`, 788 runs, 0 wrong; kernel reports first Compute completed in 773625 ns | **Real Compute and readback crossed.** |
| **E1077** | **Physical J615 live Vulkan Render/readback** | `64x64 clear + 48x48 triangle`; 1174 runs in 1.0 s; 0 wrong pixels. Kernel reports first Render completed in 127875 ns with non-zero TA and fragment timing | **Real graphics and triangle pixel correctness crossed.** |
| E1078-A | Direct-display WSI discriminator | Qualified `vkcube --wsi display --c 1` prints `Cannot find any display!`; no GPU job submitted | Display is not owned by Asahi/KMS on this boot; simpledrm and render device are separate. |
| E1078-B | Vulkan map-boundary instrumentation | Passing render checker remains 1258 runs / 0 wrong; exact 16384-byte 64x64 RGBA8 GPU readback captured | Exact GPU-produced triangle buffer identified without rewriting renderer. |
| **E1078-C** | **Active-panel framebuffer bridge** | Exact GPU triangle converted to panel `x2r10g10b10`, written to `/dev/fb0`; intended/readback hashes match exactly; original framebuffer restored exactly | **Visible-pixel bridge crossed.** Explicitly not native DCP/KMS scanout; CPU copy bridges GPU readback to simpledrm. |

## Current interpretation

The causal progression is now:

`E056 Apple HwDataA reduction -> E1075 firmware-running -> E1076 correct Compute -> E1077 correct Render/triangle -> E1078 active-panel framebuffer bridge`

The older E972 token5→token7 boundary remains important historical evidence for the older failing lineage, but it is not the current repaired-lineage wall.

## Current next sequence

**native DCP/KMS/display ownership -> GPU-buffer presentation -> compositor/desktop integration -> endurance/performance qualification**.

Do not return to broad GPU frontend searches unless contradictory evidence appears.

Historical pre-E718 experiments remain archived in `docs/HISTORICAL-EXPERIMENTS-THROUGH-E717.md`.
