# G15 stock-empty Compute-statistics SKU owner — E130

E130 closes the `firmware_state_fwva` provenance used by the exact normal-J615 stock-empty Compute SKU stream. This is static clean-room reconstruction plus compile-only Linux modeling; no custom GPU command was executed.

## Exact Apple allocation identity

In matching macOS 14.8.3 / 23J220, `AGXFirmware::allocFirmwareData()` builds one shared-data descriptor array. Its first exact allocation sizes are:

`0x1860, 0xc10, 0x1248, 0xe10, 0x4360, ...`

Those sizes match the already reconstructed Linux G15 manager-global objects in order:

- `0x1860` — `HwDataB`
- `0xc10` — `G15StatsVtx`
- `0x1248` — `G15StatsFrag`
- `0xe10` — `G15StatsComp`
- `0x4360` — `HwDataA`

Descriptor index 3 pairs the CPU-side output member `AGXFirmware +0x228` with the GPU/FW-visible member `AGXFirmware +0x268` and exact size `0xe10`. With the allocator's 0x40-byte slice alignment, this object begins at offset `0x3740` within the shared backing.

## Initial state and consumers

`AGXFirmware::initFirmwareData()` zeroes the complete object reached through `+0x228` with an exact `0xe10`-byte memset. The later `-1` sentinel writes are only for the preceding Fragment statistics object. Linux's `G15StatsComp` is already exact-size `0xe10` and default-zeroed.

`AGXArmFirmware::initFirmwareSharedData()` converts `AGXFirmware +0x268` and writes the firmware-visible address into shared-data `+0x244`. Linux already populates the corresponding G15 wrapper field from `runtime_pointers.g15_stats_comp.gpu_va()`. `AGXCLChannelG15::encodeCLCommandSKUStream()` consumes that same `+0x268` source for the stock-empty SKU packets.

Therefore the E101 `firmware_state_fwva` value is the persistent manager-owned G15 Compute statistics object, not an unknown command-local state allocation.

## Linux boundary

Patch `0047` applies from Linux checkpoint `65451e3d5cc8ebc582433abf874811c7eada79b9` to `635fca8e84b3d6ba27f6f7dd760275c635cd3495`, reconstructing tree `7ffe7e70a303ccefc7e46d9595fb33de09d6b1d4` exactly.

The compile-only delta:

- removes raw `firmware_state_fwva` from target-specific `G15StockEmptySkuFinalizeInputs`;
- carries a typed `GpuWeakPointer<G15StatsComp>` in the dormant owner graph;
- converts that pointer only when building the generic `G15StockEmptySkuInputs` serializer input;
- leaves the generic serializer parameterized;
- adds no live owner-graph constructor, Queue call site, RunCompute writer, or submission path.

Validation: strict source-diff checkpatch `0/0/0`, module build PASS at the established 24-individual-warning baseline, module SHA-256 `f58313ed791123de197c9094199867cd50e5e3fc4c04d96eb8f00a67f5732039`, and exact patch-tree reconstruction PASS.

The remaining target-specific finalize inputs are command/runtime values: firmware stamp FWVA, optional user timestamp addresses, command counter, context ID, queue event sequence, UUID and stamp value. They remain explicit until their exact producers and lifetimes are closed.
