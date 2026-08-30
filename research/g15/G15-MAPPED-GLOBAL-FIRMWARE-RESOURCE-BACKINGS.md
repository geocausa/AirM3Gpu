# Mapped global J615 firmware-resource backings (E150)

E150 joins the exact accelerator-global resource-stack lifetime from E147-E149 to the real G15 shared-bank1 mapping classes and backing geometries previously proven for each resource type.

The dormant mapped factory has five exact identities: timestamp queue is range 7 with 0x18-byte elements in a 0x4000 backing; scheduler state is range 8 with 0x40-byte elements in a 0x4000 backing; channel state is range 8 with 0x24c0-byte elements in a 0x8000 backing; uncached channel memory is range 7 with 0x2860-byte elements in a 0x8000 backing; cached channel memory is range 8 with the same 0x2860/0x8000 geometry.

Each backing is page-base allocated through the existing q22-aware shared-bank1 allocator, zero-filled with the exact shared-data helper, and stored in the same dynamic backing slot whose mode-0 last-use destruction E149 closed. A selected private address is `backing_base + local_element * exact_element_bytes`; when the backing's last lease is returned, the slot disappears and dropping it owns the corresponding mapping teardown.

E150 also adds a dedicated semantic range-8 channel-state allocator over the already-proven compact-0x003 / leaf-0x...0443 protection shape, avoiding use of the FList Page-Pool-State allocator name for this new owner.

The safety boundary remains explicit. `G15DeviceFirmwareResourceState::new_mapped_device_global()` and the selected `mapped_fwva()` accessor each have zero callers. The real GpuManager constructor is unchanged and still uses E149's host-only resource-state constructor. Therefore E150 performs no new runtime mapping and publishes no selected resource FWVA.

The next blocker is exact boot-time q22 publication ordering relative to firmware/RTKit readiness. Until that is closed, the mapped constructor remains definition-only and live RunCompute remains fail-closed.

Linux checkpoint: `f17014117d4a77e7029a7f61abf85bf8c5aea5cf`, tree `592a70d8a512843c61cb5d36ad6f72d9ff62a03b`.
