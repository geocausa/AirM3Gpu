# Current G15 Bring-up State

Research state: **2026-10-25**

Target: MacBook Air M3 J615 / T8122 / G15G C0. Exact reference: macOS 14.8.3 / 23J220.

> Canonical resume point: `docs/CANONICAL-CHECKPOINT.md`.

## Headline

**J615 GPU execution, native Apple DCP/KMS display ownership, simultaneous GPU+display operation, and direct GPU-buffer native presentation are mechanically proven. The current frontier is compositor/window-system synchronization and repeated native presentation.**

## Closed repaired-lineage milestones

- **E1075:** exact E056-derived T8122 HwDataA `+0x418c..+0x41db` restores `firmware-running` and the render node.
- **E1076:** real Vulkan Compute/readback, `788 runs / 0 wrong`; kernel first Compute `773625 ns`.
- **E1077:** real Vulkan Render/readback, `1174 runs / 0 wrong pixels`; first Render `127875 ns`, non-zero TA/fragment timing.
- **E1078:** GPU triangle reaches the panel through a verified CPU-copy bridge. Useful historical milestone, but not native scanout.
- **E1086:** old simpledrm/Wayland presentation wall localized; GPU execution remained correct.
- **E1091:** native J615 Apple DCP/KMS takeover: PMP request `0x20080` acknowledged as `0x200f0`, DCP RTKit/DCPLink + PIODMA run, `apple-drm` registers, `eDP-1` is connected/enabled at 2880x1800@60, `fb0=appledrmfb`, simpledrm displaced.
- **E1092:** the exact E1091 display stack and qualified G15G runtime coexist on one boot. Vulkan Render completes `988 runs / 0 wrong pixels`; first Render `168708 ns`; native display remains healthy.
- **E1093:** Honeykrisp exports a full-mode linear GPU DMA-BUF (offset 0, pitch 11520). Apple DRM PRIME-import/AddFB2 succeeds. The DCP oracle records `swaps 7 / active_fb 42 / imported 0 -> swaps 8 / active_fb 44 / imported 1 -> swaps 9 / active_fb 42 / imported 0` across present/restore. No CPU pixel map/copy is used.

E1092 private result SHA-256: `7d303dfa0e8e563fcefcc31fd2413be82ea07b0baa52c68138de5590f8151d0c`.

E1093 private result SHA-256: `1180520285949fb309aa2ca099ae48e5ebce6e014715ad308b1645c457ecac38`.

## Current boundary

`Compute/Render PASS + native DCP/KMS PASS + simultaneous operation PASS + direct imported GPU-buffer presentation PASS -> [native compositor/window-system + repeated fence/pageflip/vblank/VSYNC qualification] -> endurance/performance`

Use E1093 as the direct-presentation control. Do not reopen instruction-0, token5->token7, shader, TA/fragment, Queue/RunCompute/UAT/PagePool/power/parser/MMIO hypotheses without contradictory evidence.

## Safety state

After E1093 the machine was recovered to Golden Linux `7.1.6-ubuntu-m3-usbpd-gc5037a961e4d`, boot ID `db06411a-f5ab-450f-a799-8694ab6d9674`, exact known-good m1n1 and original GRUB hash-verified, Ubuntu persistent/next boot, empty GRUB one-shot, no owner marker, no experimental render node and no Asahi module loaded.
