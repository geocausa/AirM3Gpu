# Repository roles

AirM3Gpu is one project with two public Git repositories plus a machine-local evidence workspace.

## `geocausa/AirM3Gpu` — canonical project repository

This is the project front door and research checkpoint. It contains only material suitable for publication: derived facts, original notes/scripts, hashes, manifests, reproducible patch descriptions and selected source patches.

It intentionally does **not** contain Apple firmware, kernelcaches, extracted proprietary binaries, Ghidra databases, boot images, built kernel modules or raw machine-local captures.

## `geocausa/linux` — kernel implementation fork

This is a fork of the Asahi Linux kernel tree used to retain compilable J615/G15 source history. Experimental kernel branches such as `wip/g15-e274-working-body-order` and `wip/g15-e278-cdm-shared-rw` live here because they must preserve Linux kernel ancestry and cannot sensibly be represented as the AirM3Gpu project repository itself.

This repository is subordinate to AirM3Gpu: it is implementation history, not a second project.

## `/home/macmac/m3-gpu-lab` — private/local evidence workspace

This is the high-volume working corpus: experiment directories, exact-binary hashes, decompilation output, live logs, Ghidra scripts/databases, retained captures and scratch analysis. It is deliberately not pushed wholesale to either public repository.

As of 2026-09-02 this workspace contains important evidence newer than the public AirM3Gpu checkpoint. See `manifests/LOCAL-LAB-STATUS-20260902.txt` for the preservation audit.

## Rule going forward

1. New technical work happens in the local lab and a dedicated Linux worktree when source changes are required.
2. Compilable kernel commits are pushed to `geocausa/linux` before a risky live test.
3. Sanitized conclusions/results are then checkpointed into `geocausa/AirM3Gpu`.
4. Raw/proprietary evidence is never copied into a public repository merely for backup.
