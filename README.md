# AirM3Gpu

Experimental clean-room bring-up and reverse-engineering work for Apple M3 / T8122 J615 G15G C0 on Linux.

This repository is the canonical public research checkpoint. Compilable Linux implementation history lives in `geocausa/linux`; raw proprietary and machine-local evidence remains private.

## Current state — 2026-10-25

The physical MacBook Air M3 has crossed real Compute, graphics execution, native Apple DCP/KMS display takeover, simultaneous GPU+display operation, direct GPU-produced DMA-BUF presentation, and compositor-mediated native Wayland presentation.

- **E1075** restores the exact T8122 HwDataA `+0x418c..+0x41db` block and reaches `firmware-running` with `/dev/dri/renderD128`.
- **E1076** proves correct Vulkan Compute/readback: `788 runs in 1.0 s`, `0 wrong results`.
- **E1077** proves correct Vulkan graphics/readback: `1174 runs in 1.0 s`, `0 wrong pixels`, with non-zero TA and fragment timing.
- **E1078** carries exact GPU triangle pixels to the panel through a verified CPU-copy bridge and restores the framebuffer byte-for-byte; useful, but not native scanout.
- **E1086** localizes the old simpledrm/Wayland presentation wall. GPU execution remains correct while Mutter reports `Invalid sequence for VSYNC frame info`.
- **E1091** crosses native J615 display ownership: PMP/DCP/PIODMA come up, `apple-drm` registers, `eDP-1` is connected at 2880x1800@60, and `fb0=appledrmfb`.
- **E1092** proves native display + qualified G15G runtime coexist on one boot while Vulkan Render completes `988 runs / 0 wrong pixels`.
- **E1093** removes the CPU-copy bridge: a Honeykrisp-produced DMA-BUF is PRIME-imported into `apple-drm` and made the native DCP active framebuffer, then the original framebuffer is restored.
- **E1094** proves the next layer: Honeykrisp/G15G buffers enter native Wayland/Mutter and receive real `wl_buffer.release` and `wp_presentation_feedback.presented` events while Apple DRM/DCP advances. A 10-frame FIFO run completes; a 60-frame mailbox run completes; long FIFO runs expose a pacing/liveness defect rather than a basic GPU or native-presentation failure.
- **E1095** executes the live one-session 4-leg **A1 -> B -> C -> A2** Wayland timestamp causal bracket (`runs/20261025T124557Z-b95f18fd`, boot `b95f18fd-c2f2-44cd-9a54-aeafac243186`, `RESULT.md` SHA-256 `a617c8594ba092cf01f5c4498998f6a3f7dabc27fb9da3ce49818ba2f37a9740`) and achieves a **qualified `TIMESTAMP-PATH SPLIT` (`qualified=true`)**:
  - All 4 arms (`A1`, `B`, `C`, `A2`) create identical **`3`-image** swapchains (`500x500`, `XRGB8888`, `stride=4096`, `modifier=0x0`), ruling out swapchain image count as a confounder.
  - Both passthrough controls (`A1`, `A2`) suffer multi-second stalls (`A1`: `13,546.0 ms` span, `max_steady_attach_gap_ms = 10,720.039 ms`; `A2`: `12,268.8 ms` span, `max_steady_attach_gap_ms = 10,096.041 ms`) on the exact frames where `wp_commit_timer_v1.set_timestamp` catches up to a future vblank (`target_ns - last_presented_ns = +32.8 ms`).
  - Dropping only `wp_commit_timer_v1.set_timestamp` (`Arm B`, `53/53` filtered while keeping `wp_fifo_v1` `set=60, wait=113` and `wp_presentation` `presented=50` active) eliminates the stalls completely (`1,061.2 ms` span for 60 frames = **`56.5 FPS`**, `max_steady_attach_gap_ms = 118.778 ms` at initial resize, `0` gaps `>= 50 ms` across frames 6..59). `Arm C` (`MESA_VK_WSI_DEBUG=nowlts`) completes 60 frames in `841.7 ms` (`max_steady_attach_gap_ms = 102.728 ms`). `0` GPU/DART/RTKit/DCP faults across all 4 arms (`DCP swaps` `8 -> 291`, `failed = 0`).
- **E1096** completes an offline static and dynamic execution analysis (`repro-mutter-commit-timing-stall.c` against installed `libmutter-clutter-18.so.0` and `libmutter-18.so.0` + `drivers/gpu/drm/apple/dcp.c:127`):
  1. Proves why `Invalid sequence for VSYNC frame info` logs once per boot (`vsync_warnings = A1:1 B:0 C:0 A2:0` in `E1095`) and `wp_presentation_feedback.presented` sequences advance by `+1` across stalls: `dcp_crtc_send_page_flip_event()` (`drivers/gpu/drm/apple/dcp.c:127`) hardcodes `unsigned int seq = 0`, causing Mutter (`on_presented` at `0x16bfa8..0x16c010`) to log the warning once and fall back to `presentation_sequence += 1`.
  2. Executes `libmutter-clutter-18.so.0` offline to demonstrate two `schedule_update_target_time` state-machine defects (`Bug A` at `0x67a3c` and `Bug B` at `0x67ae0..0x67b40`) that leave `fc->has_next_presentation_time = 0` or `fc->next_presentation_time_us` at the stale presentation timestamp of the preceding frame, while confirming that repairing Bugs A & B (`Test 4 PASS`) or using untimed FIFO (`Test 2 PASS`, `E1095` Arm B) eliminates all spurious idle dispatches.

### E1094 audit corrections

Independent GPT-6.1 Sol/xhigh and GPT-6 Astra reviews re-read the raw E1094 traces and corrected the initial narrative:

- the Mutter `Invalid sequence for VSYNC frame info` warning **still occurs** in E1094, including a successful short FIFO run;
- the attempted `fifo_latest_ready` comparison was **invalid** because the environment selector was rejected;
- mailbox is not a one-variable FIFO control: the inspected Wayland WSI defaults FIFO to 3 images while true mailbox requires 4, unless the actual created counts prove otherwise;
- all-images-busy/release starvation is a downstream localization, not yet the root cause;
- the strongest current hypothesis is a timed/FIFO transaction-liveness interaction: after prior work drains, a target only about one refresh ahead can be followed by release stalls of roughly 0.69 s, 1.00 s and >=2.73 s;
- raw KMS sequence `0` remains a real metadata defect, but is not proven sufficient to cause the stalls;
- Mutter itself is still software-compositing on this path. E1094 proves GPU-client presentation through the compositor, not a fully GPU-accelerated GNOME desktop.

The FIFO60 Wayland presentation stall has now been both **offline root-caused (`E1096`)** and **live-isolated on hardware via the 4-leg `A1 -> B -> C -> A2` bracket (`E1095`, `qualified=true`, `TIMESTAMP-PATH SPLIT`)**. The next optional follow-ups are applying the 2-line `ClutterFrameClock` fix in Mutter (or using `MESA_VK_WSI_DEBUG=fifo`/`nowlts`), wiring a monotonic vblank sequence counter in `drivers/gpu/drm/apple/dcp.c:127`, and advancing to G15G compositor acceleration, endurance, performance, power and suspend/resume.

Historical E972/E974/E989-E1005 token5->token7 failures remain valid for the older failing lineage, but they are not the repaired-lineage wall.

**Operational note:** `macmac` has been automatically recovered to and verified on the persistent Golden Ubuntu kernel (`7.1.6-ubuntu-m3-usbpd-gc5037a961e4d`, boot ID `a3342496-330a-4b77-a391-c5f1d211ac9c`), with known-good `boot.bin`, original `custom.cfg`, `/lib/modules/7.1.12-e1091-t8122-pmp-reordered` permissions repaired (`0755`), and the `E1091`–`E1094` kernel branch (`25da27e55b1f8ea231a55fe64fdf845ad964273c`) pushed to `geocausa/linux` (`wip/g15-e1091-t8122-pmp-reordered-devices`).

Start with `docs/CANONICAL-CHECKPOINT.md`, then `docs/E1094-AUDITED-PRESENTATION.md`, `docs/NEW-CHAT-HANDOFF.md` and `docs/RECENT-EXPERIMENTS.md`.
