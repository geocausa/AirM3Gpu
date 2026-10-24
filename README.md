# AirM3Gpu

Experimental clean-room bring-up and reverse-engineering work for Apple M3 / T8122 J615 G15G C0 on Linux.

This repository is the canonical public research checkpoint. Compilable Linux implementation history lives in `geocausa/linux`; raw proprietary and machine-local evidence remains private.

## Current state — 2026-10-24

**E1076 is the first mechanically verified correct userspace Compute execution on the physical J615.**

The enabling startup fix is E1075: restore the exact 20-u32 T8122 HwDataA block at `+0x418c..+0x41db` originally recovered from Apple's J615/C0 producer in E056. That moves the reconstructed runtime from an unread first DeviceControl command to `firmware-running` with `/dev/dri/renderD128` present.

E1076 then qualified the actual userspace path. The ordinary distro Mesa is too old for the 600-byte M3-aware GET_PARAMS ABI, and retained ordinary `mesa-m3` explicitly lacks the G15 command stream. An isolated, SHA-256-pinned `mesa-m3-g15c` build contains the real G15G/AGX3 path. With only its documented experimental admission gate `ASAHI_M3_EXPERIMENTAL=1`, it explicitly admitted chip `0x8122`, 1 cluster, 10 cores and identified `Apple M3 (G15G C0)`.

A bounded one-second compute/readback then passed:

`65536 invocations x 256 steps; 788 runs; 0 wrong results`

The kernel independently reported:

`M3 G15G verdict: job-completed: the first compute job finished ... 773625 ns of GPU time`

This supersedes the old project-wide statement that current Linux never reaches instruction 0. That statement remains historically valid for the older E933/E950 token5-only lineage; it is not the current repaired E1075/E1076 frontier.

**Next gate:** one bounded render/readback using the now-qualified G15G stack, then the smallest visible triangle if render is clean.

The machine was restored after E1076 to Golden Linux `7.1.6-ubuntu-m3-usbpd-gc5037a961e4d`, known-good m1n1, and the original GRUB configuration.

Start with `docs/CANONICAL-CHECKPOINT.md`, then `docs/NEW-CHAT-HANDOFF.md` and `docs/RECENT-EXPERIMENTS.md`.
