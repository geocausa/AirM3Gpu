# Current G15 bring-up state

Research state: **2026-10-24**

Target: MacBook Air M3 J615 / T8122, GPU G15G C0. Exact ABI authority: macOS 14.8.3 / 23J220.

The authoritative state is [`CANONICAL-CHECKPOINT.md`](CANONICAL-CHECKPOINT.md). This short file exists so there is only one unambiguous “current state” entry point.

## Execution boundary

`exact command/shared state -> common Compute activation -> entry fetch-visible -> autonomous interpreter transition missing -> instruction 0 not interpreted -> recovery`

Nothing after E717 proves Linux instruction-0 execution.

## Later progress

- **E740–E828:** bounded RDE/CSInvocation/APS and firmware-KTrace observer work. E826 corrects the invalid E825 negative by restoring the proven 2 ms configuration-refresh settle. Latest diagnostic kernel head: `wip/g15-e828-producer-ktrace @ 3070021adff4108337d19c53070c1f95359b86f5`.
- **E848–E972:** offline current-Xcode parser/token work. E907 gives a known-positive Apple APS page, E950 full Linux RegisterArray pages, E955 Linux Statics RLD pages, and E972 embeds same-program Apple E932 and Linux E934 pages for exact recovered-helper replay without GPU access.
- **E972 gap:** inputs and replay source are durable, but the human-readable output/conclusion was not committed. Recover it before using E972 as a closed semantic premise.

## Next gate

Persist the E972 Apple-vs-Linux decoded sequence, identify the earliest named semantic differential, bridge it to exact 23J220, and only then design one bounded Linux live test.

Historical state through E717 is preserved unchanged in [`HISTORICAL-STATE-THROUGH-E717.md`](HISTORICAL-STATE-THROUGH-E717.md).
