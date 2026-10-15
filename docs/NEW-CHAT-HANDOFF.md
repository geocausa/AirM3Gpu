# New-chat handoff — J615 / G15 Compute bring-up

Generated: 2026-10-15

This is the public/sanitized continuation point. The private M3 lab retains detailed machine-local evidence; proprietary binaries, reverse-engineering databases, built modules and raw captures are intentionally not published here.

## Target and authority

- Physical target: MacBook Air M3 / T8122 / J615 / G15G C0.
- Persistent/default OS: Golden Ubuntu kernel `7.1.6-ubuntu-m3-usbpd-gc5037a961e4d`.
- Exact ABI authority: macOS 14.8.3 / build 23J220 plus matching retained G15G C0 firmware.
- Later macOS builds: structural/dynamic oracle only; back-translate every observation before changing Linux.

## Current execution frontier

The failure is now below all mechanically identified software-authored command, selected shared-memory, startup/reset and common engine-activation state.

`exact RunCompute/SKU/RegisterArray + selected ChannelState/DAG + UAT/PagePool/UMA -> c040/c048/c050/c058 publication -> common Compute activation -> entry VA fetch-visible -> [autonomous G15 entry-interpreter admission/execute transition fails] -> instruction 0 not interpreted -> context-store/quiesce -> replay-1`

E711-E717 additionally close the exact c120/c140/c148/c128 engine lifecycle, known Apple/firmware diagnostic paths, software-visible reset/default state, and the complete selected 0x24c0 ChannelState consumer map. XDC/current-public G15 work converges with this entry-shader interpretation rather than exposing a missing host prerequisite.

## Recent material milestones

- **E702/E703:** separate upstream-quality G15 startup YUV-matrix parity bug found/fixed compile-only; non-causal for Compute.
- **E704-E710:** final software publication and remaining command/policy planes exhausted.
- **E711:** normal c120/c140/c148 activation handshake and c128 completion reconstructed; common with successful terminate control.
- **E712/E714/E715:** known exact Apple/firmware frontend diagnostic paths exhausted; no safe passive observer.
- **E713:** XDC/current-public convergence confirms UAT/page-manager/entry-shader model; no new activation field.
- **E716:** software-visible entry-interpreter reset/default state closed.
- **E717:** complete selected ChannelState closed; 0xb0 QueueInfo + 128 x 0x48 firmware records + 0x10 unused tail.
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

Full current checkpoint: `research/g15/G15-E692-E717-AUTONOMOUS-FRONTEND-CLOSURE.md`.

Previous first-opcode checkpoint: `research/g15/G15-E659-E691-FRONTEND-ACTIVATION-BOUNDARY.md`.
