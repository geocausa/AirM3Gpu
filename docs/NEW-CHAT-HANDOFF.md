# New-chat handoff — J615 / G15G

Generated: **2026-10-25**

**First read:** `docs/CANONICAL-CHECKPOINT.md` and `docs/E1094-AUDITED-PRESENTATION.md`.

## One-sentence state

**The physical J615 has verified G15G Compute/Render, native Apple DCP/KMS, direct GPU-produced DMA-BUF presentation, and native Wayland/Mutter presentation; the current wall is repeated FIFO/timed transaction liveness and exact buffer/fence/KMS ownership, not GPU execution.**

## Hard facts

- E1075 startup: exact T8122 HwDataA `+0x418c..+0x41db` reaches `firmware-running` and `renderD128`.
- E1076 Compute: `788 runs / 0 wrong`; first Compute `773625 ns`.
- E1077 Render: `1174 runs / 0 wrong pixels`; first Render `127875 ns`.
- E1091 native display: `apple-drm`, `eDP-1`, 2880x1800@60, `appledrmfb`.
- E1092 combined-device PASS: native display and G15G coexist; Vulkan Render `988 runs / 0 wrong pixels`.
- E1093 direct-native-present PASS: GPU DMA-BUF -> Apple DRM PRIME/AddFB2 -> DCP imported framebuffer; exact restore; no CPU pixel copy.
- E1094 native Wayland presentation PASS: Honeykrisp selects Apple M3/G15G, real GPU jobs occur, Wayland buffers are committed/released, real `wp_presentation_feedback.presented` events occur, and DCP progresses. FIFO10 completes; mailbox60 completes.
- E1095 live 4-leg `ABCA` timestamp causal bracket PASS (`runs/20261025T124557Z-b95f18fd`, boot `b95f18fd-c2f2-44cd-9a54-aeafac243186`, `RESULT.md` SHA-256 `a617c8594ba092cf01f5c4498998f6a3f7dabc27fb9da3ce49818ba2f37a9740`, `qualified=true`): proves a clean **`TIMESTAMP-PATH SPLIT`** under matched 3-image FIFO topology (`last_creation_burst_counts = A1:3 B:3 C:3 A2:3`). Both passthrough controls stall (`A1`: `13.55 s` span, `max_steady_attach_gap_ms = 10,720.039 ms`; `A2`: `12.27 s` span, `max_steady_attach_gap_ms = 10,096.041 ms`) on the exact frames where `wp_commit_timer_v1.set_timestamp` catches up to a future vblank (`target_ns - last_presented_ns = +32.8 ms`), while dropping only `wp_commit_timer_v1.set_timestamp` (`Arm B`, `53/53` dropped) finishes 60 frames in **`1,061.2 ms`** (`56.5 FPS` at 60 Hz, `max_steady_attach_gap_ms = 118.778 ms`).
- E1096 offline Mutter/DCP root-cause proof PASS: `repro-mutter-commit-timing-stall.c` dynamically loads `libmutter-clutter-18.so.0` and proves `schedule_update_target_time` `Bug A` (`0x67a3c`) and `Bug B` (`0x67ae0..0x67b40`) plus `drivers/gpu/drm/apple/dcp.c:127` (`seq = 0`) VSYNC warning/+1 sequence fallback (`vsync_warnings = A1:1 B:0 C:0 A2:0` in `E1095`) in `libmutter-18.so.0` (`on_presented` at `0x16bfa8..0x16c010`).

## Audit corrections that must travel with E1094

1. `Invalid sequence for VSYNC frame info` still occurs; do not claim it disappeared.
2. `fifo_latest_ready` was rejected and is not evidence.
3. Mailbox likely uses 4 images while FIFO uses 3 in the inspected WSI; measure actual counts before treating the mode differential as causal.
4. All-images-busy is a downstream state. Determine who owns the oldest blocked buffer/transaction before calling it a release bug.
5. Reconstructed future targets are only ~16-18 ms ahead before stalls of ~0.69 s, ~1.00 s and >=2.73 s; investigate wakeup/dependency liveness rather than a gross far-future timestamp.
6. Raw sequence=0 is not yet a proven blocking input. Separate sequence, timestamp and completion-event questions.
7. Mutter itself is still software-compositing; the proven path is a G15G client presented through Mutter, not a G15G-rendered desktop.

## Current boundary

`G15G Compute/Render -> native DCP/KMS -> direct DMA-BUF native present -> Wayland/Mutter native present -> E1095/E1096 FIFO60 timestamp-stall root-caused & live-isolated (TIMESTAMP-PATH SPLIT) -> accelerated compositor -> endurance/performance`

## Next action

1. **Wayland FIFO60 presentation pacing (closed in `E1095` + `E1096`):** Use `MESA_VK_WSI_DEBUG=fifo` / `nowlts` or apply the 2-line `ClutterFrameClock` fix in `clutter_frame_clock_schedule_update_target_time` (`Bug A` + `Bug B`, verified in `E1096` Test 4) when running timed `VK_PRESENT_MODE_FIFO_KHR` clients under Mutter 50.1.
2. **Optional Kernel KMS Enhancement (`apple-dcp`):** Supply a monotonic vblank/pageflip sequence counter in `drivers/gpu/drm/apple/dcp.c:127` (`dcp_crtc_send_page_flip_event`).
3. **Broaden to G15G compositor acceleration and endurance/performance/power/suspend-resume.**

Do **not** reopen instruction-0, token5->token7, shader, TA/fragment, generic Queue/RunCompute/UAT/PagePool/power/parser/MMIO hypotheses without contradictory evidence.

## Operational safety

`macmac` has been automatically recovered to and verified on persistent Golden (`7.1.6-ubuntu-m3-usbpd-gc5037a961e4d`, boot ID `a3342496-330a-4b77-a391-c5f1d211ac9c`), with known-good `boot.bin` (`a81993b1...`), original `custom.cfg` (`cf011eee...`), `/lib/modules/7.1.12-e1091-t8122-pmp-reordered` permissions repaired (`0755`), and the `E1091`–`E1094` kernel branch (`25da27e55b1f8ea231a55fe64fdf845ad964273c`) pushed to `geocausa/linux` (`wip/g15-e1091-t8122-pmp-reordered-devices`).
