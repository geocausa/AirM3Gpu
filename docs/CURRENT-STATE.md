# Current G15 Bring-up State

Research state: **2026-10-24**

Target: MacBook Air M3 J615 / T8122 / G15G C0. Exact reference: macOS 14.8.3 / 23J220.

> Canonical resume point: `docs/CANONICAL-CHECKPOINT.md`.

## Headline

**E1076 is the newest live advancement and the first correct J615 userspace Compute/readback pass.**

E1075 first repaired startup by restoring the exact E056-derived T8122 HwDataA `+0x418c..+0x41db` 20-word image. With the exact qualified handoff, firmware reaches `magic=0xabcdabcd`, reports `firmware-running`, and exposes `/dev/dri/renderD128`.

E1076 then used an isolated G15G-capable `mesa-m3-g15c` userspace. `ASAHI_M3_EXPERIMENTAL=1` caused the driver to explicitly admit chip `0x8122`, 1 cluster, 10 cores and identify `Apple M3 (G15G C0)`.

Bounded compute/readback result:

`65536 invocations x 256 steps; 788 runs in 1.0 s; 0 wrong results`

Kernel completion evidence:

`M3 G15G verdict: job-completed: the first compute job finished ... 773625 ns of GPU time`

The current execution frontier is therefore **past instruction 0 and past real Compute retirement/readback** on the repaired E1075/E1076 lineage.

## Historical boundary retained

E972/E974/E989–E1005 remain valid descriptions of the older failing E933/E950-style lineage: common ESL token5 followed by missing Linux token7/initial PC. Do not delete or reopen those experiments, but do not present that boundary as current for E1076.

## Next bounded gate

Run one bounded **render/readback** on the same qualified E1075 runtime and isolated G15G userspace stack, retaining the 338 MHz cap and exact recovery path. If render/readback is correct, move to the smallest visible triangle.

## Safety state

E1076 was fully recovered after the test. Fabric confirmed Golden Linux `7.1.6-ubuntu-m3-usbpd-gc5037a961e4d`, known-good m1n1 SHA `a81993b...`, original GRUB SHA `cf011eee...`, and no experimental one-shot entry.
