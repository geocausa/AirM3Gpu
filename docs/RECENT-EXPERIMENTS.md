# Recent experiment index

This index covers the execution-focused phase after generic J615 Compute completion was first proven. Detailed raw evidence remains in the private/local lab; this file records only the sanitized conclusion and source checkpoint.

| Experiment | Result | Kernel checkpoint / role | Decision |
|---|---|---|---|
| E199 | LIVE PASS | terminate-only exact-target Compute | Generic Compute/RunCompute/firmware/WorkQueue completion is proven. |
| E260 | STATIC PASS | `f23eedf6f154` | Ordinary non-ray late resource state resolves to zero/default; keep `0x107a0=0x00ff0000`. |
| E261 | STATIC PASS | no source delta | RTKit appends the required G15 Compute RegisterArray tail; do not duplicate it in Linux. |
| E262 visibility | STATIC PASS | no source delta | Ad-hoc CPU cache maintenance for range-5 code is not justified. |
| E263 | STATIC PASS / LIVE FAIL | `4f23178ea220` | Keep exact terminate pointer at final terminate dword; correction alone is insufficient. |
| E264 | STATIC PASS | no source delta | 0x1000/0x4000 executable-suballocation alignment requirement rejected. |
| E265 | STATIC PASS / LIVE FAIL | `2eb1b82b2440` | Full hand-written G15 entry/state sequence is a real discrepancy but not sufficient. |
| E266-E270 | STATIC closures | no forward live delta | Launch metadata/selectors/direct grammar closed; production dword3 remains `0x40000000`. |
| E272 | LIVE FAIL | `817fc96dbf4a` | Separate result allocation alone does not fix engine completion. |
| E273 | LIVE FAIL | `1ba304e5c27f` | Matching result VA/PTE class alone does not fix engine completion. |
| E274 | LIVE FAIL, stable timeout | `bcc062a1c864` | **Preferred real-launch baseline**: scheduler accepts; engine completion never arrives. |
| E275 | LIVE FAIL, reset class | `467a31bc53a8` | Splitting CDM from shader storage regresses failure class; do not carry forward. |
| E276 | LIVE FAIL, reset class | `1c0ec35ca0e7` | Hand-written `0x40` launch control rejected; production `0x40000000` remains preferred. |
| E277 | STATIC PASS | no source delta | Keep preemption/data-buffer backing in command/DataBuffer storage family. |
| E278 | LIVE FAIL, reset class | `f8306c6f90b0` | Separate CDM in shared-RW also regresses; reject unchanged. |
| E279 | STATIC PASS | no source delta | Production entry is generated/state-dependent; neither manual entry sequence is a universal 23J220 oracle. |
| E280 | IN PROGRESS / oracle only | no source delta | Obtain exact 23J220 compiler/driver-generated minimal Compute entry bytes and metadata. |

## Current comparison boundary

E199 tells us what is *not* broken: queue transport, RunCompute publication, firmware retirement and WorkQueue completion all work for terminate-only Compute.

E274 tells us where a real launch currently stops: after scheduler acceptance but before engine completion.

E279 proves the production `ComputeProgramVariant` entry is generated and state-dependent. E280 now seeks the exact 23J220 generated entry/body oracle before changing more envelope fields.
