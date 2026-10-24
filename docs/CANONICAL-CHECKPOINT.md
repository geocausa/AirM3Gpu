# Canonical project checkpoint — J615 / G15

Updated: **2026-10-24**

This file is the single resume point for the AirM3Gpu project. Older “current frontier” statements are historical only.

## Target and authority

- Hardware: MacBook Air M3, J615 / T8122, GPU G15G C0, 10/10 cores.
- Exact ABI authority: macOS 14.8.3 / 23J220 plus matching retained G15G C0 components.
- Persistent safety baseline: Golden Linux `7.1.6-ubuntu-m3-usbpd-gc5037a961e4d`.
- Raw captures, proprietary binaries and exact machine-specific evidence remain private.

## 1. Current headline — E1076 real Compute pass

**E1076 is a live hardware execution/readback pass on the physical J615.**

Qualified runtime:

- Linux source: `geocausa/linux:wip/g15-e1075-hwdata418c @ a4778ca2a3f4791245cf1f7f42f26ccf2b17f5f7`.
- Kernel: `7.1.12-e1075-hwdata418c-ga4778ca2a3f4`.
- E1075 startup correction: exact Apple-derived T8122 HwDataA `+0x418c..+0x41db` 20-u32 image originally recovered in E056.
- Exact identity-capable boot handoff was used for the live discriminator.
- Firmware reached `magic=0xabcdabcd runtime=1 newA=1/1 newB=0`, reported `firmware-running`, and exposed `/dev/dri/renderD128`.
- Runtime was capped at pstate 1 / 338 MHz.

Userspace qualification:

- Ubuntu system Mesa was rejected before VM creation because it requested the old 592-byte GET_PARAMS ABI rather than the 600-byte M3-aware ABI.
- Retained ordinary `mesa-m3` was M3-aware but explicitly lacked the G15 command stream.
- The isolated Aurora experimental `mesa-m3-g15c 26.1.4.g15c1-2` package, SHA-256 `552c94de1a98a25cc5e0dbd9e3a4cf5fdd738fb5d95689b41428f9dee3aa0936`, contains the real G15G/AGX3 path.
- With only `ASAHI_M3_EXPERIMENTAL=1`, that driver printed `G15G admitted: chip 0x8122, 1 cluster, 10 cores; scheduling stalls on` and identified `Apple M3 (G15G C0)`.

Decisive one-second compute/readback:

`65536 invocations x 256 steps; 788 runs; 0 wrong results`

Kernel independent completion evidence:

`M3 G15G verdict: job-completed: the first compute job finished ... 773625 ns of GPU time`

**Interpretation:** the repaired E1075/E1076 lineage crosses real command construction, VM/queue submission, firmware scheduling, G15 state-operation/frontend admission, shader execution, retirement and CPU-visible readback. The current project frontier is no longer “before instruction 0.”

## 2. Why E1075 mattered

E1075 is the causal startup prerequisite for the successful E1076 lineage. It restores the exact T8122 HwDataA block:

`1, 500, 5, 12500x5, 3000x3, 12500x8, 1`

Without that block the reconstructed E1065 runtime reached q21 firmware-ready and published DeviceControl entry 1, but firmware never consumed it and timed out. With the block, DeviceControl is consumed, readiness advances to `0xabcdabcd`, firmware runs and DRM registers.

This is direct proof that the months-old E056 Apple-side reduction was not merely descriptive: it fixed a real J615 runtime barrier.

## 3. Historical execution localization — E972/E974/E989–E1005

These results remain valid for the older failing lineage and explain what that lineage was doing:

- **E972:** Apple and Linux match semantically through `token11(subtype5) -> token5(mode1/raw1)`; Apple then emits token7 carrying the initial outer-ESL PC while Linux never does.
- **E974:** parser state `0x61` is ESL slot 1 in the reserved `0x60..0x67` namespace, not a hidden hardware mode.
- **E989–E1005:** the same token5→token7→token6 lifecycle appears for working RLD/LoadDev and valid LoadShader operations, proving the old failure was generic state-operation admission/commit rather than RLD-specific.

Do **not** erase those negatives. Reclassify them as lineage-specific: they describe E933/E950-style failing runtime/command experiments, not the repaired E1075/E1076 path that now executes Compute correctly.

## 4. E1076 false starts that are now closed

- The old `G15Debug::SelfTest` bit47 hook belongs to the inactive legacy manager backend, not the active M3 runtime backend. Its absence is not a GPU failure and it should not be used as the current qualification gate.
- A boot using the ordinary known-good boot.bin stopped at the earlier identity/ABI gate; repeating with the exact E1075 qualified handoff reproduced `firmware-running` and render-node creation.
- Ordinary Mesa paths failed before GPU submission for explicit ABI/command-stream reasons. Those failures are userspace qualification results, not hardware execution failures.

## 5. Immediate next gate

Use the same qualified E1075 runtime and isolated G15G-capable userspace stack for **one bounded render/readback discriminator**. Preserve the 338 MHz cap and the same recovery envelope. If render completes with correct output/readback, move directly to the smallest visible triangle.

Do not restart broad Queue/RunCompute/UAT/PagePool/power/parser/MMIO searches unless new contradictory evidence appears. Real Compute has crossed the old frontier.

## 6. Recovery state

After E1076, the machine was restored and Fabric reboot reconciliation confirmed:

- Golden kernel: `7.1.6-ubuntu-m3-usbpd-gc5037a961e4d`.
- Known-good m1n1 `boot.bin` SHA-256: `a81993b122b7f990bc99ccab42decab7ef92adb94741dadf0ee79d24fdda3e0d`.
- Original `/boot/grub/custom.cfg` SHA-256: `cf011eee536420c6b32fa5b311345f507ae260d06c7f30777e2951e683b73462`.
- No experimental one-shot GRUB entry remains armed.

## Evidence pointers

- Private E1076 result: `/home/macmac/m3-gpu-lab/experiments/E1076-e1075-empty-compute-selftest/RESULT.md`.
- Private E1076 result SHA-256: `ed0f8e6166310f017713c4b9215d897bf3a3a386c116f81030d0b2e55f395f1b`.
- E1075 result: `experiments/E1075-t8122-hwdata-418c-positive-control/RESULT.md`.
- E972 result: `experiments/E972-rld-core0-semantic-sequence/RESULT.md`.
- E974 result: `experiments/E974-rld-state61-parser-semantics/RESULT.md`.
