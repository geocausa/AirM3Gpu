# Recent experiment index

Current authority: `docs/CANONICAL-CHECKPOINT.md`.

## 2026-10-24 — from semantic localization to real J615 Compute

| Experiment | Evidence class | Result | Decision |
|---|---|---|---|
| E972 | Offline exact Xcode16.3 replay | Apple/Linux same-program streams match semantically through mode-1/raw-1 token5; Apple then emits token7 with initial outer-ESL PC, Linux never emits token7 | Historical failing-lineage first semantic divergence is `token5 -> token7`. |
| E974 | Exact Xcode16.3 static semantics | Parser state `0x61` = ESL slot 1 in reserved `0x60..0x67` bank | Close “hardware state 0x61” interpretation. |
| E989-E1005 | Apple/Linux state-op controls | Working RLD/LoadDev and valid LoadShader share token5→token7→token6; older Linux lineage remains token5-only | Old failure is generic state-operation admission/commit, not RLD-specific. |
| E1022-E1044 | Independent Aurora/T8122 reconstruction | Reconstructs a second runtime/boot lineage and exposes concrete startup faults | Opens current J615 runtime path. |
| E1045-E1072 | Startup discriminators/oracles | MTR, HwData, IO-map, DeviceControl and Apple-oracle work localize startup; many clean negatives | Preserve exact corrections; do not reopen closed hypotheses. |
| **E1075** | **Same-J615 live causal pass** | Restore exact E056-derived T8122 HwDataA `+0x418c..+0x41db`; DeviceControl is consumed; `magic=0xabcdabcd`; firmware-running; DRM render node appears | **Startup barrier crossed.** |
| E1076-A | Boot-envelope control | E1075 kernel with ordinary boot.bin stops at earlier identity/ABI gate | Not a Compute failure; exact E1075 handoff is required. |
| E1076-B | Runtime qualification | Exact E1075 handoff reproduces firmware-running + render node. Old bit47 self-test is discovered to live in inactive manager backend | Drop bit47 as current qualification gate. |
| E1076-C | Userspace qualification | System Mesa requests old 592-byte GET_PARAMS; ordinary retained mesa-m3 lacks G15 command stream | Both fail before GPU submission; userspace mismatch, not hardware failure. |
| **E1076** | **Physical J615 live Compute/readback** | Isolated `mesa-m3-g15c` + `ASAHI_M3_EXPERIMENTAL=1` explicitly admits T8122/G15G. One-second compute-check: 65536 invocations × 256 steps, 788 runs, 0 wrong. Kernel reports first Compute completed in 773625 ns GPU time | **Current execution frontier moved past instruction 0 and real Compute retirement/readback.** Next: bounded render/readback, then triangle. |

## Current interpretation

The Apple-side reduction and later runtime reconstruction converged in a causal chain:

`E056 exact Apple HwDataA block -> E1075 DeviceControl/firmware-running -> G15G-aware userspace admission -> E1076 correct Compute execution/readback`

The older E972 token5→token7 boundary remains important historical evidence, but it is no longer the current repaired-lineage wall.

## Next sequence

**bounded render/readback -> smallest visible triangle -> only then broader desktop/long-run qualification**.

Historical pre-E718 experiments remain archived in `docs/HISTORICAL-EXPERIMENTS-THROUGH-E717.md`.
