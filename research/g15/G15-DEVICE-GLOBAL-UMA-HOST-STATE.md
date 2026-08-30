# J615 device-global UMA host-state placement — E138

E138 closes when the E136 device-global UMA state may safely exist without changing Apple's pool-creation order. The proof is exact macOS 14.8 / 23J220 static analysis plus a host-only Linux placement. No module was installed and no RunCompute/custom GPU command was issued.

## Accelerator state is eager; UMAPools are lazy

E135 already proved `AGXAcceleratorG15::start()` initializes one 0x100-entry `"UMAPool"` HardwareBuffer-ID manager at accelerator `+0x2a08`, while `AGXUMAPool::init()` assigns firmware-visible pool `+0x80` by incrementing an independent zero-initialized global qword.

E138 traces channel creation around that boundary. Exact `AGXCommandQueue::init()` does not call any TA/3D/CL channel factory. Base `AGXWorkQueue::init()`, `AGXCLWorkQueue::init()` and `AGX3DWorkQueue::init()` also initialize only workqueue-local state. Device, command-queue and bare workqueue creation therefore consume no UMAPool identity.

## Compute channel creation is lazy

`AGXCommandQueue::chooseCLWorkQueue()` first tests the selected command-queue CL-workqueue slot at `+0x5f0 + index*8`. If that slot already exists it returns the existing workqueue. Only the absent-slot path allocates and initializes a new `AGXCLWorkQueue`, calls the accelerator channel factory at vslot `+0xb48`, stores the returned channel at workqueue `+0x1e8`, and initializes that channel.

Exact J615/G15G accelerator vtable references close the factory identities mechanically:

- `+0xb38` -> `AGXAcceleratorG15::newTAChannel() [clone .8480]`;
- `+0xb40` -> `AGXAcceleratorG15::new3DChannel() [clone .8479]`;
- `+0xb48` -> `AGXAcceleratorG15::newCLChannel() [clone .8478]`.

The three cells fix the J615 accelerator vptr address point at `0xfffffe0007c322c0`. `AGXCLChannel::init()` then reaches the E135 shared-Compute UMAPool selection/creation machinery. A Compute UMAPool therefore enters the global pool-ID sequence only when the CL channel path is actually instantiated.

## TA/3D channel creation is lazy too

`AGXCommandQueue::allocate3DWorkQueue()` constructs a 3D workqueue only when that path is required. `AGX3DWorkQueue::allocTAChannel()` reaches exact accelerator vslot `+0xb38`, and the 3D-channel path reaches exact `+0xb40`. Their UMAPools consequently enter the same creation-order sequence only on actual TA/3D channel construction, not during device or command-queue initialization.

## Linux placement

Linux commit `f2cb03001da02b1bde833550f38700b247ca30c2` places one G15-only `G15DeviceUmaOwnerState` under `GpuManager` lifetime. It contains:

- the already-reconstructed exact 0x100 UMAPool HardwareBuffer-ID namespace;
- the pool-ID sequence seeded to zero.

Creating this host state allocates no UMAPool, consumes no pool ID, publishes no GPU-visible address and exposes no Queue accessor. The only code capable of advancing the sequence remains the still-dormant `G15SharedComputeUmaPoolOwner` constructor.

This is deliberately scoped to the exact J615 single-integrated-accelerator target. E138 makes no claim about how a hypothetical multi-accelerator system should share Apple's kernel-global pool-ID source.

## Validation and boundary

Linux tree: `1e10f2d211ddb16995ac73de2c7c5243f9f6d1b0`.

Patch 0052 reconstructs that tree exactly from E136 commit `7c7e4d68082f8ff349c191bc3021c2f7d023009e`.

Strict source-diff checkpatch: 0 errors, 0 warnings, 0 checks. External module build passes at the established exact 24-individual-warning baseline; module SHA-256 is `3f9406c6b481c57e6d15082414968b128e330dc035c8a9fe1c0e8330586ead9e`, vermagic `7.1.6-gc6fa9b794ba9 SMP preempt mod_unload aarch64`.

The state is now host-live if this checkpoint is eventually installed, but it has no pool/Queue consumer and no GPU-visible side effect. No shared pool is instantiated, no pool ID is consumed, ordinary `submit_compute()` is unchanged, and there is still no RunCompute writer.
