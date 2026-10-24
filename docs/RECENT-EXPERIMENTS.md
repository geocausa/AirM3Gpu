# Recent experiment index

Current authority: `docs/CANONICAL-CHECKPOINT.md`.

## 2026-10-24 — semantic localization and startup breakthrough

| Experiment | Evidence class | Result | Decision |
|---|---|---|---|
| E972 | Offline exact Xcode16.3 replay | Apple/Linux same-program streams match semantically through mode-1/raw-1 token5; Apple then emits token7 with initial outer-ESL PC, Linux never emits token7 | First execution-semantic divergence is `token5 -> token7`; no earlier same-USC execution token is missing. |
| E974 | Exact Xcode16.3 static semantics | Parser state `0x61` = ESL slot 1 in reserved `0x60..0x67` bank | Close “hardware state 0x61” interpretation; hunt producer event, not parser control bit. |
| E981-E984 | Apple positive + Linux twin | Working Apple executes RLD then reaches poisoned LoadShader fault; Linux stalls after RLD and parser sees token5=1/token7=0/token6=0 | Confirms no post-RLD opcode advance on Linux. |
| E985-E987 | Linux NOP-prefix LoadShader | Linux still token5-only; E987 proves token5 is generic ESL slot-open, not RLD-specific | RLD-specific token5 theory closed. |
| E989 | Working Apple LoadShader positive | No RLD; valid LoadShader+STOP produces token5→token7→token6 and initial PC | Token7 is a generic state-operation progress event. |
| E993 | Raw-wire comparison | RLD and LoadShader converge on identical token5 + inline token7 publication path | Common frontend event identified observationally. |
| E995-E996 | Linux valid LoadShader | Valid mapped LoadShader still produces token5=1/token7=0/token6=0 | Linux defect is not malformed RLD or invalid LoadShader form. |
| E997-E1001 | Apple/Linux counter fingerprint | Working state operations have a common token7-covarying frontend pulse family; Linux lacks it even with 1-ms observer | Independent observer corroborates token7 boundary; not user-shader activity. |
| E1002 | Shared-memory transient oracle | No stable host-readable state distinguishes token7 crossing | Readable transient shared-memory route closed. |
| E1003/E1014/E1016/E1018-E1021 | Static/public/current-Apple audits | No surviving command-resident USC block, page-table publication bug, fresh RunCompute consumer, safe admission observer or private producer | No justified software field/bit to transplant blindly. |
| E1022-E1044 | Independent Aurora/T8122 positive-control reconstruction | Reconstructs an alternative current runtime/boot lineage and gets through identity/startup far enough to expose concrete startup faults | Opens a second route to qualify J615 runtime state. |
| E1045-E1048 | MTR + HwData atomic fixes | Exact T8122 corrections sequentially clear MTR fatal and atomic-alignment fault | Real startup progression; exposes DeviceControl timeout. |
| E1050-E1057 | HwDataB/IOMAP/SGX/startup discriminators | Values are proven live but do not clear first DeviceControl timeout | Clean negatives; retain exact corrections where appropriate. |
| E1059 | Live ring observer | First DeviceControl entry is published but never consumed | Startup failure localized pre-dispatch. |
| E1061/E1063/E1065 | Power/order/cache A/Bs | Linux-owned power, q21 ordering and cached DeviceControl ring all leave opcode unread | These hypotheses closed. |
| E1066/E1069/E1072 | Working macOS oracles | Recover first-command wake/entry/index/address-class details | Useful exact differentials, individually insufficient in Linux. |
| E1071 | Live ABI correction | 64-byte Apple DeviceControl slot is a real ABI correction but alone still unread | Necessary-looking correction, not causal alone. |
| **E1075** | **Same-J615 live causal pass** | Restore exact E056-derived T8122 HwDataA `+0x418c..+0x41db` block; firmware consumes DeviceControl, reaches `magic=0xabcdabcd`, DRM render node appears | **Startup breakthrough.** Current J615 runtime is now open for bounded job qualification. |
| E1076 | Staged only | Unchanged E1075 + built-in one-empty-Compute self-test bit; reboot blocked by remote control safety layer | **NOT RUN.** Boot state rolled back to Golden; this is the next live gate. |

## Current interpretation

The months of Apple-side reduction produced a directly causal startup fix: E056's exact J615 HwDataA block is what allowed E1075 to cross the reconstructed runtime's first DeviceControl barrier.

That does not erase the older execution-semantic frontier. For useful complete state operations the best localization remains:

`outer ESL slot-open(token5) -> [autonomous state-operation accept/commit/retire edge missing] -> no initial-PC token7 -> no token6 -> no next-opcode progress`

The practical next sequence is therefore: **E1076 one empty Compute -> smallest userspace compute/readback -> bounded render/readback -> first triangle**.

Historical pre-E718 experiments remain archived in `docs/HISTORICAL-EXPERIMENTS-THROUGH-E717.md`.
