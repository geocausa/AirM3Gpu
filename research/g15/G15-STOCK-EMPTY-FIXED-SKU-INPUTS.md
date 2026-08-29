# G15 stock-empty fixed SKU inputs — E128

E128 closes two remaining stock-empty Compute SKU serializer inputs for the exact normal J615 / macOS 14.8.3 (23J220) target. This is static clean-room reconstruction plus compile-only Linux modeling; no custom GPU command was executed.

## First CL event-control index

The exact constructor chain established by E114/E119 is:

- `AGXChannel::init()` stores its first integer constructor argument at channel `+0x38`;
- G15 Compute submission copies channel `+0x38` to `G15JobMeta.evctl_index`;
- the SKU setup packet consumes that `evctl_index`;
- `chooseCLWorkQueue()` starts from the zero-filled command-queue CL-workqueue index and increments it only after a successful channel construction.

Therefore the **first normal CL channel uses `evctl_index = 0`**. The dormant J615 owner graph now uses the already-defined `G15_J615_FIRST_CL_EVCTL_INDEX` instead of accepting an arbitrary finalize input.

## Accelerator packed feature bit 39

The E101 setup packet's byte at payload-relative `+0x178` comes from accelerator halfword `+0x654`, bit 7 — overall packed-feature bit 39.

Exact 23J220 configuration proves the target value is zero. Base `AGXAccelerator::configureDevice()` applies packed-feature mask:

`0xf4840fffffff7f`

which clears bit 39 unconditionally. All subsequent base masks touching the field preserve that bit and no later base OR sets it. `AGXAcceleratorG15::configureDevice()` preserves the bit, and `AGXAcceleratorG15G::configureDevice()` finally ORs:

`0x2002100000180`

whose bit 39 is also zero. Thus stock J615 reaches the SKU encoder with **accelerator `+0x654` bit 7 = 0**.

Linux hard-wires `false` only at the exact J615 dormant owner-graph boundary. The generic byte-exact SKU serializer continues to carry the field as an input so this target result is not generalized to unrelated hardware.

## Compile-only checkpoint

Patch `0045` applies from Linux checkpoint `8f6bd394ceb5bdcd62bfa2ca3817c85bd7cabf9d` to `ccc976c598ab40f9cd8ef24837af1139979bbc09`.

Validation:

- resulting tree: `b02f2c4472e65afee9c404759da93622e3840db6`;
- exact-tree patch reconstruction: PASS;
- strict checkpatch: 0 errors, 0 warnings, 0 checks;
- Asahi module build: PASS at the established 24-individual-warning bring-up baseline;
- module SHA-256: `ced66b40865979c5aa70686673ae939058ea04d4db789428285d87f2f19da66e`;
- live G15 RunCompute producers remain fail-closed.

## Remaining boundary

The dormant finalize surface still carries firmware-state FWVA, firmware stamp FWVA, optional user timestamp addresses, command counter, context ID, queue event sequence, UUID, stamp value, and GART HW-soft-fault state. Several are already exact values in the existing Linux Compute constructor but are intentionally not bridged into the dormant graph yet. Firmware-state provenance and the exact GART soft-fault bridge remain the main source-closure work before any command writer can be considered.
