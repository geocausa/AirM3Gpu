# AirM3Gpu

Experimental, clean-room bring-up work for the Apple M3 / T8122 G15 GPU on Asahi Linux.

This repository is the **public, reproducible checkpoint** for the work. It intentionally does not contain Apple firmware, kernelcaches, extracted proprietary binaries, Ghidra databases, boot images, built kernel modules, or raw machine-local captures. Those are kept outside this repository; only derived facts, original research notes/scripts, hashes, and source patches are published here.

## Current research state — 2026-08-27

Target hardware: MacBook Air M3 (J615 / T8122), GPU identified at runtime as G15G C0.

The bring-up has advanced well beyond the earlier pre-`MSG_INIT` checkpoint. The isolated candidate now reaches native G15 `MSG_INIT`, post-init bootstrap, persistent RTKit/GpuManager lifetime, DRM registration, and `/dev/dri/renderD128`. Safe host-side GET_PARAMS, VM/GEM lifecycle, unbound VM mappings, passive queue lifecycle, q22 mapping notification, and shared-bank1/range-7 page-table handling are live-tested.

The bounded registration path now reaches and returns from the real G15 scheduler. E056 identified a mis-modeled 0x54-byte DPE leakage-update image at HwDataA `+0x4188`; restoring its exact J615/C0 values removes the earlier cold-power/DPE retrigger loop. E057 then produces scheduler `0x112` snapshots and an accepted `0x111` RunWorkQueue record.

E058-E060 close the remaining registration prerequisites without executing GPU work. A valid stamp number alone is insufficient; a firmware Barrier/type-4 record is required to bind the RunWorkQueue stamp slot to QueueInfo `gpu_buf`. With one already-satisfied Barrier at ring entry 0, the pipe retires to `(Read, CFI, Write, Shadow) = (1,1,1,1)` and callback exit `0x101` appears. E060 also corrects G15 `ReleaseResource` enum numbering to Apple's opcode `0x11`; native teardown then succeeds and firmware returns from software state 1 to state 0 without a crash.

No RunVertex, RunFragment, or RunCompute command and no shader stream is exposed by this proof. The current boundary is static reconstruction of the smallest real scheduler command contract, with live command execution still fail-closed.

See [Current State](docs/CURRENT-STATE.md), [G15 Queue Registration Lifecycle](research/g15/G15-QUEUE-REGISTRATION-LIFECYCLE.md), [G15 Pipe Submission Boundary](research/g15/G15-PIPE-SUBMISSION-BOUNDARY.md), [G15 Pipe Callback Gate](research/g15/G15-PIPE-CALLBACK-GATE.md), [Workflow](docs/WORKFLOW.md), [Patch Bases](docs/PATCHES.md), and [Recovery](docs/RECOVERY.md).

## Repository layout

- `docs/` — checkpoint, workflow, recovery, and patch-base documentation.
- `patches/linux/` — Linux/Asahi checkpoint patch against the recorded kernel base.
- `patches/mesa/` — Mesa/Honeykrisp patch series against the recorded Mesa base.
- `research/g15/` — curated original G15 reverse-engineering notes and clean-room generators.
- `manifests/` — hashes and component revisions for the current tested checkpoint.

## Status

This is bring-up/research code, not a production driver. Every live gate is intentionally staged and fail-closed. Do not treat the patchset as general M3 GPU enablement yet.
