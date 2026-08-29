# G15 stock-empty GART HW-soft-fault input — E129

E129 closes the remaining boolean input in the exact normal-J615 stock-empty Compute SKU setup packet. This is static clean-room reconstruction plus compile-only Linux modeling; no custom GPU command was executed.

## Ordinary IOGPU device path

The matching macOS 14.8.3 / 23J220 userspace and kernel chain is mechanically fixed for an ordinary Metal device:

- `IOGPUMetalDevice initWithAcceleratorPort:` initializes `options = 0`;
- `IOGPUDeviceCreateWithOptions()` forms the user-client type as `5 | (options << 16)`;
- the ordinary path therefore calls `IOServiceOpen()` with type `0x00000005`;
- `IOGPU::newUserClient()` leaves `IOGPUDeviceUserClient +0x128 = 0`;
- `IOGPU::createDevice(..., 0)` propagates zero into `AGXShared::init(..., 0)`;
- `AGXShared +0xd8 = 0` is then passed into `AGXSecureGart::init(..., 0)`.

Combined with the independently proven normal-J615 accelerator soft-fault feature state remaining zero, the ordinary target reaches `AGXGart::isHWSoftFaultEnabled()` with **false / 0**.

## Linux boundary

Patch `0046` applies from Linux checkpoint `ccc976c598ab40f9cd8ef24837af1139979bbc09` to `65451e3d5cc8ebc582433abf874811c7eada79b9`.

The target-specific dormant `G15StockEmptySkuFinalizeInputs` no longer accepts an arbitrary `gart_soft_fault_enabled` value. The J615 owner-graph finalizer supplies `false` directly. The generic byte-exact `fw::compute::G15StockEmptySkuInputs` serializer deliberately remains parameterized so this exact J615 result is not generalized to unrelated targets or option-bearing user-client variants.

Validation:

- resulting tree: `7d9dce6e7d31964e1510a989be75b410f25575ad`;
- strict checkpatch: 0 errors, 0 warnings, 0 checks;
- Asahi module build: PASS at the established 24-individual-warning bring-up baseline;
- module SHA-256: `2e0b960a2d69686696888523c03053693b3c20cc6f4af80d45c1d5fe736f7358`;
- no live Queue call site or RunCompute writer was added;
- no module was installed and no RunCompute was issued.

## Remaining boundary

The dormant finalize surface is now free of the two target booleans closed by E128/E129. Its remaining explicit values are command/runtime addresses and counters: firmware-state FWVA, firmware stamp FWVA, optional user timestamp addresses, command counter, context ID, queue event sequence, UUID and stamp value. Firmware-state provenance is the next principal exact-source gap before any command-writer integration can be considered.
