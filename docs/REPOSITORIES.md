# Repository roles

AirM3Gpu is one project with two public Git repositories plus a machine-local evidence workspace.

## `geocausa/AirM3Gpu` — canonical project repository

This is the project front door and research checkpoint. It contains only material suitable for publication: derived facts, original notes/scripts, hashes, manifests, reproducible patch descriptions and selected source patches.

It intentionally does **not** contain Apple firmware, kernelcaches, extracted proprietary binaries, Ghidra databases, boot images, built kernel modules or raw machine-local captures.

## `geocausa/linux` — kernel implementation fork

This is a fork of the Asahi Linux kernel tree used to retain compilable J615/G15 source history. Experimental kernel branches live here because they must preserve Linux kernel ancestry. The current Linux 7.3 J615 port is `wip/m3-7.3-j615` at `52f5d1cb8f5983131319a3083c0f1597e871a3e9`; earlier repaired-lineage display/GPU source remains on its historical branches.

This repository is subordinate to AirM3Gpu: it is implementation history, not a second project.

## Machine-local evidence workspaces

The high-volume evidence corpus is deliberately split by host and is not pushed wholesale to either public repository.

Active Ubuntu-side locations include:

- `/home/macbuntu/m3-gpu-evidence/` — copied/sanitized experiment bundles and parser evidence;
- `/home/macbuntu/e814-work/` — held Metal command A/B differential captures;
- `/home/macbuntu/e258-23j220-compiler/` — retained exact-target compiler materials;
- `/home/macbuntu/e812-tmc-apple/` — generated Apple API bindings used by later parser/oracle work;
- `/home/macbuntu/Projects/AirM3Gpu/` — the local canonical public-project checkout.

When the macOS endpoint is online, retained evidence lives primarily under `/Users/Shared/`, including `m3-gpu-native`, `e280-run`, the exact-version IOGPU/AGX reductions, and later profiler/parser bundles. These paths contain raw or machine-specific material and remain local.

`/home/macmac/m3-gpu-lab` is the active private Ubuntu-side lab workspace for the current J615 work. It contains high-volume/raw experiment evidence and is not the same Git history as this curated public repository; public conclusions are copied here deliberately.

As of 2026-10-25 the public checkpoint is consolidated through E1099, while the Linux implementation fork retains the compilable Linux 7.3 J615 source at `52f5d1cb...`. Machine-local evidence remains the authority for raw captures, exact-binary analysis and experiment-level evidence. Historical preservation details remain in `manifests/LOCAL-LAB-STATUS-20260902.txt`.

## Rule going forward

1. New technical work happens in the local lab and a dedicated Linux worktree when source changes are required.
2. Compilable kernel commits are pushed to `geocausa/linux` before a risky live test.
3. Sanitized conclusions/results are then checkpointed into `geocausa/AirM3Gpu`.
4. Raw/proprietary evidence is never copied into a public repository merely for backup.


## Anti-stale rule

Before new work, fetch both repositories and read `docs/CANONICAL-CHECKPOINT.md`. Branch names and old handoffs are historical evidence, not proof of the latest frontier. When a new experiment materially changes the frontier, update the canonical checkpoint and handoff in the same checkpoint commit; do not leave the conclusion only in CI stdout or a machine-local scratch file.
