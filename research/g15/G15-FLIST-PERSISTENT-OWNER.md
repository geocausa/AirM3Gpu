# G15 FList persistent backing owner — E086

E086 turns the previously proven J615/23J220 FList geometry into a typed **compile-only, unreachable** owner for the four persistent backings. It deliberately stops at the `AGXUMAFList::init()` boundary and does not publish a Page-Pool-State pointer to any command.

## Owned backing set

Linux commit `3cd3f336d9f4` represents:

- Page Pool List: range 5, 4 MiB, `0x80000` u64 entries;
- Backup Page List: range 5, 32 KiB, `0x1000` u64 entries;
- FW-Uncached State: exact 8-byte object in the distinct FList range-7 class;
- Page-Pool State: exact 0x70-byte object in range 8.

The range-5 list allocations use the per-VM cached range-5 allocator whose PTE bits are exactly the E082 compact-`0x300` FList list class. The fixed bank-1 objects use the dedicated range-7 FList and range-8 constructors. E085 already closes the q22 special-aperture notification encoding that a future live range-8 mapping requires.

## Deliberate constructor boundary

The owner seeds only the fields that exact `AGXUMAFList::init()` seeds immediately:

- Page-Pool State `pool_id`;
- Page-Pool State HardwareBuffer ID = `0xffffffff`.

It does **not** populate Page/Backup List pointers, list capacity, FW-Uncached pointer/mirror, page count/cursors, shared-compute state or a live HardwareBuffer ID. Those belong to the later `prepareBufferResources()` / `populateFirmwareState()` lifetime.

The owner also intentionally exposes no Page-Pool-State FWVA accessor and has no runtime caller. This prevents the compile-only allocation model from silently becoming an executable RunCompute prerequisite.

## Relation to the public m1n1 cross-check

E084 reinforced the same separation conceptually: Apple GPU generations already distinguish persistent manager/list storage from active command stamps/events/context state. E086 uses that only as design discipline; every size, mapping class and initialization rule here still comes from exact 23J220 evidence.

## Validation

- Linux base: `9b21157497bbb1695b9bbdb43d6490c23520504e`;
- Linux checkpoint: `3cd3f336d9f4c103f2df7e284b9a517a42e7d90f`;
- tree: `65804f29fac725207b173f8e71f2f79f64fcf3f4`;
- module SHA-256: `ab9ea1342170bea6526b440d245a0bb4a88831db09018e0773d9673894dbdcf4`;
- strict checkpatch: 0 errors, 0 warnings, 0 checks;
- patch 0018 exact-tree reconstruction: PASS;
- runtime/install: none.

## Boundary

The next exact step is not RunCompute. The owner still needs the HardwareBuffer-triggered prepare/populate/complete state transition, and live execution still requires HWMetrics, exact SKU production, and stamp/notifier completion/recovery closure.
