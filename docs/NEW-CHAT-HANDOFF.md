# New-chat handoff — J615 / G15 Compute bring-up

Generated: 2026-10-14

This is the public/sanitized continuation point. The private M3 lab retains detailed machine-local evidence; proprietary binaries, reverse-engineering databases, built modules and raw captures are intentionally not published here.

## Target and authority

- Physical target: MacBook Air M3 / T8122 / J615 / G15G C0.
- Persistent/default OS: Golden Ubuntu kernel `7.1.6-ubuntu-m3-usbpd-gc5037a961e4d`.
- Exact ABI authority: macOS 14.8.3 / build 23J220 plus matching retained G15G C0 firmware.
- Later macOS builds: structural/dynamic oracle only; back-translate every observation before changing Linux.

## Current execution frontier

The problem is no longer generic queue admission, CDM-root translation, Launch-address visibility, cache/coherency, power, context backing or replay. The current causal cut is:

`CDM root fetch -> valid Launch -> valid UMA/PagePool -> translated/byte-visible ESL entry -> correct retained init/power state -> coherent c058 active slot -> [missing autonomous G15 frontend/state-loader/interpreter transition] -> first Apple-proven ESL opcode not interpreted -> context-store/quiesce -> replay-1`

The decisive discriminator is a working-Apple first-opcode oracle. A native-NOP-prefix poisoned LoadShader at an observed-valid G15 slot is consumed on working Apple hardware and produces the expected tagged fault. Linux reaches the entry address/visibility boundary but never produces that first-opcode effect.

## Recent material milestones

- **E659:** exact physical HwDataB `+0xae4=0x31` restores required G15 startup programming and materially advances the hardware/context path.
- **E660:** persistent CL command resource corrected to the production protected/page-list mapping class.
- **E665-E667:** exact replay-1 proven via command-memory marker; onset bounded to roughly 1–10 ms after activation. Replay is downstream recovery.
- **E672/E673/E682/E683:** primary context-save and CL-scratch ordering reconstructed; the remaining address/resource plane is closed.
- **E684/E685:** retained G15 init/power register programming audited end-to-end; no surviving scalar/RMW mismatch.
- **E686:** hardware replay status is a generic lifecycle class, not a frontend error decoder.
- **E687-E690:** J615 PMGR topology, AUTO gating and continuous physical GFX power/retention are closed. Continuous explicit ACTIVE from first platform power-on still fails before opcode 0.
- **E691:** later reason-5 quiesce is retained-firmware self-management of an already-stalled active context; no missing host busy publication feeds it.

## Do not reopen without contradictory evidence

- Queue/WorkQueue transport, stamp registration or scheduler acceptance.
- CDM root fetch/control-stream placement.
- Launch geometry/mode or already-validated ESL grammar/slot placement.
- RegisterArray, RunCompute/SKU, known HwDataB or command-local producer families.
- UAT/PTE, PagePool/FList/UMA, mapping ownership, cache or DMA publication.
- Primary preemption/context backing or persistent CL command-resource backing.
- Ad-hoc secure-cache selector changes without the separate protected-context registration contract.
- Retained G15 startup/power RMW state.
- LMX0/GFX PMGR/AUTO/continuous physical-power hypotheses.
- Direct `c020/c040/c128` probing, replay-state injection, reason-5 suppression or invented host busy state.
- Existing firmware KTrace/diagnostic paths as a passive pre-opcode observer; no exact waypoint exists there.

## Next-work rule

Stay static until a mechanically exact **non-address, non-power autonomous G15 frontend/context state** is identified after c058 active-slot publication and before instruction-0 interpretation. The best next evidence would be a precise working-M3/macOS frontend-state oracle that can be back-translated to exact 23J220 authority, an exact-target hardware/context semantic not represented by the closed families, or a new public/upstream G15 state producer.

Do not spend another protected live cycle merely to refine replay timing or perturb a closed family.

Full checkpoint: `research/g15/G15-E659-E691-FRONTEND-ACTIVATION-BOUNDARY.md`.
