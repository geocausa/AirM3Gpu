# G15 event-control pool geometry — E091

E091 is a compile-only ownership checkpoint based on the exact macOS 14.8.3 / 23J220 `AGXCommandBuffer` event-control geometry closed in E088. No Linux module was installed and no RunCompute or other GPU command was issued.

Exact 23J220 maintains 36 rotating command-buffer event-control states. Each state is exactly 0xc0 bytes, the backing is contiguous, and the host keeps parallel CPU/GPU addresses. The selector is initialized to state 0 and `nextCommandBufferState()` increments before selecting modulo 36. The selected GPU address eventually becomes RunCompute `+0x14` after firmware GPUVA→FWVA conversion.

Linux commit `97bd7129c3f05a22cc604ceb04a9c9bc39893100` models only that proven geometry:

- 36 states;
- 0xc0-byte opaque block size;
- exact 0x1b00-byte contiguous pool size;
- compile-time block/pool size assertions;
- increment-then-mod-36 host selector and exact per-state offset helper.

The event-control bytes remain opaque because E088 did not close the complete internal 0xc0 ABI. More importantly, the pool deliberately does **not** implement `GpuStruct` and is connected to no allocator. The exact event-control mapping class has not yet been recovered from 23J220, so E091 makes accidental GPU allocation/publication impossible. RunCompute `+0x14` remains zero/fail-closed.

Validation: exact tree `27efe6a7509e5e1ca41a1561e24139644dc6b6c3`, module SHA-256 `fe09dd7d5471c73a1febdeb31a1d7e6c185c216036a1a6cd975b72d621f268f6`, sacrificial-kernel vermagic `7.1.6-gc6fa9b794ba9 SMP preempt mod_unload aarch64`, existing 24-individual-warning baseline, strict checkpatch 0/0/0. Patch 0020 reconstructs the exact tree from E089.

Next boundary: recover the exact 23J220 allocation/mapping class and minimum initialization fields for these event-control blocks before adding any GPU-backed owner or command pointer.
