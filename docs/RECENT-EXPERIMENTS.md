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
| E280 | IN PROGRESS / bridge ABI closed | no source delta | Exact 23J220 MTLCompiler service/plugin request bridge recovered; later work moved the boundary beyond this early hypothesis. |
| E428 | LIVE PASS / module milestone | exact Golden-ABI external module | Binds J615/G15G C0, creates DRM/render nodes, reaches persistent manager/RTKit/Compute-ready. |
| E451/E463 | CONTROL PASS | marker-only + byte-identical resident controls | Full init/control plane is stable; prior GET_PARAMS/QueueCreate stalls were instrumentation/timing perturbations. |
| E475 | LIVE PASS | terminate-only Compute | Generic queue publication, scheduler, slot allocation and completion machinery are viable. |
| E480/E481 | LIVE LOCALIZATION | non-perturbing snapshots + exact firmware parser | Real Launch stays `wptr=1/doneptr=0`; firmware reaches WFI after start-timestamp parsing. |
| E484 | LIVE NEGATIVE | one-variable post-bind ASID invalidate | Translation visibility is not the missing prerequisite. |
| E487 | LIVE PASS / discriminator | actual `g15_result` snapshot | Shader-body store never executes; blocker is pre-body execution/activation. |
| E503 | ORACLE + exact back-translation | current Metal System Trace + exact RTKit | Linux is already past Apple Compute KickStart-equivalent. |
| E505 | ORACLE | current macOS first-use allocation | First real Compute triggers one-time ~47 MiB Wire allocation; empty/later Compute do not. Structural clue only. |
| E506 | STATIC EXACT PASS | exact 23J220 UMAPool helpers | Selected minimal diagnostic has zero target UMA min/ideal growth; do not synthesize ~40 MiB pool backing. |
| E507 | ORACLE | current AGX spill/UMA LLDB | Spill sizing executes twice per real Compute and not empty; current descriptors repeat and are not target ABI. |
| E508 | STATIC EXACT PASS | exact 23J220 producer back-translation | Current spill values do not reopen target UMA; next gate is pre-body program/USC or non-UMA first-use activation. |

## Current comparison boundary

E199 tells us what is *not* broken: queue transport, RunCompute publication, firmware retirement and WorkQueue completion all work for terminate-only Compute.

E274 tells us where a real launch currently stops: after scheduler acceptance but before engine completion.

E279 proves the production `ComputeProgramVariant` entry is generated and state-dependent. E280 now seeks the exact 23J220 generated entry/body oracle before changing more envelope fields.

## 2026-10-07 comparison boundary

The preferred current boundary is no longer E274. E487 proves the body never executes and E503 proves Linux is already past Apple KickStart-equivalent. E506/E508 close the tempting first-use UMA hypothesis for the exact selected 23J220 diagnostic. Continue at pre-body program/USC execution activation or another exact non-UMA first-use resource.
