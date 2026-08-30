# J615 shared Compute UMA owner split — E136

E136 implements the definition-only ownership correction implied by E135's exact macOS 14.8 / 23J220 placement proof. No module was installed and no RunCompute or custom GPU command was issued.

## Device-global UMA state

E135 proved that G15 owns one accelerator-global 0x100-entry `UMAPool` HardwareBuffer-ID namespace at accelerator `+0x2a08`, while `AGXUMAPool::init()` assigns pool `+0x80` from one global creation-order counter whose kernel-image initial value is zero.

Linux now models those two global namespaces together as definition-only `G15DeviceUmaOwnerState`:

- one `G15HardwareBufferIdManager` with the exact 0x100 capacity/state machine already reconstructed in E079/E080;
- one zero-initialized, checked monotonically incremented pool-ID source;
- typed `G15UmaPoolIdentity` values instead of arbitrary raw pool IDs.

The owner is deliberately not placed in `GpuManager` yet. A future live instance is valid only when TA, 3D and CL UMAPool creation all consume the same creation-order state; E136 does not create a Compute-only counter.

## Shared Compute pool/FList lifetime

`G15SharedComputeUmaPoolOwner` now represents one reusable shared Compute UMAPool-side FList lifetime. Its definition-only constructor:

- requires the device-global UMA state rather than a newly supplied HardwareBuffer-ID manager;
- advances the global pool-ID source at the Apple pool-construction boundary;
- accepts only the two exact Compute priority classes;
- constructs the existing exact J615 FList with the device-global manager;
- retains the priority class and typed pool identity beside that FList.

The stock-empty HardwareBuffer-reference path rejects a priority-class mismatch before acquiring the shared pool's FList reference.

## Channel/command owners no longer contain the FList

The former dormant `G15StockEmptyComputeOwnerGraph` is renamed `G15StockEmptyComputeChannelOwners`. It no longer owns or accepts:

- an FList;
- a HardwareBuffer-ID manager;
- a raw firmware-visible pool ID;
- an FList owner cookie;
- the FList range-5 list allocator.

The channel/command owner retains event-control, HWMetrics, timestamp/scheduler/channel-state resources, cached/uncached channel memory, CL command resource, Compute statistics and SKU backing.

Every dormant two-phase prepare/finalize/abort/completion method that needs Page-Pool State now takes a separate `G15SharedComputeUmaPoolOwner`, and all HardwareBuffer rollback/completion flows through that shared pool.

This prevents the next integration step from silently recreating the E135-proven-wrong per-Queue UMA/FList topology.

## Validation and boundary

Linux commit: `7c7e4d68082f8ff349c191bc3021c2f7d023009e`

Tree: `f6dc1e1f4a7bd308991dec1de29b41879a56f452`

Patch 0051 reconstructs that tree exactly from E134 commit `a244a846e3014b9eebdec580f852e39d27a1a50d`.

Strict source-diff checkpatch: 0 errors, 0 warnings, 0 checks. External Asahi module build passes at the established exact 24-individual-warning baseline; module SHA-256 is `efe3e1ed324db6d3abf77ac6b930a5f7a16ef27c45ad9b187c10c123bb925d9b`, vermagic `7.1.6-gc6fa9b794ba9 SMP preempt mod_unload aarch64`.

There is still no device-global UMA runtime instance, no live shared Compute pool instance, no Queue call site for the dormant channel owner, no `submit_compute()` change and no RunCompute writer. The next ownership problem is the exact shared-pool-container retain/select/replacement lifecycle and safe eventual placement of the single device-global UMA owner only after all pool classes share its creation-order sequence.
