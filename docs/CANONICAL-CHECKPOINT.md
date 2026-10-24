# Canonical project checkpoint — J615 / G15

Updated: **2026-10-24**

This file is the single resume point for the AirM3Gpu project. Older “current frontier” statements are historical only.

## Target and authority

- Hardware: MacBook Air M3, J615 / T8122, GPU G15G C0, 10/10 cores.
- Exact ABI authority: macOS 14.8.3 / 23J220 plus matching retained G15G C0 components.
- Persistent safety baseline: Golden Linux `7.1.6-ubuntu-m3-usbpd-gc5037a961e4d`.
- Raw captures, proprietary binaries and machine-specific evidence remain private.

## 1. Current headline — real Compute, Render and triangle pixels on J615

The repaired E1075 runtime lineage now executes both Compute and graphics correctly on the physical J615.

Qualified runtime:

- Linux source: `geocausa/linux:wip/g15-e1075-hwdata418c @ a4778ca2a3f4791245cf1f7f42f26ccf2b17f5f7`.
- Experimental kernel: `7.1.12-e1075-hwdata418c-ga4778ca2a3f4`.
- E1075 startup correction: exact Apple-derived T8122 HwDataA `+0x418c..+0x41db` 20-u32 image originally recovered in E056.
- Exact E1043-D identity-capable boot handoff used for live qualification.
- Firmware reaches `magic=0xabcdabcd runtime=1 newA=1/1 newB=0`, reports `firmware-running`, and exposes `/dev/dri/renderD128`.
- Runtime capped at pstate 1 / 338 MHz.

Qualified userspace:

- System Mesa is too old for the 600-byte M3-aware GET_PARAMS ABI.
- Retained ordinary `mesa-m3` is M3-aware but explicitly lacks the G15 command stream.
- Isolated Aurora `mesa-m3-g15c 26.1.4.g15c1-2`, package SHA-256 `552c94de1a98a25cc5e0dbd9e3a4cf5fdd738fb5d95689b41428f9dee3aa0936`, contains the working G15G/AGX3 path.
- With `ASAHI_M3_EXPERIMENTAL=1`, it prints `G15G admitted: chip 0x8122, 1 cluster, 10 cores; scheduling stalls on` and identifies `Apple M3 (G15G C0)`.

### E1076 — real Compute/readback

Bounded one-second result:

`65536 invocations x 256 steps; 788 runs; 0 wrong results`

Kernel independent completion:

`M3 G15G verdict: job-completed: the first compute job finished ... 773625 ns of GPU time`

This crosses real command construction, VM/queue submission, firmware scheduling, G15 frontend/state-operation admission, shader execution, retirement and CPU-visible readback.

### E1077 — real Render + triangle readback

Using the same runtime and userspace, `vk-render-check` performs a 64x64 clear plus a 48x48 triangle and verifies every output pixel.

Bounded one-second result:

`PASS: 1174 runs in 1.0 s, 0 with wrong pixels`

Kernel independent completion:

`M3 G15G verdict: job-completed: the first render job finished ... 127875 ns of GPU time`

The timing stream records non-zero TA and fragment time. This crosses graphics command construction, tile/render state, vertex/fragment execution, render completion and CPU-visible pixel readback.

Private E1077 `RESULT.md` SHA-256: `db13fb493401d0e1eba7ff397752d0dbc59d7fb46e23f926135a6f7c0aae8f58`.

### E1078 — GPU triangle reaches active internal-panel framebuffer

The already-passing E1077 checker was instrumented only at Vulkan allocation/map/unmap boundaries. It still passed `1258 runs / 0 wrong pixels` and exposed one exact 16384-byte host-visible readback: 64x64x4 RGBA8.

Decoded GPU output:

- background RGBA `(32,64,96,255)`;
- triangle bbox `x=8..55, y=8..55` = 48x48;
- 2304 non-background pixels;
- center pixel `(128,128,0,255)`;
- raw GPU readback SHA-256 `949b6175a756379bf58f27c4543a1f15a294b259c54e9a0464fe860605e309cd`.

Display side of this boot:

- simpledrm/simple-framebuffer owns the internal panel, separate from the Asahi render-only DRM card;
- connected connector: `card0-Unknown-1`;
- mode: 2880x1800;
- `/dev/fb0`: 32 bpp, stride 11520;
- device-tree format: `x2r10g10b10`.

The exact GPU output was nearest-neighbour scaled 28x, centered, converted pixel-for-pixel to `x2r10g10b10`, and written to the active framebuffer. Intended panel image and immediate framebuffer readback both hashed to:

`23f4bf863f03c29bcc2832c625edaadd20f2e5cb656015b494358df2415898dc`

The original framebuffer was then restored exactly; pre-display and restored SHA-256 both equal:

`1b86724c5caa46dc35dfca1d5a9976e9d43810c071422facc0c28e076ee2db95`

Private E1078 `RESULT.md` SHA-256: `196cef3d28007c10c2fcdacf810bdd9fc19b20d6221e5fd29a257954daa6389b`.

**Interpretation:** this is an end-to-end visible-pixel bridge from real J615 GPU rendering to the active internal-panel framebuffer. It is deliberately **not** claimed as native Asahi/DCP/KMS scanout: a CPU copy bridges the working GPU render device to the existing simpledrm framebuffer.

## 2. Why E1075 mattered

E1075 restores the exact T8122 HwDataA block:

`1, 500, 5, 12500x5, 3000x3, 12500x8, 1`

Without it, the E1065 lineage reaches q21 firmware-ready and publishes DeviceControl entry 1, but firmware never consumes it. With it, DeviceControl is consumed, readiness advances to `0xabcdabcd`, firmware runs and DRM registers. The old E056 Apple-side reduction therefore became a directly causal Linux startup fix.

## 3. Historical execution localization retained

E972/E974/E989–E1005 remain valid for the older failing E933/E950-style lineage:

- E972: Apple and Linux match through outer-ESL token5; Apple then emits token7 carrying the initial PC while Linux does not.
- E974: parser state `0x61` is ESL slot 1, not a hidden hardware mode.
- E989–E1005: the old failure is generic to complete state operations, not RLD-specific.

Do not erase those negatives, but do not present them as the current repaired-lineage frontier. E1076/E1077 cross real Compute and Render execution.

## 4. Closed false starts / do not repeat

- Old `G15Debug::SelfTest` bit47 belongs to the inactive legacy manager backend, not the active M3 runtime backend.
- Ordinary known-good boot.bin does not provide the qualified E1075 identity/ABI handoff.
- System Mesa and ordinary retained `mesa-m3` fail before GPU submission for explicit ABI/command-stream reasons.
- Direct Vulkan `vkcube --wsi display` on this boot reports `Cannot find any display!`: the Asahi device is render-only while simpledrm owns the panel.
- Do not restart broad Queue/RunCompute/UAT/PagePool/power/parser/MMIO searches without contradictory evidence.

## 5. Current frontier

The next major engineering layer is **native DCP/KMS/display ownership and GPU-buffer presentation**, followed by broader compositor/desktop and endurance qualification.

The GPU-side milestones are now mechanically passed on the repaired lineage:

`firmware-running -> Compute correct -> Render correct -> triangle pixels correct -> active panel framebuffer bridge`

## 6. Recovery state

After E1077/E1078, Fabric reboot reconciliation confirmed a return to Golden:

- Golden kernel: `7.1.6-ubuntu-m3-usbpd-gc5037a961e4d`.
- Recovery boot ID: `dbb9862d-d4fa-4e33-beec-0afecf0feaf8`.
- Known-good m1n1 `boot.bin` SHA-256: `a81993b122b7f990bc99ccab42decab7ef92adb94741dadf0ee79d24fdda3e0d`.
- Original `/boot/grub/custom.cfg` SHA-256: `cf011eee536420c6b32fa5b311345f507ae260d06c7f30777e2951e683b73462`.
- No pending GRUB one-shot entry, no render node and no live-owner marker.

## Evidence pointers

- E1075: `/home/macmac/m3-gpu-lab/experiments/E1075-t8122-hwdata-418c-positive-control/RESULT.md`.
- E1076: `/home/macmac/m3-gpu-lab/experiments/E1076-e1075-empty-compute-selftest/RESULT.md`.
- E1077: `/home/macmac/m3-gpu-lab/experiments/E1077-vk-render-readback/RESULT.md`.
- E1078: `/home/macmac/m3-gpu-lab/experiments/E1078-visible-triangle-bridge/RESULT.md`.
