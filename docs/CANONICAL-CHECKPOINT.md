# Canonical project checkpoint — J615 / G15

Updated: **2026-10-24**

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

### Current hard boundary

`native J615 DCP/KMS PASS + G15G Compute/Render PASS (independently) -> [simultaneous display + GPU qualification] -> [native KMS/dma-buf/fence presentation of a GPU-produced buffer] -> compositor/desktop acceleration`

The next experiment must combine the two already-proven subsystems without changing their internals.

## 4. Historical execution localization retained

E972/E974/E989–E1005 remain valid for the older E933/E950-style failing lineage:

- E972: Apple and Linux match through outer-ESL token5; Apple then emits token7 carrying the initial PC while Linux does not.
- E974: parser state `0x61` is ESL slot 1, not a hidden hardware mode.
- E989–E1005: the old failure is generic to complete state operations, not RLD-specific.

Preserve those negatives, but do not present them as the current repaired-lineage frontier.

## 5. Do not repeat

- Do not reopen instruction-0, token5→token7, shader, TA/fragment, Queue/RunCompute/UAT/PagePool/power/parser/MMIO hypotheses without contradictory evidence.
- Do not use the legacy manager-backend `G15Debug::SelfTest` bit47 as a current qualification gate.
- Do not interpret `Cannot find any display!`, softpipe Wayland EGL, or Xwayland no-DRI3 as GPU Render failures.
- Do not replace system Mesa merely to continue the display investigation; the isolated G15G userspace path is already qualified.

## 6. Immediate next actions

1. Restore the live-test free-space safety margin without deleting E1091 evidence or its source/build lineage.
2. Prepare one E1092 boot that keeps the exact E1091 native-display kernel/handoff and changes only the GPU policy: remove the Asahi blacklist and add the already-proven E1075 startup/pstate-cap parameters.
3. Require both native `apple-drm`/`eDP-1` health and G15G `firmware-running`/render-node health before submitting any job; then run exactly one bounded offscreen `vk-render-check`.
4. Only if combined-device qualification passes, present one GPU-produced buffer through native KMS/dma-buf/fence plumbing, with no CPU framebuffer bridge.

Only after that direct native-presentation proof should compositor/desktop acceleration and endurance/performance qualification begin.

## 7. Recovery state

After E1091, the machine was recovered back to Golden:

- Golden kernel: `7.1.6-ubuntu-m3-usbpd-gc5037a961e4d`.
- Current verified Golden boot ID after the later recovery: `d45645c3-7ca3-4049-b01c-87e264dd74ed`.
- Known-good m1n1 `boot.bin` SHA-256: `a81993b122b7f990bc99ccab42decab7ef92adb94741dadf0ee79d24fdda3e0d`.
- Original `/boot/grub/custom.cfg` SHA-256: `cf011eee536420c6b32fa5b311345f507ae260d06c7f30777e2951e683b73462`.
- Persistent and next boot are Ubuntu; no pending GRUB one-shot entry and no experimental render node.

## Evidence pointers

- E1075: `/home/macmac/m3-gpu-lab/experiments/E1075-t8122-hwdata-418c-positive-control/RESULT.md`.
- E1076: `/home/macmac/m3-gpu-lab/experiments/E1076-e1075-empty-compute-selftest/RESULT.md`.
- E1077: `/home/macmac/m3-gpu-lab/experiments/E1077-vk-render-readback/RESULT.md`.
- E1078: `/home/macmac/m3-gpu-lab/experiments/E1078-visible-triangle-bridge/RESULT.md`.
- E1086: `/home/macmac/m3-gpu-lab/experiments/E1086-wayland-presentation-differential/RESULT.md`.
- E1091: `/home/macmac/m3-gpu-lab/experiments/E1091-t8122-pmp-reordered-devices/RESULT.md`.
