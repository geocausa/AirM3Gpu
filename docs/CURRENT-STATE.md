# Current G15 Bring-up State

Research state: **2026-10-24**

Target: MacBook Air M3 J615 / T8122 / G15G C0. Exact reference: macOS 14.8.3 / 23J220.

> Canonical resume point: `docs/CANONICAL-CHECKPOINT.md`.

## Headline

**J615 now has mechanically verified correct Compute, Vulkan Render, triangle pixels, and an active-panel framebuffer bridge.**

E1075 is the startup prerequisite: restoring the exact E056-derived T8122 HwDataA `+0x418c..+0x41db` 20-word image, together with the exact qualified handoff, moves the reconstructed runtime to `firmware-running` with `/dev/dri/renderD128` present.

E1076 proves real Compute/readback:

`65536 invocations x 256 steps; 788 runs in 1.0 s; 0 wrong results`

Kernel completion evidence:

`M3 G15G verdict: job-completed: the first compute job finished ... 773625 ns of GPU time`

E1077 proves real graphics execution and pixel-correct triangle readback:

`vk-render-check: 64x64 clear + 48x48 triangle, 1 s`

`PASS: 1174 runs in 1.0 s, 0 with wrong pixels`

Kernel completion evidence:

`M3 G15G verdict: job-completed: the first render job finished ... 127875 ns of GPU time`

The render timing stream has non-zero TA and fragment time. The private E1077 `RESULT.md` SHA-256 is `db13fb493401d0e1eba7ff397752d0dbc59d7fb46e23f926135a6f7c0aae8f58`.

E1078 then instruments only the Vulkan map/unmap boundary of the already-passing checker. It still passes `1258 runs / 0 wrong pixels` and captures the exact 16384-byte 64x64 RGBA8 GPU readback. The triangle occupies a 48x48 bbox (`x=8..55, y=8..55`), with raw readback SHA-256 `949b6175a756379bf58f27c4543a1f15a294b259c54e9a0464fe860605e309cd`.

Those exact GPU-produced pixels were converted to the active internal panel's `x2r10g10b10` simple-framebuffer format and written to `/dev/fb0`. Intended image and immediate framebuffer readback both hash to `23f4bf863f03c29bcc2832c625edaadd20f2e5cb656015b494358df2415898dc`. The pre-display framebuffer and restored framebuffer both hash to `1b86724c5caa46dc35dfca1d5a9976e9d43810c071422facc0c28e076ee2db95`.

**Important distinction:** E1078 is a GPU-render -> CPU-copy -> simpledrm framebuffer bridge. It demonstrates the real GPU triangle reaching active panel scanout memory, but it is not native Asahi/DCP/KMS presentation.

## Current frontier

The next major engineering layer is **native DCP/KMS/display ownership and GPU-buffer presentation**, then compositor/desktop and endurance qualification.

The current repaired-lineage progression is:

`firmware-running -> correct Compute -> correct Render -> correct triangle pixels -> active panel framebuffer bridge`

Do not return to broad Queue/RunCompute/UAT/PagePool/power/parser/MMIO searches without contradictory evidence.

## Historical boundary retained

E972/E974/E989–E1005 remain valid for the older failing E933/E950-style lineage: common outer-ESL token5 followed by missing Linux token7/initial PC. Preserve those results, but do not present them as the current E1075/E1077 frontier.

## Safety state

After E1077/E1078, Fabric reconciled the machine back to Golden Linux `7.1.6-ubuntu-m3-usbpd-gc5037a961e4d`, boot ID `dbb9862d-d4fa-4e33-beec-0afecf0feaf8`, known-good m1n1 SHA `a81993b...`, original GRUB SHA `cf011eee...`, no pending one-shot entry, no render node and no live-owner marker.
