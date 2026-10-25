# Recent experiment index

Current authority: `docs/CANONICAL-CHECKPOINT.md`.

## 2026-10-24/25 — repaired-lineage progression

| Experiment | Evidence class | Result | Decision |
|---|---|---|---|
| E1075 | Same-J615 live causal pass | Exact T8122 HwDataA `+0x418c..+0x41db` crosses startup barrier; firmware-running; render node | Startup barrier crossed. |
| E1076 | Physical J615 Compute/readback | 788 runs / 0 wrong; first Compute 773625 ns | Real Compute crossed. |
| E1077 | Physical J615 Vulkan Render/readback | 1174 runs / 0 wrong pixels; first Render 127875 ns; non-zero TA/fragment | Real graphics crossed. |
| E1078 | Active-panel framebuffer bridge | Exact GPU triangle copied to simpledrm panel and framebuffer restored | Visible GPU-derived pixels, CPU-copy only. |
| E1086 | Presentation differential | Vulkan/Zink remain correct; simpledrm Wayland localizes presentation/VSYNC wall | Display/presentation problem, not GPU execution. |
| E1091 | Native J615 DCP/KMS live pass | PMP/DCP/PIODMA; `apple-drm`; `eDP-1`; `appledrmfb`; simpledrm displaced | Native display ownership crossed. |
| **E1092** | **Simultaneous native display + G15G** | Native `apple-drm/eDP-1` healthy while Vulkan Render completes 988 runs / 0 wrong | **Combined-device gate crossed.** |
| **E1093** | **Direct G15G DMA-BUF -> native DCP/KMS** | PRIME import + AddFB2; DCP becomes imported fb44 then restores fb42; no CPU pixel copy | **Direct native GPU-buffer presentation crossed.** |
| **E1094** | **Native Wayland/Mutter presentation** | Real GPU client buffers, releases and `presented` feedback through Mutter/native Apple DRM/DCP; FIFO10 completes; mailbox60 completes | **Compositor-mediated native presentation crossed; long FIFO pacing remains.** |
| **E1095** | **Live 4-leg ABCA Wayland timestamp causal bracket** | `runs/20261025T124557Z-b95f18fd` (`qualified=true`): matched 3-image FIFO across all arms; `A1`/`A2` stall (`10.72 s` / `10.10 s` max gap) on future `set_timestamp`; `B` (`drop-timestamps`, `53/53`) finishes 60 frames in `1.06 s` (`56.5 FPS`) | **`TIMESTAMP-PATH SPLIT` proven live on hardware.** |
| **E1096** | **Offline Mutter 50.1 & DCP vblank root-cause proof** | `repro-mutter-commit-timing-stall` against `libmutter-clutter-18.so.0` proves `schedule_update_target_time` Bugs A & B + `dcp.c:127` `seq=0` fallback | **Root causes of `wp_commit_timer_v1` stall and VSYNC sequence warning proven offline.** |
| **E1097** | **Native GNOME G15G compositor live qualification** | Hardware Zink/Honeykrisp compositor contexts; first Render before client; FIFO60 rc0; DCP swaps 3->67, failed0; Golden recovery | **Accelerated GNOME compositor crossed.** |
| **E1098** | **Linux 7.3 J615 port/build** | Source commit `52f5d1cb...` builds `7.3.0-m3-j615-e1098+` with J615 DTB, Asahi, Apple DRM and PMP artifacts | **7.3 candidate built and packaged.** |
| **E1099** | **Physical Linux 7.3 first boot + bounded desktop/GPU test** | Boots 7.3; Honeykrisp GNOME; Render/Compute complete; FIFO60 rc0; DCP 3->83 failed0; post-baseline fatal scan empty; Golden auto-recovery | **Linux 7.3 J615 live gate crossed.** |

## E1094 audited correction set

The initial E1094 interpretation was independently audited against raw traces. Canonical corrections:

- Mutter's `Invalid sequence for VSYNC frame info` warning persists in E1094; the original negative search used a failed journal capture.
- `fifo_latest_ready` is not a valid result because the selector was rejected.
- mailbox and FIFO are not presently a single-variable comparison: inspected WSI policy uses 4 images for true mailbox and 3 for FIFO unless measured creation counts say otherwise.
- long FIFO waits are release/image-availability stalls at the client boundary, but exact server-side ownership remains unproven.
- targets immediately before the long pauses are only about one refresh ahead; the pauses are orders of magnitude longer.
- sequence-zero is a metadata defect, not yet a demonstrated blocking cause.

## Current interpretation

`E056 HwDataA reduction -> E1075 firmware-running -> E1076 Compute -> E1077 Render -> E1091 native DCP/KMS -> E1093 direct native presentation -> E1094 Wayland presentation -> E1095/E1096 timing diagnosis -> E1097 accelerated GNOME compositor -> E1098 Linux 7.3 port/build -> E1099 Linux 7.3 live qualification`

Current hard boundary:

`Linux 7.3 J615 boot + native GNOME/Honeykrisp + Render/Compute/FIFO60 + DCP progress PASS -> endurance/performance/power/suspend-resume/peripheral integration`

Historical E972/E974/E989-E1005 token5->token7 results remain valid for the older failing lineage but are not the repaired-lineage frontier.
