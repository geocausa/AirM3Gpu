# AirM3Gpu

Experimental clean-room bring-up and reverse-engineering work for Apple M3 / T8122 J615 G15G C0 on Linux.

This repository is the canonical public research checkpoint. Compilable Linux implementation history lives in `geocausa/linux`; raw proprietary and machine-local evidence remains private.

## Current state — 2026-10-24

The project has now crossed the first real graphics milestone on the physical MacBook Air M3.

- **E1075** restores the exact Apple-derived T8122 HwDataA `+0x418c..+0x41db` block and moves the reconstructed J615 runtime from an unread first DeviceControl command to `firmware-running` with `/dev/dri/renderD128` present.
- **E1076** proves real correct Vulkan Compute/readback on J615: `65536 invocations x 256 steps`, `788 runs in 1.0 s`, `0 wrong results`; the kernel independently reports the first Compute job completed.
- **E1077** proves real Vulkan graphics execution and triangle readback: `64x64 clear + 48x48 triangle`, `1174 runs in 1.0 s`, `0 wrong pixels`; the kernel independently reports the first Render job completed with non-zero TA and fragment time.
- **E1078** captures the exact 64x64 GPU-produced triangle readback, converts those pixels to the active internal panel's `x2r10g10b10` simple-framebuffer format, writes them to `/dev/fb0`, verifies the framebuffer byte-for-byte, then restores the original console byte-for-byte.

The current frontier is therefore **not** instruction-0 admission, Compute, or basic Render. Those are crossed on the repaired E1075/E1077 lineage. The next major layer is **native DCP/KMS/display ownership and GPU-buffer presentation**. E1078 is a deliberate CPU-copy bridge from a working GPU render path to the existing simpledrm panel framebuffer; it is not yet native Asahi scanout.

Historical E972/E974/E989–E1005 token5→token7 failures remain valid for the older E933/E950-style lineage and must not be erased, but they are no longer the current repaired-lineage wall.

The machine was restored after E1077/E1078 to Golden Linux `7.1.6-ubuntu-m3-usbpd-gc5037a961e4d`, known-good m1n1, original GRUB, no pending one-shot entry, and no experimental owner marker.

Start with `docs/CANONICAL-CHECKPOINT.md`, then `docs/NEW-CHAT-HANDOFF.md` and `docs/RECENT-EXPERIMENTS.md`.
