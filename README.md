# AirM3Gpu

Experimental, clean-room bring-up work for the Apple M3 / T8122 G15 GPU on Asahi Linux.

This repository is the **public, reproducible checkpoint** for the work. It intentionally does not contain Apple firmware, kernelcaches, extracted proprietary binaries, Ghidra databases, boot images, built kernel modules, or raw machine-local captures. Those are kept outside this repository; only derived facts, original research notes/scripts, hashes, and source patches are published here.

## Current checkpoint — 2026-08-24

Target hardware: MacBook Air M3 (J615 / T8122), GPU identified at runtime as G15G C0.

Live hardware milestones reached:

- T8122 GPU discovery and identity validation.
- 1 MGPU / 10 active cores / 10 fragment units / 4 GPs, core mask `0x3ff`.
- GFX ASC start/stop with fail-closed cleanup.
- 42-bit G15 UAT handoff and TTB bootstrap.
- Exact J615 14-state power configuration validation.
- Full G15 InitData object graph construction, validation, and destruction without firmware init.
- Complete pre-RTKit GpuManager/channel graph construction and validation.
- RTKit protocol v12 management handshake; firmware EP20 and doorbell EP21 are discovered but not started.

The current hard boundary is deliberately **before application endpoint startup and before `MSG_INIT`**. There is no DRM render node and no GPU work submission at this checkpoint.

The next blocker is the firmware-preallocated RTKit crashlog buffer requested by system endpoint EP1:

- raw message: `0x1041000192c000`
- size: `0x4000`
- tagged DVA: `0x1000192c000`
- low-40 form used by m1n1 AGX I/O helpers: `0x192c000`
- neither address is mapped in the Linux-constructed G15 TTBR0 at request time

The next step is to recover the G15 private/ASC mapper translation contract for that preallocated crashlog buffer before allowing application endpoints or `MSG_INIT`.

See [Current State](docs/CURRENT-STATE.md), [Workflow](docs/WORKFLOW.md), [Patch Bases](docs/PATCHES.md), and [Recovery](docs/RECOVERY.md).

## Repository layout

- `docs/` — checkpoint, workflow, recovery, and patch-base documentation.
- `patches/linux/` — Linux/Asahi checkpoint patch against the recorded kernel base.
- `patches/mesa/` — Mesa/Honeykrisp patch series against the recorded Mesa base.
- `research/g15/` — curated original G15 reverse-engineering notes and clean-room generators.
- `manifests/` — hashes and component revisions for the current tested checkpoint.

## Status

This is bring-up/research code, not a production driver. Every live gate is intentionally staged and fail-closed. Do not treat the patchset as general M3 GPU enablement yet.
