# Current G15 Bring-up State

Research state: **2026-10-25**

Target: MacBook Air M3 J615 / T8122 / G15G C0. Exact reference: macOS 14.8.3 / 23J220.

> Canonical resume point: `docs/CANONICAL-CHECKPOINT.md`.

## Headline

**J615 GPU execution, native Apple DCP/KMS, direct GPU-buffer presentation, and compositor-mediated native Wayland presentation are mechanically proven. The current defect is repeated FIFO/timed presentation liveness, not basic GPU execution or native scanout.**

## Closed repaired-lineage milestones

- **E1075:** exact T8122 HwDataA `+0x418c..+0x41db` restores `firmware-running` and the render node.
- **E1076:** real Vulkan Compute/readback, `788 runs / 0 wrong`; first Compute `773625 ns`.
- **E1077:** real Vulkan Render/readback, `1174 runs / 0 wrong pixels`; first Render `127875 ns`; non-zero TA/fragment timing.
- **E1078:** GPU triangle reaches the panel through a verified CPU-copy bridge; not native scanout.
- **E1086:** simpledrm/Wayland presentation wall localized; GPU execution remains correct.
- **E1091:** native J615 Apple DCP/KMS takeover: `apple-drm`, `eDP-1`, 2880x1800@60, `fb0=appledrmfb`.
- **E1092:** native display and qualified G15G runtime coexist; Vulkan Render `988 runs / 0 wrong pixels`.
- **E1093:** Honeykrisp full-mode linear DMA-BUF -> `apple-drm` PRIME import/AddFB2 -> synchronous native DCP commit; DCP oracle changes to the imported framebuffer and restores exactly; no CPU pixel copy.
- **E1094:** Honeykrisp/G15G -> Wayland -> Mutter -> native Apple DRM/DCP presentation is proven by real buffer releases, presentation feedback, GPU Render activity and DCP progression. FIFO 10-frame completes; mailbox 60-frame completes; long FIFO runs expose intermittent release/pacing stalls.
- **E1095:** Live one-session 4-leg `A1 -> B -> C -> A2` Wayland timestamp causal bracket (`runs/20261025T124557Z-b95f18fd`, boot `b95f18fd-c2f2-44cd-9a54-aeafac243186`, `RESULT.md` SHA-256 `a617c8594ba092cf01f5c4498998f6a3f7dabc27fb9da3ce49818ba2f37a9740`, `qualified=true`) achieves a clean **`TIMESTAMP-PATH SPLIT`**: all 4 arms allocate identical **`3`-image** swapchains (`500x500`, `XRGB8888`, `stride=4096`, `modifier=0x0`), both passthrough controls (`A1`: `13.55 s`, `max_steady_attach_gap_ms = 10,720.039 ms`; `A2`: `12.27 s`, `max_steady_attach_gap_ms = 10,096.041 ms`) stall whenever `wp_commit_timer_v1.set_timestamp` catches up to a future vblank, and dropping only `wp_commit_timer_v1.set_timestamp` (`Arm B`, `53/53` dropped while keeping `wp_fifo_v1` `set=60, wait=113` and `wp_presentation` `presented=50` intact) eliminates the stalls completely (`1.06 s` for 60 frames = **`56.5 FPS`**, `max_steady_attach_gap_ms = 118.778 ms`).
- **E1096:** Offline binary + dynamic execution harness (`repro-mutter-commit-timing-stall.c`) against installed `libmutter-clutter-18.so.0` and `libmutter-18.so.0` proves the `drivers/gpu/drm/apple/dcp.c:127` `seq = 0` VSYNC warning/+1 sequence fallback (`vsync_warnings = A1:1 B:0 C:0 A2:0` in `E1095`) and reproduces two `ClutterFrameClock` `schedule_update_target_time` state-machine defects (`Bug A` at `0x67a3c` and `Bug B` at `0x67ae0..0x67b40`) when `wp_commit_timer_v1` is used alongside `wp_fifo_v1`, while confirming that untimed FIFO (`Test 2 PASS`) and repairing Bugs A & B (`Test 4 PASS`) eliminate all spurious idle dispatches.

E1092 private result SHA-256: `7d303dfa0e8e563fcefcc31fd2413be82ea07b0baa52c68138de5590f8151d0c`.

E1093 private result SHA-256: `1180520285949fb309aa2ca099ae48e5ebce6e014715ad308b1645c457ecac38`.

## E1094 audited facts

- `Invalid sequence for VSYNC frame info` persists; its absence in the first summary was caused by a failed journal collection path.
- The `fifo_latest_ready` test is invalid and must not be used as evidence.
- Mailbox vs FIFO changes more than present mode in the inspected WSI: default FIFO depth is 3 images; true mailbox requires 4 unless observed counts show otherwise.
- The traced long waits correlate with delayed `wl_buffer.release`; three current images can become unavailable.
- Reconstructed next targets are only ~16-18 ms beyond the previous presentation, yet observed waits are about 0.69 s, 1.00 s and at least 2.73 s. Grossly far-future targets therefore do not explain the stalls.
- Sequence-zero metadata is real but not mechanically linked to the blocking predicate. Event delivery, timestamp quality and sequence validity must be treated separately.
- Mutter is software-compositing in this qualification path. A GPU client is presented through Mutter; Mutter itself is not yet G15G-accelerated.

## Current boundary

`G15G Compute/Render PASS -> native DCP/KMS PASS -> direct imported GPU-buffer PASS -> native Wayland/Mutter presentation PASS -> E1095/E1096 FIFO60 timestamp-stall root-caused & live-isolated (TIMESTAMP-PATH SPLIT) -> G15G compositor acceleration -> endurance/performance/power/suspend-resume`

Use E1093 as the direct-presentation control and E1095 Arm B (`drop-timestamps`) / Arm C (`nowlts`) as the proven 60 Hz FIFO controls. Do not reopen instruction-0, token5->token7, shader, TA/fragment, Queue/RunCompute/UAT/PagePool/power/parser/MMIO hypotheses without contradictory evidence.

## Next follow-ups

1. Apply the 2-line `ClutterFrameClock` `schedule_update_target_time` fix in Mutter (or use `MESA_VK_WSI_DEBUG=fifo` / `nowlts`) for timed `VK_PRESENT_MODE_FIFO_KHR` clients under Mutter 50.1, and optionally wire a monotonic vblank sequence counter in `drivers/gpu/drm/apple/dcp.c:127`.
2. Advance to G15G compositor acceleration and endurance/performance/power/suspend-resume.

## Operational state

`macmac` has been automatically recovered to and verified on the persistent Golden Ubuntu kernel (`7.1.6-ubuntu-m3-usbpd-gc5037a961e4d`, boot ID `a3342496-330a-4b77-a391-c5f1d211ac9c`), with known-good `boot.bin` (`a81993b1...`), original `custom.cfg` (`cf011eee...`), `/lib/modules/7.1.12-e1091-t8122-pmp-reordered` permissions repaired (`0755`), and the compilable `E1091`–`E1094` kernel branch (`25da27e55b1f8ea231a55fe64fdf845ad964273c`) pushed to `geocausa/linux` (`wip/g15-e1091-t8122-pmp-reordered-devices`).
