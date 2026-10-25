# New-chat handoff — J615 / G15G

Generated: **2026-10-25**

**First read:** `docs/CANONICAL-CHECKPOINT.md` and `docs/E1094-AUDITED-PRESENTATION.md`.

## One-sentence state

**The physical J615 now boots the repaired stack on Linux 7.3 and passes bounded native GNOME/Honeykrisp compositor, Render, Compute, FIFO60 and DCP progression with automatic Golden recovery; the frontier is longer stability, power, suspend/resume and peripheral integration.**

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
- E1097 accelerated compositor PASS: GNOME Shell itself uses pinned Zink/Honeykrisp Apple M3, first compositor Render predates the client, FIFO60 rc0, DCP 3->67 failed0, then Golden recovery.
- E1098 Linux 7.3 port/build PASS: `7.3.0-m3-j615-e1098+`, source `52f5d1cb8f5983131319a3083c0f1597e871a3e9`, pushed as `geocausa/linux:wip/m3-7.3-j615`.
- E1099 Linux 7.3 live PASS: physical boot succeeds; GNOME Honeykrisp hardware contexts; Render 175041 ns; Compute 1273875 ns; FIFO60 rc0; DCP swaps 3->83 failed0; no new post-baseline fatal signature; exact Golden recovery.

## Audit corrections that must travel with E1094

1. `Invalid sequence for VSYNC frame info` still occurs; do not claim it disappeared.
2. `fifo_latest_ready` was rejected and is not evidence.
3. Mailbox likely uses 4 images while FIFO uses 3 in the inspected WSI; measure actual counts before treating the mode differential as causal.
4. All-images-busy is a downstream state. Determine who owns the oldest blocked buffer/transaction before calling it a release bug.
5. Reconstructed future targets are only ~16-18 ms ahead before stalls of ~0.69 s, ~1.00 s and >=2.73 s; investigate wakeup/dependency liveness rather than a gross far-future timestamp.
6. Raw sequence=0 is not yet a proven blocking input. Separate sequence, timestamp and completion-event questions.
7. Mutter was still software-compositing in E1094; E1097 later supersedes this historical limitation by qualifying GNOME Shell itself on G15G/Zink/Honeykrisp.

## Current boundary

`Linux 7.3 J615 live PASS -> longer endurance/performance/power/suspend-resume/peripheral qualification`

## Next action

1. Extend E1099 on Linux 7.3 into a longer bounded desktop/GPU endurance run with fault/latency capture.
2. Qualify suspend/resume, power management, peripherals and higher GPU pstates before persistent daily-driver use.
3. Optionally land the already-proven Mutter frame-clock repair and monotonic apple-dcp vblank/pageflip sequence instead of relying on `nowlts`.

Do **not** reopen instruction-0, token5->token7, shader, TA/fragment, generic Queue/RunCompute/UAT/PagePool/power/parser/MMIO hypotheses without contradictory evidence.

## Operational safety

After E1099 the machine is verified back on Golden `7.1.6-ubuntu-m3-usbpd-gc5037a961e4d`; `boot.bin=a81993b1...`, `custom.cfg=cf011eee...`, GRUB one-shot empty, E1099 service/owner absent. The 7.3 source is off-machine on `geocausa/linux:wip/m3-7.3-j615` at `52f5d1cb8f5983131319a3083c0f1597e871a3e9`.
