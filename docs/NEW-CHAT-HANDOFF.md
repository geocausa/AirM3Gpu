# New-chat handoff — J615 / G15 Compute bring-up

Generated: **2026-10-24**

**First read:** `docs/CANONICAL-CHECKPOINT.md`. It supersedes older "current frontier" text elsewhere in the history.

## One-sentence state

Linux reaches normal G15 Compute activation with the entry program fetch-visible but still fails before instruction 0 is interpreted; subsequent E740–E972 work built RDE/APS observers and an Apple-vs-Linux token/RLD replay path to identify the autonomous frontend difference without blind GPU mutation.

## Target / authority

- MacBook Air M3 / J615 / T8122 / G15G C0.
- Exact ABI authority: macOS 14.8.3 / 23J220.
- Newer macOS/Xcode: oracle only until back-translated.
- Keep the Golden Linux kernel untouched; later G15 branches are sacrificial observer/test candidates.

## Latest preserved checkpoints

- Research replay frontier: **E972**, commit `767337aa6e32bbd13d75b3ef046faaa0a8fd1c0a` before consolidation.
- Latest kernel observer branch: **E828**, `wip/g15-e828-producer-ktrace @ 3070021adff4108337d19c53070c1f95359b86f5`.
- Known-working Apple APS positive comparison: **E907**.
- Full Linux RegisterArray replay pages: **E950**.
- Linux Statics RLD replay pages: **E955**.
- Held Metal no-dispatch/dispatch differential oracle: **E814**, locally under `/home/macbuntu/e814-work/`.

## Hard boundary

`exact command/shared state -> common Compute activation -> entry fetch-visible -> autonomous interpreter transition missing -> instruction 0 not interpreted -> recovery`

Nothing after E717 proves Linux instruction-0 execution. E740–E828 are observer instrumentation; E848–E972 are parser/replay work.

## Critical correction not to lose

E826 proves the earlier E825 KTrace-negative interpretation was invalid because the firmware config refresh needs the already-proven ~2 ms settle after ForceConfigSnapshot. Do not cite E825's absence of KTrace as a real negative.

## Immediate next task

Recover and durably record the actual **E972 Apple E932 vs Linux E934 decoded token sequence**, then name the earliest semantic difference and back-translate its producer to exact 23J220. The replay source and pages are preserved, but its stdout/result interpretation was not committed.

After that, compare only against the relevant E907/E950/E955 pages and E814 differential. A new live Linux test is justified only when this yields one concrete causal state/producer.

## Do not restart

Do not redo broad Queue/WorkQueue, scheduler, CDM root, UAT/PTE, PagePool/FList/UMA, RunCompute/SKU/RegisterArray, selected ChannelState, PMGR/power, replay/reason-5, or generic cache/DMA sweeps without contradictory evidence.

Do not treat current-Xcode APS token/chunk behavior as exact 23J220 ABI without a bridge.
