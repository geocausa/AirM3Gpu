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

## Audit corrections that must travel with E1094

1. `Invalid sequence for VSYNC frame info` still occurs; do not claim it disappeared.
2. `fifo_latest_ready` was rejected and is not evidence.
3. Mailbox likely uses 4 images while FIFO uses 3 in the inspected WSI; measure actual counts before treating the mode differential as causal.
4. All-images-busy is a downstream state. Determine who owns the oldest blocked buffer/transaction before calling it a release bug.
5. Reconstructed future targets are only ~16-18 ms ahead before stalls of ~0.69 s, ~1.00 s and >=2.73 s; investigate wakeup/dependency liveness rather than a gross far-future timestamp.
6. Raw sequence=0 is not yet a proven blocking input. Separate sequence, timestamp and completion-event questions.
7. Mutter itself is still software-compositing; the proven path is a G15G client presented through Mutter, not a G15G-rendered desktop.

## Current boundary

`G15G Compute/Render -> native DCP/KMS -> direct DMA-BUF native present -> Wayland/Mutter native present -> [FIFO/timed liveness + exact ownership/sync/KMS dependency] -> accelerated compositor -> endurance/performance`

## Next action

1. **First restore and verify exact Golden.** Current host is still on the E1094 experimental kernel.
2. Offline-build and prequalify a transparent Wayland proxy that forwards bytes/FDs/object lifetimes unchanged and can drop only `wp_commit_timer_v1.set_timestamp` for the test surface.
3. Re-arm the qualified E1094 stack only after Golden is clean.
4. In one compositor session run FIFO60 A/B/C/A: passthrough / timestamp-only removal / passthrough+`nowlts` / passthrough. Require both A controls to reproduce.
5. Record requested/effective mode, actual image count, format/modifier, generation, API/image state, releases/presented/discarded, valid journal cursor and DCP state.
6. If a long wait reproduces, identify the oldest Mutter dependency across timed/FIFO state, reservation fence, composition use, KMS submit/event/frame-clock, and release/flush/receipt. Stop once one complete dependency-to-release chain is known.

Do **not** reopen instruction-0, token5->token7, shader, TA/fragment, generic Queue/RunCompute/UAT/PagePool/power/parser/MMIO hypotheses without contradictory evidence.

## Operational safety

Historical E1093 Golden recovery was verified, but it is no longer the current live state. At this checkpoint `macmac` remains on experimental kernel `7.1.12-e1091-t8122-pmp-reordered`; Golden recovery is pending. Do not call the host Golden until boot.bin/custom.cfg, GRUB, bless targets, owner marker and kernel are all re-verified.
