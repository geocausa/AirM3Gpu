# AirM3Gpu

Experimental, clean-room bring-up work for the Apple M3 / T8122 G15 GPU on Asahi Linux.

This repository is the **public, reproducible checkpoint** for the work. It intentionally does not contain Apple firmware, kernelcaches, extracted proprietary binaries, Ghidra databases, boot images, built kernel modules, or raw machine-local captures. Those are kept outside this repository; only derived facts, original research notes/scripts, hashes, and source patches are published here.

## Current research state — 2026-08-27

Target hardware: MacBook Air M3 (J615 / T8122), GPU identified at runtime as G15G C0.

The bring-up has advanced well beyond the earlier pre-`MSG_INIT` checkpoint. The isolated candidate now reaches native G15 `MSG_INIT`, post-init bootstrap, persistent RTKit/GpuManager lifetime, DRM registration, and `/dev/dri/renderD128`. Safe host-side GET_PARAMS, VM/GEM lifecycle, unbound VM mappings, passive queue lifecycle, q22 mapping notification, and shared-bank1/range-7 page-table handling are live-tested.

The current hard boundary is a deliberately bounded **empty Compute QueueInfo publication**. The host advances the priority-2 Compute TX `WriteIndex` from 0 to 1 and rings the exact G15 EP21 doorbell, but firmware leaves both `ReadIndex` and `CFIIndex` at zero and emits statistics tag `0x0f`. No GPU command-ring entry is exposed and no GPU command buffer is executed.

Offline reconstruction has ruled out the obvious TX-layout, RuntimePointers aliasing, barrier, doorbell, G15 wake-note, MTR, shared-bank1, and normal J615 submission-time power hypotheses. In particular, J615 initializes accelerator feature gate `+0x6c1` to zero, so Apple's `ensurePoweredHardware(false)` performs no `state 2 -> 1` transition on normal submission.

E045 proves the failed first Compute publication reaches `g15_pipe_work_callback` and passes its outer runtime-power gate. E047 places the boundary before `FUN_6a0c`, inside `FUN_3d330()`. E048-E051 then close the cold-power worker handshake: q21 reaches power/state 1, `FUN_447ec(arg=1)` enters and exits, and the event-1 post primitive returns successfully. The remaining boundary is later in the short `FUN_3d330` tail, before the scheduler's first trace. Direct PMGR pokes remain unjustified; the next step is passive tracing of the existing `0x10078` notification marker and q22's optional-tail gate.

See [Current State](docs/CURRENT-STATE.md), [G15 Pipe Submission Boundary](research/g15/G15-PIPE-SUBMISSION-BOUNDARY.md), [G15 Pipe Callback Gate](research/g15/G15-PIPE-CALLBACK-GATE.md), [Workflow](docs/WORKFLOW.md), [Patch Bases](docs/PATCHES.md), and [Recovery](docs/RECOVERY.md).

## Repository layout

- `docs/` — checkpoint, workflow, recovery, and patch-base documentation.
- `patches/linux/` — Linux/Asahi checkpoint patch against the recorded kernel base.
- `patches/mesa/` — Mesa/Honeykrisp patch series against the recorded Mesa base.
- `research/g15/` — curated original G15 reverse-engineering notes and clean-room generators.
- `manifests/` — hashes and component revisions for the current tested checkpoint.

## Status

This is bring-up/research code, not a production driver. Every live gate is intentionally staged and fail-closed. Do not treat the patchset as general M3 GPU enablement yet.
