# J615 shared Compute UMA placement — E135

E135 closes the persistent ownership topology needed after the E134 fence transaction. This is exact macOS 14.8 / 23J220 static reconstruction only. No Linux source changed, no module was installed, and no RunCompute or custom GPU command was issued.

## Accelerator-global UMAPool HardwareBuffer IDs

Exact `AGXAcceleratorG15::start()` initializes `AGXHardwareBufferIDManager` at accelerator `+0x2a08` with count `0x100` and the exact name `"UMAPool"`. This is a separate namespace from accelerator `+0x29c0`, count `0x7f`, named `"ParamBuffer"`.

`AGXUMAPool::prepareLocked()` later loads the pool's accelerator pointer, adds `0x2a08`, and calls `AGXHardwareBufferIDManager::alloc()` with the pool FList as owner. The 256-entry ID namespace is therefore accelerator-global while sticky ownership remains tied to the individual FList/pool object.

## Normal CL channels reuse shared UMAPools

Exact `AGXChannel::init()` obtains an `AGXUMASharedPoolContainer`, locks it, and indexes four shared pool slots by:

`((DataMasterType == 2) ? 2 : 0) + priority_class`.

The exact CL subclass path uses DataMasterType `2`, selecting one of the two Compute slots. If that slot already contains a retainable pool, the channel reuses it. Otherwise the accelerator `halNewUMAPool()` vslot creates a pool, its init receives shared/reusable=`1` and async-grow=`1`, and the new pool is inserted into that shared slot. The selected pool is retained at `AGXChannel +0x188`; channel cleanup calls `AGXAccelerator::removeUMAPool()` and releases the channel reference.

`AGXUMASharedPoolContainer::init(AGXAccelerator*, AGXShared*)` independently establishes four zeroed pool slots at `+0x48/+0x50/+0x58/+0x60`, with `AGXShared*` at `+0x30` and `AGXAccelerator*` at `+0x38`.

This proves the Compute UMAPool/FList is not inherently a unique per-channel object.

## Pool ID is creation-order state

`AGXUMAPool::init()` increments the global qword at `0xfffffe000be03a70` and stores the incremented value at pool `+0x80`. The exact kernel image initializes that global to zero. `populateFirmwareState()` later copies pool `+0x80` into Page Pool State `+0x00`.

The first UMAPool created after boot would therefore receive `1`, but a particular Compute pool's ID depends on all preceding UMAPool creation. It is not a J615 Compute constant and must not be hard-coded merely to make dormant Linux construction callable.

## Linux placement consequence

The E134 dormant `G15StockEmptyComputeOwnerGraph` still constructs one `_flist` by value next to HWMetrics/SKU/channel resources and accepts a freshly supplied HardwareBuffer-ID manager, owner cookie and pool ID. That grouping is harmless only because it has zero call sites.

Exact Apple ownership requires at least three separate tiers before live placement:

- accelerator/device-global UMAPool HardwareBuffer-ID manager;
- reusable shared Compute UMAPool/FList ownership with globally ordered pool identity;
- channel/command owners that retain or reference that shared pool while following their independently proven lifetimes.

Therefore a direct per-Queue instantiation of the E134 owner graph is explicitly blocked. The next safe Linux change is a definition-only ownership split; it is not a RunCompute writer.
