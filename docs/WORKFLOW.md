# Development and Publication Workflow

AirM3Gpu uses a split-repository workflow so reverse-engineering evidence cannot accidentally become public or get mixed with upstream component history.

## Repositories

### Public canonical checkpoint: AirM3Gpu

This repository contains:

- original research summaries and clean-room scripts
- exact component revision manifests
- reproducible patch series
- milestone status and recovery instructions

It does **not** contain raw Apple binaries, firmware, kernelcaches, APFS extracts, Ghidra databases, boot images, built modules, or large raw runtime logs.

### Local evidence lab

The private/local lab contains the full machine-specific evidence, raw captures, exploratory logs, and reverse-engineering databases. It is the evidence vault, not the publication branch.

### Component worktrees

Linux/Asahi and Mesa are developed in their own Git worktrees. AirM3Gpu receives exported patch series; it does not vendor full Linux or Mesa histories.

## Milestone discipline

A durable milestone follows this order:

1. Save a local evidence checkpoint before a risky transition.
2. Make the smallest source change that opens one new gate.
3. Build with the exact target kernel/Mesa base.
4. Exercise a fail-closed live preflight.
5. Verify cleanup/postconditions, not merely the positive log line.
6. Commit the component worktree immediately after the gate is closed.
7. Export the component commit(s) into `patches/`.
8. Update `docs/CURRENT-STATE.md` and `manifests/CURRENT-HASHES.txt`.
9. Commit AirM3Gpu.
10. Push AirM3Gpu `main` only when the checkpoint is internally consistent.

No durable work should remain as an uncommitted component diff beyond one test cycle.

## Branch policy

- `main`: tested, recoverable public checkpoints only.
- `wip/<topic>`: optional public work-in-progress branches when useful.
- component branches remain separate and are never pushed to Linux/Mesa upstream remotes merely as a side effect of publishing AirM3Gpu.

Never force-push `main` as part of normal bring-up.

## Safety gates

Each new hardware/firmware boundary must be independently proven. In particular:

- preserve a known-good boot path before changing the shared m1n1 payload;
- use one-shot experimental kernel boots;
- leave the normal/golden kernel as the persistent boot default;
- do not combine first-time UAT, RTKit, InitData, endpoint-start, and GPU-work transitions in one experiment;
- record both success criteria and cleanup criteria;
- do not convert an unknown firmware address into a Linux physical mapping by analogy.

## Publication hygiene

Before each public commit:

- run `git diff --check` on component changes;
- verify patch bases and heads;
- inspect `git status --short` for accidental binaries/caches;
- do not use `git add -A` in the private evidence lab;
- keep generated/recovered proprietary material outside AirM3Gpu;
- use hashes and derived descriptions in place of private binary artifacts.
