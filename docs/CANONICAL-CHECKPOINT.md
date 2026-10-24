# Canonical project checkpoint — J615 / G15

Updated: **2026-10-24**

This file is the single resume point for the AirM3Gpu project. Older “current frontier” statements are historical only.

## Target and authority

- Hardware: MacBook Air M3, J615 / T8122, GPU G15G C0.
- Exact ABI authority: macOS 14.8.3 / 23J220 plus matching retained G15G C0 components.
- Newer macOS/Xcode observations are structural/dynamic oracles unless back-translated.
- Persistent safety baseline: Golden Linux `7.1.6-ubuntu-m3-usbpd-gc5037a961e4d`.
- Raw captures and proprietary binaries remain machine-local.

## 1. Same-J615 startup breakthrough — E1075

E1075 is a **live causal pass**, not an oracle-only result.

Base: the E1065 negative runtime lineage, which reached q21 firmware-ready, published DeviceControl entry 1, but firmware never consumed it (`read=0/retired=0`) and timed out after two seconds.

Single changed variable: restore the exact 20-u32 T8122 HwDataA image at `+0x418c..+0x41db`:

`1, 500, 5, 12500x5, 3000x3, 12500x8, 1`

This block was originally recovered from Apple's exact J615/C0 producer in E056 (`populateDPELeakageUpdateConfig`). E1075 Linux source commit:

`geocausa/linux:wip/g15-e1075-hwdata418c @ a4778ca2a3f4791245cf1f7f42f26ccf2b17f5f7`

Live J615 outcome:

- InitData self-check passes;
- q21 firmware-ready appears;
- readiness moves to `magic=0xabcdabcd runtime=1 newA=1/1 newB=0` within milliseconds;
- kernel reports `firmware accepted owned initdata and device controls`;
- verdict is `firmware-running`;
- Asahi DRM registers and `/dev/dri/renderD128` appears;
- no GPU/DART/RTKit fault, panic, Oops or DeviceControl timeout in the captured boot;
- no user compute/render workload was submitted.

**Interpretation:** months-old exact Apple reduction directly fixed a newly reconstructed J615 runtime startup barrier. This is a real advancement. It does not by itself solve the older first-state-operation execution failure, because E980 proved the older E933/E950 RLD line already carried this block.

Independent public J613 hardware results using the same T8122 block report firmware start, first Compute and Render completion, GL/Vulkan readback and longer accelerated workloads. Treat that as strong external corroboration for the block, not as a substitute for J615 job qualification.

## 2. Exact execution-semantic frontier — E972/E974/E989–E1005

The former generic label “before instruction 0” is now too vague.

### E972 — first semantic Apple/Linux divergence

Same-program Apple E932 and Linux E934 core0 APS pages were replayed through the exact Xcode 16.3 `(15,4,5)` token decoder.

After excluding timestamp/counter cadence and submission identity, both systems produce the same pre-execution semantic sequence:

`token11(subtype 5) -> token5(mode=1, raw_index=1)`

Apple then immediately emits a 60-bit **token7** carrying the selected Launch/outer-ESL PC. Linux emits timing metadata instead and never emits token7 anywhere in the page.

There is no earlier same-USC execution/control token missing on Linux.

### E974 — state 0x61 named correctly

Xcode's parser constructs mode-1 state IDs as `0x60 | (raw_index & 7)`. Therefore `0x61` is simply **outer ESL slot 1** in the reserved `0x60..0x67` ESL-clique namespace. It is not a hidden hardware-enable value.

So the real observation is:

`outer ESL slot opened (token5) -> [missing hardware producer event] -> initial outer ESL PC published (token7)`

### E989–E1005 — RLD-specific theory superseded

Working Apple publishes the same token5→token7→token6 lifecycle for both RLD/LoadDev and complete valid LoadShader state operations. Linux opens token5 for both but publishes neither token7 nor token6. E999/E1000/E1001 also recover a common frontend pulse family that covaries with token7 on working Apple and remains absent on Linux.

The unresolved execution edge is therefore **generic to complete G15 entry-state/IOTO operations**, not one RLD opcode, addressing form, user shader, or parser quirk.

Current hard execution boundary:

`exact startup + exact VM/PTE + exact RunCompute/RegisterArray + common Compute activation + outer ESL slot-open(token5) -> [autonomous complete state-operation accept/commit/retire edge missing] -> no initial-PC token7 -> no slot token6 -> no next-opcode progress`

## 3. What later audits closed

E979/E1003/E1014/E1016/E1018–E1021 found no surviving host/RTKit/RegisterArray/UAT/parser/current-Apple private producer that can simply be transplanted to cross token5→token7. Broad re-opening of Queue, RunCompute layout, generic UAT/PTE, PagePool/FList/UMA, PMGR power, parser state IDs, profiler control, or blind MMIO is therefore not justified.

## 4. Immediate next gate — E1076

Use the **already live-proven E1075 kernel unchanged** and enable only built-in `asahi.g15_debug` bit 47 (`0x800000000000`). That self-test submits exactly one empty Compute/CL job after firmware init and polls its stamp for at most 2000 ms; it never makes probe success depend on the job.

E1076 was prepared in the private lab but was **not executed** because the remote reboot command was blocked by the control layer. No candidate boot or GPU work occurred. The temporary GRUB entry was removed and `next_entry` cleared; the machine remains on Golden.

If E1076 retires cleanly, proceed to the smallest userspace compute/readback. Only after compute is clean should a bounded render/readback (and then a first triangle) be attempted.

## Do not lose these distinctions

- **E1075 is a genuine startup breakthrough.**
- **E1075 is not proof of user shader execution on J615.**
- **E972 output is no longer missing.** Its result is the token5→token7 semantic boundary.
- **State 0x61 is not a hardware mode.** It is parser ESL slot 1.
- **The old RLD-only theory is obsolete.** The failure is common to complete state operations.
- **E1076 is not a result yet.** It is the next bounded live test.

## Repository / evidence pointers

- Public research: `geocausa/AirM3Gpu`.
- Linux implementation: `geocausa/linux`.
- Active private lab on Golden M3 Linux: `/home/macmac/m3-gpu-lab`.
- E1075 result: `experiments/E1075-t8122-hwdata-418c-positive-control/RESULT.md`.
- E972 result: `experiments/E972-rld-core0-semantic-sequence/RESULT.md`.
- E974 result: `experiments/E974-rld-state61-parser-semantics/RESULT.md`.
- E1076 staging record: `experiments/E1076-e1075-empty-compute-selftest/RESULT.md`.
