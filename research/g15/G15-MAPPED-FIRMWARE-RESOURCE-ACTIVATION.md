# J615 mapped firmware-resource activation boundary (E152)

E152 integrates E150's real mapped accelerator-global firmware-resource stacks into the G15 `GpuManager` construction path while preserving E151's exact q22 bootstrap asymmetry.

`build_pre_rtkit()` creates InitData and the inactive q22 notifier first. `make_mgr()` then creates the five mapped resource-stack first backings from the shared bank-1 root. Their allocator policy is `AfterActivation`, so these eager Apple-style PTE mappings do not produce q22 records. The existing G15 pre-RTKit manager validator already requires the q22 ring to be empty after the full graph is built, making accidental bootstrap publication fail closed.

The host q22 notifier is activated only after `MSG_INIT` has been consumed and q21 reports `firmware_ready == 1`. Activation is monotonic and requires an empty ring. In the persistent driver ordering, DRM registration follows successful `gpu.init()`, so userspace cannot create a Queue or select timestamp/scheduler/channel resources before activation.

This also gives the mapped backing lifecycle the correct two-sided failure behavior. Before activation, destroying a bootstrap backing emits no unmap because no corresponding map was published. After activation, later growth and teardown use ordinary q22 map/unmap. If unmap publication fails, the existing fail-closed MMU path preserves the PTE and VA reservation instead of allowing stale firmware state to point at a reused address.

The current J615 probe still retains the E075 `ENODEV` gate before persistent `GpuManager`/RTKit/DRM construction. Patch 0064 does not remove that hardware gate, does not change `queue/compute.rs`, `fw/compute.rs`, or `workqueue.rs`, and does not consume selected resource FWVAs in RunCompute.

Linux checkpoint: `d9f5cf4cddeb2a109cc9c074fff080102d5d89dd`, tree `55edccc4efa3efb01c4ed516f70b2ce282a11ec7`.
