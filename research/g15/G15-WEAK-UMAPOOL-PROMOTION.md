# J615 weak UMAPool promotion model (E141)

E141 models the exact E137 shared-pool promotion/reference contract in safe Rust without making a UMAPool live.

Apple's four `AGXUMASharedPoolContainer` slots are weak/non-owning. Existing-slot selection succeeds only when a nonzero pool object reference can be acquired under the container lock. Each channel then has its own direct pool reference, while the nonzero epoch of pool `+0x20` carries one additional aggregate active-channel retain. Last-reference finalization clears a slot only after pointer-identity comparison.

The kernel Rust `Arc` implementation used by this tree explicitly has no weak-reference facility. Linux therefore still does not store an `Arc<G15SharedComputeUmaPoolOwner>` or raw pool pointer in the client container. Each slot contains only a globally unique `G15UmaPoolIdentity`, a channel-direct logical reference count, and the active-channel count.

`G15ClientUmaComputeChannelRef` owns the client-container `Arc`, not the pool. Compute slots are exactly `2 + priority_class`. Existing-slot promotion is mutex-protected and succeeds only for a logically live identity. Drop removes the active-channel contribution before the direct reference and clears the slot only if the same identity reaches zero, preserving replacement safety without a dangling weak pointer.

The code remains definition-only. It cannot create a pool or FList, has no Queue accessor/call site, and does not consume a global pool ID. The next integration boundary is coupling this identity/reference state to the actual shared Compute pool owner under one selection/create/replacement transaction.

Linux checkpoint: `a301b5b72feff2e32f69dd9e5e560a4c61a2ed60`, tree `f607a93285a3f615d554d424b100d09094ecc69d`.
