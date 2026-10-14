# G15 first-opcode activation boundary — E659 through E691

Research checkpoint: 2026-10-14

Target: MacBook Air M3 J615 / T8122 / G15G C0. Exact ABI authority remains macOS 14.8.3 / 23J220 plus matching retained G15G C0 firmware. Later macOS builds are used only as structural/dynamic oracles and are back-translated before changing Linux.

## Executive result

The current Linux bring-up now crosses every software-visible prerequisite that has been mechanically identified for the selected direct Compute workload:

`CDM root fetch -> valid Launch -> valid UMA/PagePool context -> translated and byte-visible ESL entry -> correct retained init/power state -> coherent c058 active-slot publication`

The surviving failure is narrower:

`c058 active slot -> [missing autonomous G15 frontend/state-loader/interpreter transition] -> first Apple-proven ESL opcode is not interpreted -> context-store/quiesce -> replay-1`

The key proof is an address-tagged first-opcode oracle. On working Apple hardware, a native-NOP prefix followed by a poisoned G15 LoadShader at an Apple-proven slot is consumed and produces the expected tagged fault. On Linux, the same structurally validated entry is fetched/visible but the poisoned LoadShader effect never occurs.

This checkpoint therefore no longer describes the problem as generic CDM-root, Launch-address, cache/coherency, scheduler, power, or replay failure.

## Material advances

### E659 — restore exact G15 startup state

A previously zero Linux field corresponding to exact physical HwDataB `+0xae4` must be `0x31`. Restoring it causes retained firmware to program the expected G15 startup register state and materially advances the real-Compute path into hardware/context activity that was previously unreachable.

This is a real ABI correction and remains part of the preferred lineage.

### E660 — persistent CL command-resource mapping

The persistent CL command resource must use the production protected/page-list mapping class already recovered for the G15 path. Correcting that mapping removes the newly exposed NoAccess fault and becomes the baseline for later experiments.

### E665–E667 — replay classified as early recovery

Firmware's Finalize replay-1 branch was identified without directly reading hardware-owned status registers. A command-memory marker written only by that branch proves Linux enters replay-1. Timing then bounds the transition to roughly 1–10 ms after activation: the Compute slot is already active before replay/context-store state appears.

Replay is therefore downstream recovery from the failed activation, not the missing activation primitive itself.

### E672/E673/E682/E683 — address/resource plane closed

A tagged primary-context experiment proves the post-activation path performs a CDM context-store write into the primary Compute preemption/context allocation. Exact producer analysis shows that storage is output of context-save rather than a CPU-authored prerequisite.

A paired experiment then proves the primary context-store write occurs before persistent CL scratch access. This reclassifies the earlier CL scratch traffic as downstream context/recovery activity.

Together with prior root, UAT/PTE, PagePool/FList/UMA, entry-translation, byte-visibility, DMA/coherency, preemption-backing and shader/program-target work, there is no remaining justified address-bearing object between valid ESL visibility and first-opcode interpretation.

### E684/E685 — retained G15 init/power register state closed

The retained firmware's G15-specific init/power register programming was audited end to end. Software-controlled fields are either exact or independently compatible with a working M3 path; preserved bits are deliberately hardware/status-owned. No remaining scalar or read-modify-write difference in that family survives as a justified activation candidate.

### E686 — replay status is not a frontend error decoder

The hardware status class selecting replay-1 is consumed as a generic lifecycle/replay state. The replay helpers do not inspect ESL bytes, Launch geometry, USC program state or a specific decode-error field. It is not evidence that can be renamed to a particular frontend fault.

### E687–E690 — platform power and retention closed

The J615 PMGR graph and GFX-domain behavior were re-audited. A late explicit GFX repower was insufficient, leaving a narrower lifecycle question: could Linux's automatic physical gating discard non-retentive state after firmware initialization?

The retained firmware power callback was then reconstructed. Its low-power-to-active path does replay substantial G15 hardware initialization, while Linux PMGR automatic gating can change physical actual state without a Linux domain off/on callback.

E690 tested this distinction directly without changing the GPU command. Physical GFX was held explicitly ACTIVE with automatic gating disabled from the first platform power-on through retained-firmware initialization and the unchanged first-opcode oracle. The result was unchanged: the poisoned LoadShader was still not consumed and the established context-store/replay signature returned.

Thus LMX0 parenting, GFX automatic gating, continuous physical power, and loss of retained init state across an automatic off interval are not the missing transition.

### E691 — logical quiesce is downstream firmware self-management

The later reason-5 context-management sweep is produced by retained firmware's own logical-power/background state machine when it encounters a still-active context. Apple's submission-time `GFX-BUSY` bookkeeping does not provide a missing firmware-visible scalar for this decision on J615.

Reason-5 handling therefore occurs because the command remains active after the autonomous frontend fails to make first-opcode progress. Suppressing or delaying it would only postpone recovery.

## Closed families

Absent contradictory evidence, do not reopen these as first-opcode root-cause families:

- Queue/WorkQueue transport, stamp registration and scheduler acceptance.
- CDM engine start, root fetch and control-stream placement.
- Launch geometry/mode and validated G15 ESL grammar/slot placement.
- RegisterArray ordering/count/tail and known exact producers.
- RunCompute/SKU command-local state already recovered for the selected path.
- HwDataB imported/direct runtime fields, including the real `+0xae4` correction.
- UAT/PTE, PagePool/FList/UMA, mapping ownership and cache/DMA publication.
- Primary preemption/context backing and persistent CL command-resource backing.
- Ad-hoc secure-cache mode changes without the separate Apple protected-context registration contract.
- Retained G15 startup/power RMW programming.
- Known successful-M3 direct-MMIO/public implementation deltas already audited.
- J615 LMX0/GFX PMGR topology, automatic gating and continuous physical power.
- Replay-state manipulation, reason-5 quiesce manipulation, or host GFX-BUSY bookkeeping.
- Existing firmware diagnostics/KTrace as a passive waypoint between active-slot publication and first-opcode interpretation; no such exact waypoint has been found.

## Current research rule

Do not spend another protected live cycle merely to timestamp recovery or perturb a closed family. The next live experiment requires one new mechanically exact discriminator.

The remaining search space is deliberately narrow: a **non-address, non-power, autonomous G15 frontend/context state** that exists after active-slot publication and before the first ESL instruction is interpreted.

Strong next directions are:

1. a working-M3/macOS oracle that exposes a concrete autonomous frontend state difference which can be back-translated to the exact 23J220 ABI;
2. exact-target hardware/context semantics not already represented by the closed resource, power, replay, queue or command families;
3. new public/upstream G15 work that reveals a previously absent state producer or hardware programming requirement.

This remains research/bring-up code, not a production driver. Runtime activation stays fail-closed outside bounded experiments.
