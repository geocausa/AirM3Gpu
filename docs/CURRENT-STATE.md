# Current G15 Bring-up State

Research state: **2026-10-26**

Target: MacBook Air M3 J615 / T8122 / G15G C0. Exact reference: macOS 14.8.3 / 23J220.

> Canonical resume point: `docs/CANONICAL-CHECKPOINT.md`.

## Headline

**J615 GPU execution, native Apple DCP/KMS, direct/Wayland presentation, G15G-accelerated GNOME, Linux 7.3, bounded endurance, and every state in the current 338–1338 MHz firmware table are mechanically qualified. The active frontier is telemetry plus sustained/thermal, suspend/resume, performance/power and peripheral integration.**

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
- **E1097:** Native GNOME Shell is G15G-accelerated through pinned Zink/Honeykrisp; first compositor Render predates the client; FIFO60 `rc=0`; DCP `3 -> 67`, `failed=0`; Golden recovery verified.
- **E1098:** Linux 7.3 J615 candidate builds successfully from source commit `52f5d1cb8f5983131319a3083c0f1597e871a3e9` (`geocausa/linux:wip/m3-7.3-j615`) as `7.3.0-m3-j615-e1098+`.
- **E1099:** Physical J615 boots that 7.3 candidate; GNOME Shell qualifies on Honeykrisp, Render `175041 ns`, Compute `1273875 ns`, FIFO60 `rc=0`, DCP `3 -> 83`, `failed=0`, post-baseline fatal scan empty; automatic exact Golden recovery succeeds.
- **E1100:** The exact E1095 timestamp ABCA causal topology replayed on Linux 7.3 remains `qualified=true`: A1/A2 stall or time out, B drops all 53/53 commit timestamps and completes, C emits no commit timestamps and completes, all arms retain matched 3-image FIFO topology, every post-baseline fatal scan is empty, and DCP remains `failed=0`. This strongly supports kernel-lineage independence of the timed-FIFO defect.
- **E1101:** Accelerated GNOME endurance control passes on 7.3: one hardware-qualified Shell PID survives 8×600-frame `nowlts` rounds; all 8 clients rc0; DCP progresses every round with `failed=0`; all per-round/global fatal deltas are empty; exact Golden recovery succeeds.
- **E1102:** pstate 2 / 618 MHz PASS: J615 InitData caps/boots at state 2, firmware peak/actual/current state 2 is observed, 2×300-frame accelerated-GNOME clients rc0, DCP progresses with failed0, all fatal deltas empty, exact Golden recovery.
- **E1103:** pstate 3 / 796 MHz PASS: InitData cap/boot request 3, firmware peak/actual/current state 3 observed, both clients rc0, DCP progress/failed0, all fatal deltas empty, exact Golden recovery.
- **E1104:** pstate 4 / 928 MHz PASS with firmware state 4 genuinely exercised, 2×300 frames rc0, DCP progress/failed0, empty fault deltas and exact recovery.
- **E1105:** pstate 5 / 1056 MHz PASS under the same bounded envelope.
- **E1106:** explicit experiment gate widened to state 6; pstate 6 / 1170 MHz PASS. Ordinary M3 pstate policy remains unchanged.
- **E1107:** explicit gate widened to state 7; pstate 7 / 1278 MHz PASS with actual/current state 7 observed.
- **E1108:** explicit gate widened to state 8; pstate 8 / 1338 MHz PASS. Firmware repeatedly reports actual/current state 8, 2×300 frames rc0, DCP global `3 -> 635` (`failed=0`), fatal scan empty, Honeykrisp GNOME remains qualified, exact Golden recovery. State 8 is the highest state in the current live J615 table.

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

`G15G Compute/Render PASS -> native DCP/KMS PASS -> direct GPU-buffer PASS -> native Wayland PASS -> E1095/E1096 timing diagnosis PASS -> E1097 accelerated GNOME PASS -> E1099 Linux 7.3 PASS -> E1100 cross-kernel timestamp replay PASS -> E1101 bounded endurance PASS -> E1102–E1108 states 2..8 / 618..1338 MHz PASS -> telemetry + sustained/thermal qualification -> suspend/performance/power/peripherals`

Use E1093 as the direct-presentation control and E1095 Arm B (`drop-timestamps`) / Arm C (`nowlts`) as the proven 60 Hz FIFO controls. Do not reopen instruction-0, token5->token7, shader, TA/fragment, Queue/RunCompute/UAT/PagePool/power/parser/MMIO hypotheses without contradictory evidence.

## Next follow-ups

1. Establish trustworthy J615 GPU/SoC temperature and power telemetry. Use a one-shot macOS oracle boot whenever Apple sensor behavior or mapping is needed.
2. With telemetry, extend pstate-8 qualification from a short bounded pass to sustained/thermal-soak testing and characterize performance/power scaling across the proven states.
3. Qualify suspend/resume and focused peripheral regressions (Wi-Fi, USB-C/PD, storage, input, audio/backlight/battery reporting). Optional Mutter/KMS timing repairs remain orthogonal follow-ups.

## Operational state

After E1108, `macmac` is back on persistent Golden `7.1.6-ubuntu-m3-usbpd-gc5037a961e4d` (boot `ca57148f-281e-49ca-85ae-029c2798cf53`). Golden `boot.bin` SHA-256 is `a81993b122b7f990bc99ccab42decab7ef92adb94741dadf0ee79d24fdda3e0d`; original `custom.cfg` is `cf011eee536420c6b32fa5b311345f507ae260d06c7f30777e2951e683b73462`; GRUB one-shot, owner marker and E1108 service are absent. State-8 source commit `2866b256c82f5c71e5a9205f7cfcf7ab44a89460` is pushed to `geocausa/linux:wip/m3-7.3-j615`.
