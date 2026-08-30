# J615 mode-0 firmware resource backing lifetime (E149)

E149 closes the backing-reclamation policy for the five exact 23J220 resource stacks used by the dormant J615 Compute/channel model. Timestamp, scheduler-state, channel-state, uncached channel-memory, and cached channel-memory are all initialized with `AGXFWPoolShrinkMode = 0`.

The common template's `allocateNewBlock()` stores `(shrink_mode != 0)` in the backing allocation use-count, so these target backings start at zero. Selection increments that count. Every normal target release path passes `false` to `releaseAllocationBlockIfPossible()`. When the last selected element is returned and the backing count reaches zero, the template immediately removes the backing's complete global-index range from the availability bitmaps, frees the backing, and clears its backing-array slot.

The explicit `shrinkStack()` method is therefore not the ordinary mode-0 reclamation path: in normal state it searches for allocations with count 1, zeros them, and invokes the same release helper. The target mode-0 stacks naturally reach count zero through ordinary resource release.

This matters when a lower backing empties while a later backing remains active. The lower global-index range disappears rather than becoming immediately selectable. Free elements in still-present later backings are consumed first; only when the global availability bitmap is empty does growth recreate a backing, reusing an empty backing-array slot before extending the array.

Linux patch 0061 corrects E148's conservative retained-logical-backing approximation to this exact hole/recreation behavior. The change remains host-only: it adds no mapped firmware resource backing, selected FWVA, command writer, or RunCompute path. `queue/compute.rs`, `fw/compute.rs`, and `workqueue.rs` are unchanged.

Linux checkpoint: `368783dd61e2dd709d99b696feb4a8af9c7ec767`, tree `cebe8170d2e83af5c51d82ed2a358a7093b63205`.
