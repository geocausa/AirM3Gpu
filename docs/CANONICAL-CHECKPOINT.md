# Canonical project checkpoint — J615 / G15

Updated: **2026-10-24**

This file is the single resume point for the AirM3Gpu project. Read it before opening historical notes or starting a new experiment. Historical documents remain valuable evidence, but any older "current frontier" statement is superseded by this file.

## Target and evidence authority

- Hardware: MacBook Air M3, J615 / T8122, GPU G15G C0.
- Exact ABI authority: macOS 14.8.3 / 23J220 plus the matching retained G15G C0 components.
- Current macOS/Xcode observations are **structural or dynamic oracles** unless explicitly back-translated to 23J220.
- Persistent Linux safety baseline remains the Golden kernel; observer/test branches are sacrificial candidates, not a replacement Golden image.
- Raw/proprietary captures remain machine-local. Public repositories contain derived facts, reproducible probes, selected captured profiler pages, and source history only.

## Current hard execution boundary

The last mechanically closed execution boundary remains:

`exact RunCompute/SKU/RegisterArray + selected ChannelState/DAG + UAT/PagePool/UMA -> common Compute activation -> entry VA fetch-visible -> [autonomous G15 entry-interpreter admission/execute transition fails] -> instruction 0 not interpreted -> context-store/quiesce/replay`

No evidence found after E717 demonstrates successful Linux interpretation of instruction 0. Later work materially improved **observation and comparison of that autonomous interval**; it did not move the Linux execution boundary past instruction 0.

## What progressed after the old E717 handoff

### E740–E764 — RDE observer integration

The kernel fork gained bounded RDE observation on the live Compute path, lifetime binding, synchronized sampler ownership, kick-boundary triggering, late trace windows, sampler-active SKU modeling, and split source mapping/ownership. These are observer changes, not a new execution path.

### E776–E828 — CSInvocation / APS / producer tracing

The observer path was progressively narrowed from four-lane CSInvocation correlation to APS USC topology, source-buffer ownership, launch selector, counter-control image, runtime installation, selected-observer integration, raw-ring snapshots and ring-control preservation. E808 exposed transition status. E817–E823 explored source cacheability and START/STOP timing/gates. E825 opened bounded firmware KTrace around APS/power events.

A crucial correction is preserved at **E826**: the earlier trace window had not allowed the proven 2 ms firmware config-refresh settle, so absence of KTrace records before that point was not a valid negative result. E827/E828 then extend the bounded observer to producer-side power-event trace classes. Latest kernel observer head:

`geocausa/linux wip/g15-e828-producer-ktrace @ 3070021adff4108337d19c53070c1f95359b86f5`

This branch is diagnostic/observer history. It is not evidence that Linux shader execution succeeded.

### E848–E972 — offline APS parser/token bridge

Research then moved off the risky live path and into reproducible parser/token replay:

- E848 recovers the Xcode 16.x G15 v4r5 parser/decoder surface.
- E850 independently recovers the Xcode 15.4 v4r4 token jump table, providing cross-version continuity evidence.
- E862–E872 recover token-wire fields/classes and controlled parser state transitions without GPU access.
- E877–E900 reconstruct production profiler policy/masks and replay captured Linux APS pages.
- **E907** preserves and replays a known-working Apple APS positive page and decodes its token wire.
- **E950** preserves/replays full Linux RegisterArray APS pages.
- **E955** preserves/replays Linux Statics RLD pages.
- **E972** embeds same-program Apple E932 and Linux E934 pages and replays both through the exact recovered Xcode 16.3 token decoder. The source explicitly performs **no GPU access**.

The E972 replay code and page inputs are durable. The human-readable CI stdout/conclusion was not committed as a result document, so **do not infer a closed E972 semantic conclusion merely from the existence of the harness**. Recover/persist that output before using it as a premise.

## Parallel macOS command-construction oracle — E814

The local E814 held-command A/B oracle remains useful but is no longer the newest frontier:

- A = configured Compute encoder with no dispatch; B = otherwise matched one-thread dispatch.
- Four repeated A/B pairs completed successfully; B writes `0xe814`.
- KCB used size changes from `0x3dc` (A) to `0x404` (B).
- Repeatable dispatch-specific stable byte spans were isolated in a small subset of tag-100 mappings.
- Local Ubuntu copy: `/home/macbuntu/e814-work/` plus `e814-{oracle,label,complete}.tgz`.

Treat the E814 byte spans as differential candidates until mechanically named; they are not automatically firmware ABI fields.

## Other preserved closed oracles that still matter

- E280: exact 23J220 compiler backend can still produce the old-stack CompileReply under current macOS via the compatibility bridge.
- E503: current-macOS timing oracle places Compute KickStart only microseconds before modeled execution start; shared CPU-visible sentinel timing is not a first-instruction discriminator.
- E810: current Xcode 27 G15G/C0 APS parser has 4 KiB chunk geometry and token-16 chunk-skip semantics. This is **not** silently promoted to a 23J220 payload rule.
- EOT `0x15349` is closed constant zero on the matched G15 path.
- EOT `0x15351` Apple semantics/lifetime are closed as transient 64-byte-aligned ESL upload GPUVA, but an equivalent Linux producer remains a separate implementation gap.

## Closed families — do not reopen without contradictory evidence

Do not restart broad sweeps of Queue/WorkQueue transport, scheduler acceptance, CDM root fetch, generic UAT/PTE visibility, PagePool/FList/UMA ownership, RunCompute/SKU/RegisterArray construction, selected ChannelState layout, retained startup/power RMW state, PMGR continuous-power hypotheses, replay/reason-5 recovery, or already-audited diagnostic paths. Those families were used to reach the first-opcode boundary and have later observer work layered on top of them.

The later APS/RDE work does **not** justify random MMIO or GPU-state mutation. It exists to name a concrete discriminator first.

## Resume here

1. Recover and persist the **actual E972 Apple-E932 vs Linux-E934 token-sequence output** (or an equivalent exact replay) as a result file, with tool/binary hashes.
2. Reduce the first meaningful semantic difference to a named APS/RLD/ESL state or producer. Keep current-Xcode facts labeled as current-Xcode facts.
3. Back-translate that candidate to exact 23J220 producer/consumer evidence.
4. Cross-check against E907 positive Apple page, E950 RegisterArray pages, E955 Statics RLD pages, and the E814 command differential only where mechanically relevant.
5. Only after a concrete causal producer/state survives those checks should one bounded Linux live candidate be designed.

**Do not resume by rebuilding E814, redoing RunCompute mapping, or re-proving the E717 closed families.**

## Canonical repository heads at consolidation

The consolidation branch merges and preserves these previously divergent histories:

- old `main` temporary-CI history through `cea6044cdd1b0f38d7fd86e9f422c09ebde74754`;
- E972 research chain through `767337aa6e32bbd13d75b3ef046faaa0a8fd1c0a`;
- E814 Xcode 16.3 parser-continuity branch through `7c2eb2adca1294c2ed02ebdc839b189405bec693`;
- E850 Xcode 15.4 token-continuity branch through `895e53c25897dc52010a94695f12690e03f64e6e`.

See `docs/RECENT-EXPERIMENTS.md` for the experiment ledger, `docs/NEW-CHAT-HANDOFF.md` for the short operational handoff, and `docs/REPOSITORIES.md` for repository roles.
