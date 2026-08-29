# G15 two-phase stock-empty Compute asset preparation

E113 implements only the ordering refactor demanded by E112; it remains compile-only and unreachable.

Phase 1 keeps E110's already-armed-fence requirement, selects/binds the exact event-control and SKU rotating lifetimes, seeds event-control, reserves the selected `0x300` SKU slot **without writing it**, activates the stock-empty FList/HardwareBuffer epoch, and reserves Page-Pool-State/HWMetrics addresses. The non-Copy preparation token therefore knows the selected stream FWVA before SKU bytes exist.

Phase 2 requires the future RunCompute FWVA and keeps every still-unclosed E112 SKU source explicit: firmware state, CL channel state, CL resource-region base, firmware stamp, optional user timestamps, command counter, context ID, queue event sequence, event-control index, UUID, stamp value, soft-fault state and accelerator `+0x654` bit 7. Only then is the exact E102 stream serialized and written to the already-reserved slot. The reservation is re-derived and index/FWVA checked before the write.

Failures release the acquired FList HardwareBuffer reference and roll fresh event/SKU guard bindings back. Successful finalization still returns only the E111 host staging record; there is no RunCompute writer or Queue/submission call site.

Linux commit: `b21300a1ad4db8df298c493c98f014803d33ad5b`. Tree: `66a6a838a94275fe6352471b7c9c12804920ddba`. Module SHA-256: `067a3767ed6782a8ae0f22985de1d40f46bc995464e819f9df962c111c3630b6`. Strict checkpatch: 0/0/0. No module was installed and no RunCompute was issued.
