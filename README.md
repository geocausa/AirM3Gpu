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
- **E1095** builds and offline-qualifies the transparent Wayland timestamp-filter proxy (`wlcommit-proxy`) and one-session 4-leg **A/B/C/A** isolator (`run-e1095-abca.sh`: FIFO passthrough -> drop only `wp_commit_timer_v1.set_timestamp` -> passthrough + `MESA_VK_WSI_DEBUG=nowlts` -> passthrough again).
- **E1096** completes an offline static and dynamic execution analysis (`repro-mutter-commit-timing-stall.c` against installed `libmutter-clutter-18.so.0` and `libmutter-18.so.0` + `drivers/gpu/drm/apple/dcp.c:127`):
  1. Proves why `Invalid sequence for VSYNC frame info` logs once per boot and `wp_presentation_feedback.presented` sequences advance by `+1` across 700 ms stalls: `dcp_crtc_send_page_flip_event()` (`drivers/gpu/drm/apple/dcp.c:127`) hardcodes `unsigned int seq = 0`, causing Mutter (`on_presented` at `0x16bfa8..0x16c010`) to log the warning once and fall back to `presentation_sequence += 1`.
  2. Reconstructs the `E1094` `VKCUBE.txt` timeline (showing 100% of frames whose `wp_commit_timer_v1.set_timestamp` target is still in the future when `"after-update"` clears `surface->fifo_barrier` — Frames 9, 12, and 15 — stall immediately) and executes `libmutter-clutter-18.so.0` offline to demonstrate two `schedule_update_target_time` state-machine defects (`Bug A`: setting `pending_reschedule_now = 1` when `state == DISPATCHED_ONE`, which clears `has_next_presentation_time = 0`; `Bug B`: transitioning `IDLE -> SCHEDULED_LATER` in fixed-refresh mode without setting `has_next_presentation_time = 1`), while untimed FIFO (`10/10` frames at 60 Hz) avoids both defects completely.

### E1094 audit corrections

Independent GPT-6.1 Sol/xhigh and GPT-6 Astra reviews re-read the raw E1094 traces and corrected the initial narrative:

- the Mutter `Invalid sequence for VSYNC frame info` warning **still occurs** in E1094, including a successful short FIFO run;
- the attempted `fifo_latest_ready` comparison was **invalid** because the environment selector was rejected;
- mailbox is not a one-variable FIFO control: the inspected Wayland WSI defaults FIFO to 3 images while true mailbox requires 4, unless the actual created counts prove otherwise;
- all-images-busy/release starvation is a downstream localization, not yet the root cause;
- the strongest current hypothesis is a timed/FIFO transaction-liveness interaction: after prior work drains, a target only about one refresh ahead can be followed by release stalls of roughly 0.69 s, 1.00 s and >=2.73 s;
- raw KMS sequence `0` remains a real metadata defect, but is not proven sufficient to cause the stalls;
- Mutter itself is still software-compositing on this path. E1094 proves GPU-client presentation through the compositor, not a fully GPU-accelerated GNOME desktop.

The current technical frontier is therefore **FIFO/timed Wayland transaction liveness, exact buffer/dependency ownership, and KMS/frame-clock synchronization**, followed by compositor acceleration, endurance, performance, power and suspend/resume.

The preferred next discriminator is an offline-qualified transparent Wayland proxy and one-session **A/B/C/A** comparison: FIFO passthrough -> drop only `wp_commit_timer_v1.set_timestamp` -> passthrough + Mesa `nowlts` -> passthrough again. Actual image count/format/modifier/generation, releases, feedback, API state, journal cursor and DCP state must be captured for every arm.

Historical E972/E974/E989-E1005 token5->token7 failures remain valid for the older failing lineage, but they are not the repaired-lineage wall.

**Operational note:** `macmac` has been recovered to and verified on the persistent Golden Ubuntu kernel (`7.1.6-ubuntu-m3-usbpd-gc5037a961e4d`, boot ID `e5ac1a80-e174-4560-b1cd-f393e384b883`), with known-good `boot.bin`, original `custom.cfg`, `/lib/modules/7.1.12-e1091-t8122-pmp-reordered` permissions repaired (`0755`), and the `E1091`–`E1094` kernel branch (`25da27e55b1f8ea231a55fe64fdf845ad964273c`) pushed to `geocausa/linux` (`wip/g15-e1091-t8122-pmp-reordered-devices`).

Start with `docs/CANONICAL-CHECKPOINT.md`, then `docs/E1094-AUDITED-PRESENTATION.md`, `docs/NEW-CHAT-HANDOFF.md` and `docs/RECENT-EXPERIMENTS.md`.
