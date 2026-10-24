# New-chat handoff — J615 / G15

Generated: **2026-10-24**

**First read:** `docs/CANONICAL-CHECKPOINT.md`.

## One-sentence state

**The physical J615 now executes correct Vulkan Compute and graphics under Linux, renders a pixel-correct triangle, and has carried those exact GPU-produced triangle pixels into the active internal-panel framebuffer; the remaining major layer is native DCP/KMS/display ownership and GPU-buffer presentation.**

## Latest hard facts

- E1075 Linux source: `geocausa/linux:wip/g15-e1075-hwdata418c @ a4778ca2a3f4791245cf1f7f42f26ccf2b17f5f7`.
- E1075 startup state: exact E056-derived T8122 HwDataA `+0x418c..+0x41db` 20-u32 image plus the exact identity-capable handoff; firmware reaches `magic=0xabcdabcd`, `firmware-running`, render node present.
- Qualified userspace: isolated `mesa-m3-g15c 26.1.4.g15c1-2`, package SHA-256 `552c94de1a98a25cc5e0dbd9e3a4cf5fdd738fb5d95689b41428f9dee3aa0936`.
- G15G admission: `ASAHI_M3_EXPERIMENTAL=1`; driver prints `G15G admitted: chip 0x8122, 1 cluster, 10 cores; scheduling stalls on` and identifies `Apple M3 (G15G C0)`.
- **E1076 Compute:** `65536 invocations x 256 steps`, `788 runs in 1.0 s`, `0 wrong results`; kernel first Compute completion `773625 ns` GPU time.
- **E1077 Render:** `64x64 clear + 48x48 triangle`, `1174 runs in 1.0 s`, `0 wrong pixels`; kernel first Render completion `127875 ns` GPU time with non-zero TA and fragment timing.
- E1077 private result SHA-256: `db13fb493401d0e1eba7ff397752d0dbc59d7fb46e23f926135a6f7c0aae8f58`.
- **E1078 exact GPU readback:** 16384 bytes = 64x64x4 RGBA8; triangle bbox `x=8..55, y=8..55`; raw SHA-256 `949b6175a756379bf58f27c4543a1f15a294b259c54e9a0464fe860605e309cd`.
- **E1078 panel bridge:** exact GPU-produced pixels converted to panel `x2r10g10b10`; intended image and active framebuffer readback both SHA-256 `23f4bf863f03c29bcc2832c625edaadd20f2e5cb656015b494358df2415898dc`; original framebuffer and restored framebuffer both SHA-256 `1b86724c5caa46dc35dfca1d5a9976e9d43810c071422facc0c28e076ee2db95`.
- E1078 private result SHA-256: `196cef3d28007c10c2fcdacf810bdd9fc19b20d6221e5fd29a257954daa6389b`.

## Display distinction

The current experimental boot has the internal panel owned by simpledrm/simple-framebuffer (`card0-Unknown-1`, connected, 2880x1800) while Asahi is a separate render-only DRM device. A bounded `vkcube --wsi display --c 1` using the qualified G15G driver returns `Cannot find any display!` and submits no GPU job.

Therefore E1078 is intentionally classified as:

`GPU Render -> verified CPU-visible readback -> CPU copy -> active simpledrm framebuffer`

It is **not** native Asahi/DCP/KMS scanout.

## Next action

Work the display layer, not the GPU frontend: identify/enable the narrowest native DCP/KMS ownership and GPU-buffer presentation path for J615. Only after that move to compositor/desktop integration and longer-run qualification.

Do **not** reopen instruction-0, token5→token7, generic Queue/RunCompute/UAT/PagePool/power/parser/MMIO hypotheses unless contradictory evidence appears. E1076/E1077 crossed real Compute and Render.

## Historical evidence

E972/E974/E989–E1005 remain valid for the older E933/E950-style failing lineage. Keep them as lineage-specific negatives; do not delete or reinterpret them as the current wall.

## Recovery

The Mac is back on Golden Linux `7.1.6-ubuntu-m3-usbpd-gc5037a961e4d`, boot ID `dbb9862d-d4fa-4e33-beec-0afecf0feaf8`, known-good m1n1 and original GRUB hash-verified, with no pending one-shot entry, render node or live-owner marker.
