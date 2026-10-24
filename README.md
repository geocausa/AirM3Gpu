# AirM3Gpu

Experimental bring-up and reverse-engineering work for the Apple M3 / T8122 J615 G15G C0 GPU on Asahi Linux.

This repository is the **canonical public research checkpoint**. The compilable Linux kernel history lives in the subordinate `geocausa/linux` fork; raw proprietary and machine-local evidence stays outside the public repositories.

## Current state — 2026-10-24

Linux reaches exact command/shared-state publication, normal Compute activation and entry visibility, but still fails **before instruction 0 is interpreted**. Later work did not claim Linux shader success: it progressed through bounded RDE/APS observation on Linux (through E828) and offline Apple-vs-Linux APS/RLD token replay (through E972).

The immediate research gate is to recover and durably preserve the actual E972 Apple-E932 vs Linux-E934 decoded token output, identify the earliest semantic differential, and back-translate its producer/consumer to exact macOS 14.8.3 / 23J220 before another live mutation.

**Start here:**

1. [`docs/CANONICAL-CHECKPOINT.md`](docs/CANONICAL-CHECKPOINT.md) — authoritative frontier, closed families, and next gate.
2. [`docs/NEW-CHAT-HANDOFF.md`](docs/NEW-CHAT-HANDOFF.md) — short operational continuation point.
3. [`docs/RECENT-EXPERIMENTS.md`](docs/RECENT-EXPERIMENTS.md) — post-E717 observer/replay chronology.
4. [`docs/WORKFLOW.md`](docs/WORKFLOW.md) — anti-repeat and publication discipline.
5. [`docs/REPOSITORIES.md`](docs/REPOSITORIES.md) — repository and machine-local evidence roles.

Historical pre-E718 state and experiment ledgers are retained unchanged in `docs/HISTORICAL-STATE-THROUGH-E717.md`, `docs/HISTORICAL-EXPERIMENTS-THROUGH-E717.md`, and `manifests/HISTORICAL-HASHES-THROUGH-E717.txt`.

## Safety/status

This is research code, not general M3 GPU enablement. Keep the Golden Linux boot path untouched, use bounded one-variable experiments, and do not reopen closed Queue/RunCompute/UAT/power families without contradictory evidence.
