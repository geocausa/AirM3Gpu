# New-chat handoff — J615 / G15

Generated: **2026-10-24**

**First read:** `docs/CANONICAL-CHECKPOINT.md`.

## One-sentence state

J615 now has a genuine startup breakthrough: E1075 restores the exact E056-derived T8122 HwDataA `+0x418c` block and moves the current runtime from first-DeviceControl timeout to running firmware + DRM render node; useful state-operation execution is still blocked at the hardware edge observed as outer-ESL `token5 -> token7(initial PC)`.

## Latest hard facts

- **E1075 LIVE PASS:** firmware reaches `magic=0xabcdabcd`; `/dev/dri/renderD128` exists; no fault/timeout; no user GPU job submitted.
- Linux source: `geocausa/linux:wip/g15-e1075-hwdata418c @ a4778ca2a3f4791245cf1f7f42f26ccf2b17f5f7`.
- The causal 20-u32 block was recovered months earlier in **E056** from Apple's J615/C0 `populateDPELeakageUpdateConfig` path. E1075 proves that reduction was directly useful.
- **E972:** same-program Apple/Linux semantic stream is identical through ESL slot-open token5; Apple immediately emits token7 with the initial outer-ESL PC, Linux never does.
- **E974:** parser state `0x61` = ESL slot 1 (`0x60 | raw_index`), not a hardware enable state.
- **E989–E1005:** valid LoadShader reproduces the same Apple token5→token7→token6 lifecycle and Linux token5-only failure, so the problem is generic state-operation commit/admission, not RLD-specific.

## Next action

**E1076:** unchanged E1075 kernel + only `asahi.g15_debug` bit 47. Built-in self-test submits one empty Compute job and polls for no more than 2 seconds.

E1076 has **not run**. The remote control layer blocked the reboot. Its temporary one-shot GRUB state was rolled back completely; Golden remains the persistent/current boot and `next_entry` is clear.

When a controlled reboot is available, run E1076 first. If it retires, immediately advance to the smallest userspace compute/readback. If compute is clean, the next meaningful visual milestone is a bounded render/readback/triangle.

## Do not repeat

Do not re-open broad Queue/RunCompute/UAT/PagePool/power/parser/profiler sweeps. Do not call E972 output missing. Do not call `0x61` a hardware state. Do not treat E1075 as proof that J615 shader execution is solved.
