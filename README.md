# AirM3Gpu

Experimental clean-room bring-up and reverse-engineering work for Apple M3 / T8122 J615 G15G C0 on Linux.

This repository is the canonical public research checkpoint. Compilable Linux implementation history lives in `geocausa/linux`; raw proprietary and machine-local evidence remains private.

## Current state — 2026-10-24

A real same-J615 startup breakthrough has now been demonstrated.

**E1075** restores the exact 20-u32 T8122 HwDataA block at `+0x418c..+0x41db` that was originally recovered from Apple's J615/C0 producer in **E056**. With that single change, the previously unread first DeviceControl command is consumed, firmware reaches `magic=0xabcdabcd`, the kernel reports `firmware-running`, and `/dev/dri/renderD128` appears. No user GPU job was submitted in E1075.

Separately, the older mature execution line is now semantically localized much more tightly than the former generic “before opcode 0” description. **E972** proves Apple and Linux both open the same outer ESL slot, but Apple immediately publishes token7 carrying the initial ESL PC while Linux does not. **E974** proves parser state `0x61` is simply ESL slot 1. E989–E1005 then show this is not RLD-specific: valid LoadShader paths on working Apple cross the same token5→token7→token6 lifecycle, while Linux opens token5 and never publishes token7.

So the project currently has two distinct facts:

1. **Startup/runtime:** E1075 materially advances J615 to running firmware + DRM render node.
2. **Useful state-operation execution:** the unresolved hardware edge is still the generic outer-ESL state-operation accept/commit transition represented by `token5 -> token7(initial PC)`.

The next bounded live gate is **E1076**: boot the already-proven E1075 kernel with only built-in `g15_debug` bit 47 enabled, causing exactly one empty Compute self-test with a 2-second poll ceiling. E1076 has been staged/documented but **not run** because the remote reboot action was blocked; the machine was restored to Golden boot state with no pending one-shot entry.

Start with `docs/CANONICAL-CHECKPOINT.md`, then `docs/NEW-CHAT-HANDOFF.md` and `docs/RECENT-EXPERIMENTS.md`.

## Safety/status

This is research code, not general M3 GPU enablement. Keep Golden boot persistent, use one-variable bounded experiments, and do not confuse E1075 startup success with closure of the token5→token7 execution frontier.
