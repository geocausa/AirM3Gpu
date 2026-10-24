# New-chat handoff — J615 / G15G

Generated: **2026-10-25**

**First read:** `docs/CANONICAL-CHECKPOINT.md`.

## One-sentence state

**The physical J615 now has mechanically verified G15G Compute/Render, native Apple DCP/KMS, simultaneous GPU+display operation, and direct GPU-produced DMA-BUF presentation through native DCP; the remaining major wall is compositor/window-system synchronization and repeated native presentation.**

## Hard facts

- E1075 startup: exact T8122 HwDataA `+0x418c..+0x41db` plus identity-capable handoff reaches `firmware-running` and `renderD128`.
- E1076 Compute: `788 runs / 0 wrong`; first Compute `773625 ns`.
- E1077 Render: `1174 runs / 0 wrong pixels`; first Render `127875 ns`.
- E1091 native display: PMP request `0x20080` -> ack `0x200f0`; `apple-drm`, `eDP-1`, 2880x1800@60, `appledrmfb`; Asahi intentionally blacklisted for the display-only proof.
- E1092 combined-device PASS: native `apple-drm/eDP-1` and G15G `firmware-running/renderD128` coexist; Vulkan Render `988 runs / 0 wrong pixels`; first Render `168708 ns`. Result SHA-256 `7d303dfa0e8e563fcefcc31fd2413be82ea07b0baa52c68138de5590f8151d0c`.
- E1093 direct-native-present PASS: GPU clears an exportable 2880x1800 linear DMA-BUF, Apple DRM PRIME-import/AddFB2 succeeds, and DCP records `7/fb42/imported0 -> 8/fb44/imported1 -> 9/fb42/imported0` across present/restore. No CPU pixel copy. Result SHA-256 `1180520285949fb309aa2ca099ae48e5ebce6e014715ad308b1645c457ecac38`.

## Current boundary

`G15G Compute/Render PASS + native DCP/KMS PASS + simultaneous operation PASS + direct imported GPU-buffer presentation PASS -> [native compositor/window-system + repeated sync/pageflip/vblank/VSYNC] -> endurance/performance`

## Next action

1. Keep E1092/E1093 as fixed controls; do not repeat them unless later work needs regression qualification.
2. Run one bounded native compositor/window-system test on `apple-drm` using the qualified G15G userspace path.
3. Record DCP `swaps/active_fb/imported`, pageflip/vblank/fence behavior and any WSI/compositor errors around each bounded present.
4. Only after repeated native presentation is mechanically stable, broaden into desktop acceleration and endurance/performance.

Do **not** reopen instruction-0, token5->token7, shader, TA/fragment, generic Queue/RunCompute/UAT/PagePool/power/parser/MMIO hypotheses without contradictory evidence.

## Recovery

Golden Linux is active: `7.1.6-ubuntu-m3-usbpd-gc5037a961e4d`, boot ID `db06411a-f5ab-450f-a799-8694ab6d9674`, exact known-good m1n1 and original GRUB hash-verified, Ubuntu persistent/next, empty GRUB one-shot, no owner marker, no experimental render node.
