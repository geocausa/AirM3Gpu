# New-chat handoff — J615 / G15

Generated: **2026-10-24**

**First read:** `docs/CANONICAL-CHECKPOINT.md`.

## One-sentence state

**E1076 proves real correct userspace Compute on the physical J615/G15G C0:** the repaired E1075 runtime reaches firmware-running, a G15G-capable Honeykrisp build explicitly admits T8122, and a one-second compute/readback completes 788 runs with zero wrong results while the kernel independently reports first Compute completion.

## Latest hard facts

- E1075 Linux source: `geocausa/linux:wip/g15-e1075-hwdata418c @ a4778ca2a3f4791245cf1f7f42f26ccf2b17f5f7`.
- E1075 causal startup state: exact E056-derived T8122 HwDataA `+0x418c..+0x41db` 20-u32 image.
- E1075 live result: DeviceControl consumed; readiness `magic=0xabcdabcd`; `firmware-running`; `/dev/dri/renderD128` present.
- E1076 userspace: isolated `mesa-m3-g15c 26.1.4.g15c1-2`, package SHA-256 `552c94de1a98a25cc5e0dbd9e3a4cf5fdd738fb5d95689b41428f9dee3aa0936`.
- G15G admission gate: `ASAHI_M3_EXPERIMENTAL=1`.
- Driver admission: `G15G admitted: chip 0x8122, 1 cluster, 10 cores; scheduling stalls on`.
- Device: `Apple M3 (G15G C0)` / Honeykrisp.
- Compute/readback: `65536 invocations x 256 steps`, `788 runs in 1.0 s`, `0 wrong results`.
- Kernel: first compute job completed, GPU time `773625 ns`.
- Private E1076 result SHA-256: `ed0f8e6166310f017713c4b9215d897bf3a3a386c116f81030d0b2e55f395f1b`.

## Important correction to older frontier

E972/E974/E989–E1005 remain valid for the historical failing E933/E950 lineage: Linux opened outer ESL token5 but did not emit token7/initial PC. **That is no longer the current repaired-lineage frontier.** E1076 crosses real state-operation/frontend admission and executes/retires correct Compute work.

Also do not use old `G15Debug::SelfTest` bit47 as the current gate: that hook belongs to the inactive legacy manager backend, not the active M3 runtime backend.

## Next action

Do **one bounded render/readback** with the same qualified E1075 runtime + isolated G15G userspace, keeping the 338 MHz cap and known recovery envelope. If correct, move directly to the smallest visible triangle.

Do not return to broad Queue/RunCompute/UAT/PagePool/power/parser/MMIO searches unless new contradictory evidence appears.

## Recovery

The machine is back on Golden Linux `7.1.6-ubuntu-m3-usbpd-gc5037a961e4d`. Known-good m1n1 and original GRUB were hash-verified, and Fabric reconciled the recovery reboot successfully.
