# Canonical project checkpoint — J615 / G15

Updated: **2026-10-26**

This file is the single resume point for the AirM3Gpu project. Older “current frontier” statements are historical only.

## Target and authority

- Hardware: MacBook Air M3, J615 / T8122, GPU G15G C0, 10/10 cores.
- Exact ABI authority: macOS 14.8.3 / 23J220 plus matching retained G15G C0 components.
- Persistent safety baseline: Golden Linux `7.1.6-ubuntu-m3-usbpd-gc5037a961e4d`.
- Raw captures, proprietary binaries and machine-specific evidence remain private.

## 1. Proven repaired-lineage GPU milestones

Qualified runtime:

- Linux source: `geocausa/linux:wip/g15-e1075-hwdata418c @ a4778ca2a3f4791245cf1f7f42f26ccf2b17f5f7`.
- Experimental kernel: `7.1.12-e1075-hwdata418c-ga4778ca2a3f4`.
- E1075 startup correction: exact Apple-derived T8122 HwDataA `+0x418c..+0x41db` 20-u32 image originally recovered in E056.
- Exact E1043-D identity-capable boot handoff used for live qualification.
- Firmware reaches `magic=0xabcdabcd runtime=1 newA=1/1 newB=0`, reports `firmware-running`, and exposes `/dev/dri/renderD128`.
- Runtime capped at pstate 1 / 338 MHz.

Qualified userspace:

- System Mesa is too old for the 600-byte M3-aware GET_PARAMS ABI.
- Retained ordinary `mesa-m3` is M3-aware but lacks the G15 command stream.
- Isolated Aurora `mesa-m3-g15c 26.1.4.g15c1-2`, package SHA-256 `552c94de1a98a25cc5e0dbd9e3a4cf5fdd738fb5d95689b41428f9dee3aa0936`, contains the working G15G/AGX3 path.
- With `ASAHI_M3_EXPERIMENTAL=1`, it admits chip `0x8122`, 1 cluster, 10 cores and identifies `Apple M3 (G15G C0)`.

### E1076 — real Compute/readback

`65536 invocations x 256 steps; 788 runs in 1.0 s; 0 wrong results`

Kernel independently reports first Compute completion, `773625 ns` GPU time. This crosses command construction, VM/queue submission, firmware scheduling, shader execution, retirement and CPU-visible readback.

### E1077 — real Render + triangle readback

Canonical bounded result:

`vk-render-check: 64x64 clear + 48x48 triangle`

`PASS: 1174 runs in 1.0 s, 0 with wrong pixels`

Kernel independently reports first Render completion, `127875 ns` GPU time, with non-zero TA and fragment timing. Private E1077 `RESULT.md` SHA-256: `db13fb493401d0e1eba7ff397752d0dbc59d7fb46e23f926135a6f7c0aae8f58`.

### E1078 — GPU triangle reaches active panel through a verified CPU-copy bridge

The passing render path yielded an exact 16384-byte 64x64 RGBA8 GPU readback, raw SHA-256 `949b6175a756379bf58f27c4543a1f15a294b259c54e9a0464fe860605e309cd`, triangle bbox `x=8..55, y=8..55`.

On that boot simpledrm/simple-framebuffer owns the internal panel (2880x1800, `x2r10g10b10`) while Asahi is a separate render-only DRM device. The exact GPU output was converted to the panel format and CPU-copied to `/dev/fb0`; intended image and immediate framebuffer readback both hashed to `23f4bf863f03c29bcc2832c625edaadd20f2e5cb656015b494358df2415898dc`. The original framebuffer was restored byte-for-byte, pre/restored SHA-256 `1b86724c5caa46dc35dfca1d5a9976e9d43810c071422facc0c28e076ee2db95`.

Private E1078 `RESULT.md` SHA-256: `196cef3d28007c10c2fcdacf810bdd9fc19b20d6221e5fd29a257954daa6389b`.

**Classification:** E1078 is real GPU-rendered pixels reaching the active panel, but by a CPU copy. It is not native Asahi/DCP/KMS scanout.

## 2. E1086 — native-presentation differential

E1086 extends E1077/E1078; it does not replace or renumber them.

The exact E1075 runtime was reproduced again through Fabric. Independent graphics controls remained clean:

- Vulkan `vk-render-check`: `1084 runs in 1.0 s`, `0 wrong pixels`; kernel first Render completion `127708 ns` GPU time.
- Zink over Honeykrisp with `MESA_LOADER_DRIVER_OVERRIDE=zink`: renderer `zink Vulkan 1.4(Apple M3 (G15G C0) (MESA_HONEYKRISP))`; `1028 runs in 1.0 s`, `0 wrong pixels`.
- No GPU, DART or RTKit fault, panic or Oops during the sequence.

Presentation discriminators:

1. `vkcube --wsi display` admits G15G, then reports `Cannot find any display!`; the render GPU exposes no Vulkan direct-display object for the simpledrm-owned panel.
2. GDM/GNOME Wayland starts on `card0/simpledrm`. Attempts by old system Mesa to use the G15G render device are rejected by the kernel's explicit 592-vs-600-byte GET_PARAMS gate, so Mutter remains on simpledrm/CPU.
3. A Wayland EGL window can render and `eglSwapBuffers`, but follows the compositor's simpledrm device and therefore uses softpipe even when the isolated prefix, Zink override and `DRI_PRIME=1` are supplied.
4. Honeykrisp `vkcube --wsi wayland --c 60` selects `Apple M3 (G15G C0)` and causes GPU activity, but does not finish within the 10-second bound. At the corresponding time Mutter logs `Invalid sequence for VSYNC frame info`. No GPU/DART/RTKit fault occurs.
5. `VK_PRESENT_MODE_IMMEDIATE_KHR` is unsupported on this Wayland surface, so the FIFO/vsync path cannot be bypassed that way.
6. Xwayland is software-only and reports no DRI3, so Honeykrisp XCB presentation is unavailable in this session.

Private E1086 `RESULT.md` SHA-256: `0e78deb6a85e6ca3030ff712d44e160295d065739dd987b07379b35fac4b6a54`.

### E1086 boundary before native DCP work

`correct G15G Compute + TA/fragment triangle render/readback -> GPU Wayland swapchain/job activity -> [cross-device simpledrm/render-GPU presentation + vsync/fence integration] -> native presented GPU buffer`

E1086 established that this was a display/presentation problem, not a GPU execution problem.

## 3. E1091 — first native J615 DCP/KMS display takeover

E1087-E1091 moved the display side from fail-soft PMP/DCP admission to a real native Apple DRM device. The decisive E1091 display-only boot used kernel `7.1.12-e1091-t8122-pmp-reordered` and the retained J615 display handoff image SHA-256 `2c908a8ddbc592a43a298298051f7727eec2081b1e75aa5aa535cdbb36b60a81`.

Mechanically observed progression:

- the T8122 display gate enabled both DCP DARTs, mailbox, DCP and display-subsystem;
- the 56-value J615 PMP image from `/chosen/asahi,t8122-pmp` was consumed;
- PMP halted preflight accepted and seeded request `0x20080`;
- the PMP started on the first display consumer and reached ready with request `0x20080`, ack `0x200f0`;
- `apple-dcp` observed the display request acknowledged;
- DCP RTKit and DCPLink ran and PIODMA buffers mapped;
- J615 display started at `2880x1800@60`, with 64 notch rows hidden;
- native `apple-drm` registered, `eDP-1` is connected/enabled, and `fb0` became `appledrmfb`;
- simpledrm no longer owns the active framebuffer.

The Asahi GPU was intentionally blacklisted for E1091. This is therefore a native-display proof, not yet a simultaneous GPU+display or zero-copy presentation proof.

Private E1091 `RESULT.md` SHA-256: `d350acf59fc23ce1d219cb3d5e046dd0c29fcdcaddd1416aaad01a175472a486`. The exact source delta against E1075 base `a4778ca2a3f4791245cf1f7f42f26ccf2b17f5f7` is preserved privately as `E1091-SOURCE-DIFF.patch`, SHA-256 `691b050993339adfa50d1419479a6fe0453662d2938007c34faae01dc2de25a3`.

## 4. E1092 — simultaneous native display + G15G Render

E1092 keeps the exact E1091 kernel/display source and changes only the handoff/runtime policy needed to enable the already-qualified GPU path. The combined handoff SHA-256 is `9b0b8bbc44c4de447381e12e5eea2d825856abb08f3eecbc75343f1839636502` and contains the J615 GPU, GPU-power-standin and DCP opt-ins. The cmdline retains `asahi.t8122_start=1 asahi.t8122_pstate_cap=1`.

On one boot:

- native `apple-drm`, `eDP-1` and `appledrmfb` remain operational;
- G15G reaches `firmware-running`, exposes `renderD128`, and remains capped at 338 MHz;
- the isolated G15G Vulkan stack completes `988 runs in 1.0 s, 0 wrong pixels`;
- kernel independently reports first Render completion `168708 ns` and first Compute completion `25791 ns`;
- no fatal GPU, DART or RTKit fault occurs.

Private E1092 `RESULT.md` SHA-256: `7d303dfa0e8e563fcefcc31fd2413be82ea07b0baa52c68138de5590f8151d0c`.

## 5. E1093 — first direct G15G DMA-BUF -> native Apple DCP/KMS presentation

E1093 removes the E1078 CPU pixel-copy bridge.

A bounded helper asks Honeykrisp for a full-mode 2880x1800 linear `VK_FORMAT_B8G8R8A8_UNORM` exportable DMA-BUF. The mechanically checked layout is offset 0, row pitch 11520 bytes (exactly 2880*4 and 64-byte aligned), allocation 20,736,000 bytes. Native Apple DRM PRIME import succeeds and `drmModeAddFB2(XRGB8888, pitch=11520, offset=0)` succeeds.

For the decisive run the GPU clears the image magenta and waits for GPU completion before export. The same DMA-BUF is PRIME-imported into `apple-drm`, made KMS fb44 and committed to the native J615 CRTC. The driver-owned DCP debugfs oracle records:

```text
before:   swaps 7, active_fb 42, imported 0
present:  swaps 8, active_fb 44, imported 1
restore:  swaps 9, active_fb 42, imported 0
```

This is stronger than a successful userspace ioctl. Source inspection shows `dcp_v14_present()` advances `swaps` and `active_fb` only after the synchronous firmware `A(407)` swap returns success, and `imported` is derived from the active framebuffer GEM object's import attachment. The informational dmesg `swap N complete` line is intentionally emitted only for swaps 1..3, explaining its absence for E1093.

No GPU image mmap or CPU pixel copy is used. The native panel remains connected and the original framebuffer is deterministically restored.

Private E1093 `RESULT.md` SHA-256: `1180520285949fb309aa2ca099ae48e5ebce6e014715ad308b1645c457ecac38`. Evidence manifest SHA-256: `013b8d032f407445efcc694d02d945de8a703d20c9d824f6f4801ddc9a772f8e`.

## 6. E1094 — native Wayland/Mutter presentation, audited

E1094 keeps the qualified E1092/E1093 native-display + G15G stack and moves one layer upward into the ordinary GNOME/Mutter Wayland path. Real Honeykrisp/G15G buffers are committed as Wayland DMA-BUF buffers, released by the compositor, and receive real `wp_presentation_feedback.presented(...)` events while native Apple DRM/DCP remains active. A short FIFO run completes, and a 60-frame mailbox run completes. This mechanically crosses compositor-mediated native presentation.

Independent Sol/xhigh and Astra reviews then re-read the retained raw traces and corrected several initial interpretations:

- Mutter's `Invalid sequence for VSYNC frame info` warning **persists** in E1094, including a successful short FIFO run. The first negative search used a failed journal capture and must not be used as evidence.
- The attempted `fifo_latest_ready` comparison is invalid because Mesa rejected the selector.
- Mailbox vs FIFO is not yet a one-variable present-mode control: the inspected Wayland WSI defaults FIFO to three images and true mailbox to four unless actual creation counts prove otherwise.
- The long client waits are localized to delayed release/image availability, but all-images-busy is a downstream state rather than a root cause.
- Before the long waits, the next timed target is only about one refresh (~16-18 ms) beyond the previous presentation, yet retained waits are about 0.69 s, 1.00 s and at least 2.73 s. A grossly far-future target therefore does not explain the observed delay.
- Native sequence-zero metadata remains a real defect, but sequence validity, timestamp quality and KMS completion-event liveness must be separated; sequence zero alone is not mechanically linked to the blocking predicate.
- Mutter is still software-compositing on this qualification path. E1094 proves a G15G client is presented through Mutter, not that GNOME itself is rendered by G15G.

The leading working model is a timed/FIFO liveness interaction: older work drains, a modestly future/due transaction or another dependency on its critical path is not advanced promptly, the three-image FIFO ring fills, and later compositor/fence/KMS activity releases a burst. This is a hypothesis, not a proven Mutter bug.

Private E1094 initial `RESULT.md` SHA-256: `a61a762a673600944bf0b49dd3a0cdef798c3efe3ec05d244e32d23e4f8001cf`. Sol/xhigh audit SHA-256: `7d16c74678af90ae885126b729b7081493f1663206a10246c3f9e463d4e720a0`. Astra focused review SHA-256: `3cdf801d75fda1cfbf78371f47bec5a5d5e94975e08dcc214249cc5e89a61245`.

See `docs/E1094-AUDITED-PRESENTATION.md` for the corrected evidence model and next discriminator.

### E1095 & E1096 — Live 4-Leg ABCA Timestamp Causal Proof & Mutter/DCP Root-Cause Proof

- **E1095 (`experiments/E1095-wayland-timestamp-abca`):** Built and offline-qualified the transparent Wayland proxy (`wlcommit-proxy`, `wlcommit-proxy.c` SHA-256 `62eb8f328c47f479865f1a59ce06dc1ec41d0c011e3ec46e88d30b11db518668`) and the one-session 4-leg `ABCA` runner (`run-e1095-abca.sh` SHA-256 `11137512d7df8e9b1df0f1630fda25c9323f1e38ed909ccb7a9bb97c2ab0dfec`; `analyze-abca.py` SHA-256 `299f44f73d6ab3cf2b3bf0e4fa7f7425405c8bf3fa5f296daa786ff2f0709a18`; `OFFLINE-QUALIFICATION.md` SHA-256 `a249c4118e0eb64ca3498d07f23d5f7063d5e87dc38c992f6f402a7701078148`), then executed the complete 4-leg `A1 -> B -> C -> A2` live bracket (`runs/20261025T124557Z-b95f18fd`, boot `b95f18fd-c2f2-44cd-9a54-aeafac243186`, `RESULT.md` SHA-256 `a617c8594ba092cf01f5c4498998f6a3f7dabc27fb9da3ce49818ba2f37a9740`, `BRACKET.json` SHA-256 `e598c07d9081b555b016b9787aedc1a07e32c4fd015759d96da79212e0ce1600`, `BRACKET-SUMMARY.txt` SHA-256 `c3461bd4958e324d60c4dcb5be3afbfa81495afbbfb2dd0fe15353b2adcccd67`):
  - **Verdict (`qualified=true`):** `TIMESTAMP-PATH SPLIT: timestamp-only suppression (B) eliminated the FIFO60 stalls while both passthrough controls (A1, A2) stalled; wp_commit_timer_v1.set_timestamp is causally necessary for the stall under matched 3-image FIFO topology.`
  - **Swapchain image count & topology:** All 4 arms (`A1`, `B`, `C`, `A2`) allocated `6` buffers in two bursts of **`3` images** (`last_creation_burst_counts = A1:3 B:3 C:3 A2:3`, `500x500`, `XRGB8888`, `stride=4096`, `modifier=0x0`), ruling out swapchain image count as a confounder.
  - **Pacing & stall measurements across `A1 -> B -> C -> A2`:**
    - **`A1` (`mode=pass`):** `rc=0`, `stalled=1`, 60 attaches in **`13,546.0 ms`** (`4.43 FPS`), `max_steady_attach_gap_ms = 10,720.039 ms`, `steady_stalls_gt_100ms = 4` (`501.844 ms` @ f14, `593.019 ms` @ f21, `10,720.039 ms` @ f25, `537.803 ms` @ f44), `timestamps_seen = 52 / dropped = 0`, `fifo = set:60, wait:112`, `presented = 53`, `releases = 56`, `DCP swaps` `8 -> 80` (`failed=0`), `vsync_warnings = 1`.
    - **`B` (`mode=drop-timestamps`):** `rc=0`, **`stalled=0`**, 60 attaches in **`1,061.2 ms`** (`56.5 FPS` at 60 Hz), `max_steady_attach_gap_ms = 118.778 ms` (at initial resize frame 5; `0` gaps `>= 50 ms` across frames 6..59), `timestamps_seen = 53 / dropped = 53` (100% filtered), `fifo = set:60, wait:113`, `presented = 50`, `releases = 56`, `DCP swaps` `87 -> 146` (`failed=0`), `vsync_warnings = 0`.
    - **`C` (`MESA_VK_WSI_DEBUG=nowlts`):** `rc=0`, **`stalled=0`**, 60 attaches in **`841.7 ms`**, `max_steady_attach_gap_ms = 102.728 ms`, `timestamps_seen = 0 / dropped = 0`, `fifo = set:60, wait:60`, `presented = 0`, `releases = 56`, `DCP swaps` `147 -> 196` (`failed=0`), `vsync_warnings = 0`.
    - **`A2` (`mode=pass` reversal):** `rc=0`, `stalled=1`, 60 attaches in **`12,268.8 ms`** (`4.89 FPS`), `max_steady_attach_gap_ms = 10,096.041 ms`, `steady_stalls_gt_100ms = 4` (`124.035 ms` @ f6, `10,096.041 ms` @ f16, `281.721 ms` @ f39, `729.804 ms` @ f58), `timestamps_seen = 53 / dropped = 0`, `fifo = set:60, wait:113`, `presented = 51`, `releases = 56`, `DCP swaps` `196 -> 265` (`failed=0`), `vsync_warnings = 0`.
  - Every `>= 200 ms` stall across both `A1` and `A2` occurred on a frame where `target_ns - last_presented_ns` was `+32.8 ms` (~2 vblanks in the future) or `+66.2 ms` (~4 vblanks in the future). `0` GPU/DART/RTKit/DCP faults occurred across the entire boot (`DCP swaps` `8 -> 291`, `failed=0`), and automatic Golden recovery succeeded (`boot_id=a3342496-330a-4b77-a391-c5f1d211ac9c`, `GOLDEN-RECOVERY-AUTO.txt` SHA-256 `2d130dd5960ea0e2429747e4ff0eb34313feb1abb07d91dd7098a960efe0b097`).
- **E1096 (`experiments/E1096-mutter-commit-timing-and-dcp-vblank-root-cause`):** Completed an offline static and dynamic execution proof (`repro-mutter-commit-timing-stall.c` SHA-256 `42a6f0db9ddf3e96b389b2d623f724ceafb904f618924fc337a8ab64f3030f3e`; `REPRO-OUTPUT.txt` SHA-256 `4b61d96ef6ff057b8a0934b0a9cf14251832ec7dae901022a0f6d32f7d638cda`; `RESULT.md` SHA-256 `c333c3c99997de1cbda12ff16a5f4f500ce2fb35882d0426095f8f43624661e2`):
  1. **Exact mechanism of `"Invalid sequence for VSYNC frame info"` (`vsync_warnings = A1:1 B:0 C:0 A2:0` in `E1095`) and `+1` presentation sequence deltas across stalls:** In `drivers/gpu/drm/apple/dcp.c:127` (`dcp_crtc_send_page_flip_event`), the Apple DCP driver hardcodes `unsigned int seq = 0` when calling `send_vblank_event(dev, e, seq, flip)`. In `libmutter-18.so.0` (`on_presented` at `0x16bfa8..0x16c010`), `frame_info->sequence == 0` with `COGL_FRAME_INFO_FLAG_VSYNC` logs `Invalid sequence for VSYNC frame info` once per boot (`+433`) and falls back to `wayland_output->presentation_sequence += 1` instead of `frame_info->sequence - last_sequence` (`0x16c154`).
  2. **Full `E1094` `VKCUBE.txt` timeline reconstruction & `ClutterFrameClock` state-machine defects:** Executing `repro-mutter-commit-timing-stall` against `/usr/lib/aarch64-linux-gnu/mutter-18/libmutter-clutter-18.so.0` demonstrated two `schedule_update_target_time` (`0x67908..0x67b40`) defects: **Bug A (`0x67a3c`)** sets `fc->pending_reschedule_now = 1` when `state == DISPATCHED_ONE (5)`, causing `notify_presented` to call `clutter_frame_clock_schedule_update_now` which zeroes `fc->has_next_presentation_time = 0`; **Bug B (`0x67ae0..0x67b40`)** transitions `IDLE (1) -> SCHEDULED_LATER (4)` in fixed refresh-rate mode (`vrr_mode == 0`) without updating `fc->has_next_presentation_time` or `fc->next_presentation_time_us`, leaving `fc->next_presentation_time_us` at `0` or at the stale presentation timestamp of the preceding frame and blocking the timed transaction across spurious idle dispatches (`Test 1 PASS`). Untimed FIFO (`Test 2 PASS`, `E1095` Arm B / Arm C) and repairing Bugs A & B (`Test 4 PASS`) eliminate all spurious idle dispatches.

### E1097 — native GNOME compositor acceleration

GNOME Shell itself was qualified on the pinned Zink/Honeykrisp Apple M3 path before the test client ran. The first compositor Render completed independently of the later client; FIFO60 returned 0; DCP advanced `3 -> 67` with `failed=0`; automatic Golden recovery was verified. Private `RESULT.md` SHA-256: `5e5bed6c23002b502d39190892ffa9e194df3c5e003a3f3ecfa2ec9b4dd7516f`.

### E1098 — Linux 7.3 J615 port/build

The repaired J615/G15G/display stack was ported into an isolated Linux 7.3 source tree. Source commit `52f5d1cb8f5983131319a3083c0f1597e871a3e9` builds release `7.3.0-m3-j615-e1098+`, including the J615 DTB, Asahi GPU, Apple DRM and PMP modules. The source is pushed to `geocausa/linux:wip/m3-7.3-j615`. The older local `PROGRESS.md`/build manifest predate package installation and the first hardware run; E1099 supersedes their `hardware_tested=false` bookkeeping.

### E1099 — Linux 7.3 physical first boot and bounded desktop/GPU qualification

The guarded one-shot boot entered `7.3.0-m3-j615-e1098+` on the physical J615. GNOME Shell repeatedly reported `zink Vulkan 1.4(Apple M3 (G15G C0) (MESA_HONEYKRISP))`; no software compositor context was observed. FIFO60 returned `0`; first Render completed in `175041 ns`; first Compute in `1273875 ns`; native DCP advanced `3 -> 83` during the bounded test with `failed=0`; the post-baseline fatal scan was empty. The controller restored Golden assets and automatically returned to `7.1.6-ubuntu-m3-usbpd-gc5037a961e4d`. See `docs/E1099-LINUX73-LIVE-QUALIFICATION.md`.

### E1100 — Linux 7.3 timestamp-path causal replay

The E1095 `A1 -> B -> C -> A2` bracket was replayed on Linux 7.3 while retaining its userspace/compositor topology and holding `asahi.t8122_pstate_cap=1` fixed strictly as a known-good control point. A1/A2 stalled and hit their 30-second arm timeouts (`16934.396 ms` / `14141.215 ms` maximum steady attach gaps); B dropped all `53/53` observed commit timestamps and completed (`106.834 ms` maximum gap); C emitted no commit timestamps and completed (`108.869 ms`). All arms used matched 3-image FIFO topology, all post-baseline fatal scans were empty, and DCP stayed `failed=0`. The analyzer again returned `qualified=true` / `TIMESTAMP-PATH SPLIT`. See `docs/E1100-LINUX73-TIMESTAMP-ABCA.md`.

This strongly supports that the timed-FIFO defect is not specific to the older 7.1.x kernel lineage. It does not establish a quantitative 7.3 regression from a single replay.

### E1101 — accelerated GNOME endurance control on 7.3

One hardware-qualified GNOME Shell PID survived 8 consecutive 600-frame FIFO/`nowlts` rounds (4,800 requested frames, 80.536783 s summed client intervals). Every client returned 0 and selected Apple M3/G15G; every round advanced DCP with `failed=0`; all eight per-round and the global post-baseline fatal scans were empty. DCP progressed `3 -> 4872` before cleanup; final Shell RSS was only +432 kB versus pre-run. Exact Golden recovery succeeded. See `docs/E1101-LINUX73-ACCELERATED-GNOME-ENDURANCE.md`.

The live J615 table publishes states 1–8 at 338/618/796/928/1056/1170/1278/1338 MHz. At the E1101 checkpoint the T8122 experiment admitted caps only 1–5; E1106–E1108 later widened that explicit experiment gate one state at a time through 8. J615 still has no effective SoC-die thermal zone for this governor, so the high-state qualifications remain deliberately short.

### E1102 — pstate 2 / 618 MHz qualification

The first frequency escalation passes. J615 InitData published cap/boot request 2 = 618 MHz, firmware peak state 2 and repeated actual/current state-2 samples were observed, both 300-frame accelerated-GNOME rounds returned 0, DCP progressed with `failed=0`, all post-baseline fault scans were empty, and exact Golden recovery succeeded. This is bounded qualification without effective SoC-die temperature feedback, not thermal-soak qualification. See `docs/E1102-PSTATE2-618MHZ-QUALIFICATION.md`.

### E1103 — pstate 3 / 796 MHz qualification

The next single-variable escalation also passes: cap/boot request 3 = 796 MHz, firmware peak/actual/current state 3 observed, both 300-frame clients rc0, DCP progress with failed0, all fault deltas empty, exact Golden recovery. See `docs/E1103-PSTATE3-796MHZ-QUALIFICATION.md`.

### E1104–E1105 — pstate 4 / 928 MHz and pstate 5 / 1056 MHz

Both one-state-at-a-time escalations pass under the same short accelerated-GNOME envelope. Firmware genuinely exercises the requested state, both 300-frame rounds return 0, DCP advances with `failed=0`, fatal deltas remain empty and Golden recovery is exact. State 4 is 928 MHz (755 mV core / 790 mV SRAM); state 5 is 1056 MHz (805 mV core / SRAM). See `docs/E1104-PSTATE4-928MHZ-QUALIFICATION.md` and `docs/E1105-PSTATE5-1056MHZ-QUALIFICATION.md`.

### E1106–E1107 — explicit gate widening to 1170 and 1278 MHz

The armed T8122 experiment ceiling is deliberately widened one state at a time while ordinary non-experimental M3 policy remains unchanged. E1106 source `a922ef259ba...` admits state 6 and qualifies 1170 MHz / 850 mV. E1107 source `beeec2e1a3201b44577d1990c795c1203d6b2ff3` admits state 7 and qualifies 1278 MHz / 890 mV. Firmware actual/current/peak evidence confirms each state is genuinely reached; 2×300-frame workloads, DCP/fault gates and exact Golden recovery all pass.

### E1108 — pstate 8 / 1338 MHz, highest current J615 table state

Source `2866b256c82f5c71e5a9205f7cfcf7ab44a89460` widens only the armed T8122 experiment to state 8 and is pushed to `geocausa/linux:wip/m3-7.3-j615`. Before hardware, 8/8 knob tests, 86/86 air-gpu tests (19 expected skips), and 46/46 release tests (29 expected skips) pass. Live InitData exposes 8 performance states, state 8 = 1338 MHz at 915 mV core/SRAM. Firmware repeatedly reports actual/current state 8 and peak state 8; both 300-frame clients return 0; DCP progresses `3 -> 635` (`failed=0`) and reaches 641 after cleanup; fatal scan is empty; Honeykrisp GNOME remains qualified; exact Golden recovery succeeds. See `docs/E1108-PSTATE8-1338MHZ-QUALIFICATION.md`.

Linux still has no effective J615 SoC-die temperature zone for these runs. Therefore E1102–E1108 are bounded execution qualifications, not thermal-soak results. State 8 is the highest state in the current live J615 table, not a universal physical silicon-maximum claim.

### Current hard boundary

`G15G Compute/Render PASS + native DCP/KMS PASS + direct/Wayland presentation PASS + timing diagnosis PASS + accelerated GNOME PASS + Linux 7.3 PASS + timestamp replay/endurance PASS + states 1..8 through 1338 MHz bounded PASS -> telemetry/sustained thermal qualification -> suspend/performance/power/peripherals`

Basic execution, native presentation, timed-FIFO diagnosis, accelerated GNOME composition and the first Linux 7.3 hardware gate are closed milestones.

## 7. Historical execution localization retained

E972/E974/E989–E1005 remain valid for the older E933/E950-style failing lineage:

- E972: Apple and Linux match through outer-ESL token5; Apple then emits token7 carrying the initial PC while Linux does not.
- E974: parser state `0x61` is ESL slot 1, not a hidden hardware mode.
- E989–E1005: the old failure is generic to complete state operations, not RLD-specific.

Preserve those negatives, but do not present them as the current repaired-lineage frontier.

## 8. Do not repeat

- Do not reopen instruction-0, token5→token7, shader, TA/fragment, Queue/RunCompute/UAT/PagePool/power/parser/MMIO hypotheses without contradictory evidence.
- Do not use the legacy manager-backend `G15Debug::SelfTest` bit47 as a current qualification gate.
- Do not interpret `Cannot find any display!`, softpipe Wayland EGL, or Xwayland no-DRI3 as GPU Render failures.
- Do not replace system Mesa merely to continue the display investigation; the isolated G15G userspace path is already qualified.

## 9. Immediate next actions

1. Establish reliable J615 GPU/SoC thermal and power telemetry; use the macOS oracle when Apple sensor behavior or mapping is required.
2. Extend state-8 from a short bounded pass to sustained/thermal-soak qualification and characterize performance/power scaling across states 1–8.
3. Qualify suspend/resume and focused peripheral regressions. Optional Mutter ClutterFrameClock and Apple-DCP vblank-sequence repairs remain useful but are no longer blockers for the proven `nowlts` qualification path.

## 10. Recovery state

The E1093 recovery back to Golden was mechanically verified at the time:

- Golden kernel: `7.1.6-ubuntu-m3-usbpd-gc5037a961e4d`.
- E1093 recovery boot ID: `db06411a-f5ab-450f-a799-8694ab6d9674`.
- Known-good m1n1 `boot.bin` SHA-256: `a81993b122b7f990bc99ccab42decab7ef92adb94741dadf0ee79d24fdda3e0d`.
- Original `/boot/grub/custom.cfg` SHA-256: `cf011eee536420c6b32fa5b311345f507ae260d06c7f30777e2951e683b73462`.

Latest recovery is post-E1108: Golden `7.1.6-ubuntu-m3-usbpd-gc5037a961e4d` is running at boot `ca57148f-281e-49ca-85ae-029c2798cf53`; `boot.bin` and `custom.cfg` match known-good hashes; GRUB one-shot, owner marker and E1108 service are absent. State-8 source `2866b256c82f5c71e5a9205f7cfcf7ab44a89460` is pushed to `geocausa/linux:wip/m3-7.3-j615`.

## Evidence pointers

- E1075: `/home/macmac/m3-gpu-lab/experiments/E1075-t8122-hwdata-418c-positive-control/RESULT.md`.
- E1076: `/home/macmac/m3-gpu-lab/experiments/E1076-e1075-empty-compute-selftest/RESULT.md`.
- E1077: `/home/macmac/m3-gpu-lab/experiments/E1077-vk-render-readback/RESULT.md`.
- E1078: `/home/macmac/m3-gpu-lab/experiments/E1078-visible-triangle-bridge/RESULT.md`.
- E1086: `/home/macmac/m3-gpu-lab/experiments/E1086-wayland-presentation-differential/RESULT.md`.
- E1091: `/home/macmac/m3-gpu-lab/experiments/E1091-t8122-pmp-reordered-devices/RESULT.md`.
- E1092: `/home/macmac/m3-gpu-lab/experiments/E1092-j615-native-display-plus-g15g/RESULT.md`.
- E1093: `/home/macmac/m3-gpu-lab/experiments/E1093-g15g-prime-to-appledrm/RESULT.md`.
- E1094: `/home/macmac/m3-gpu-lab/experiments/E1094-native-gnome-appledrm-g15g/RESULT.md`; corrected by the retained Sol/Astra audits referenced above.
- E1095: `/home/macmac/m3-gpu-lab/experiments/E1095-wayland-timestamp-abca/RESULT.md`.
- E1096: `/home/macmac/m3-gpu-lab/experiments/E1096-mutter-commit-timing-and-dcp-vblank-root-cause/RESULT.md`.

- E1097: `/home/macmac/m3-gpu-lab/experiments/E1097-desktop-acceleration-preparation/RESULT.md`.
- E1098: `/home/macmac/m3-gpu-lab/experiments/E1098-ubuntu-2610-linux-73-migration-audit/`.
- E1099: public summary `docs/E1099-LINUX73-LIVE-QUALIFICATION.md`; raw evidence remains machine-local.
- E1100: public summary `docs/E1100-LINUX73-TIMESTAMP-ABCA.md`; raw bracket evidence remains machine-local.
- E1101: public summary `docs/E1101-LINUX73-ACCELERATED-GNOME-ENDURANCE.md`; raw endurance evidence remains machine-local.
- E1102: public summary `docs/E1102-PSTATE2-618MHZ-QUALIFICATION.md`; raw pstate evidence remains machine-local.
- E1103: public summary `docs/E1103-PSTATE3-796MHZ-QUALIFICATION.md`; raw pstate evidence remains machine-local.
- E1104: `docs/E1104-PSTATE4-928MHZ-QUALIFICATION.md`.
- E1105: `docs/E1105-PSTATE5-1056MHZ-QUALIFICATION.md`.
- E1106: `docs/E1106-PSTATE6-1170MHZ-QUALIFICATION.md`.
- E1107: `docs/E1107-PSTATE7-1278MHZ-QUALIFICATION.md`.
- E1108: `docs/E1108-PSTATE8-1338MHZ-QUALIFICATION.md`; raw pstate evidence remains machine-local.
