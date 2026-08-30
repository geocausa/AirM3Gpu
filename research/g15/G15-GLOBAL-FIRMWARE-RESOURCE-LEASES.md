# J615 global firmware resource leases (E148)

E148 turns the E147 global-selector proof into host-side Linux lifetime bookkeeping without exposing any firmware-visible resource address.

G15 now owns one `G15DeviceFirmwareResourceState` at `GpuManager` lifetime. It contains five independently locked logical resource stacks for timestamp queue, scheduler state, channel state, uncached channel memory, and cached channel memory. Their exact already-proven elements-per-backing are `0x2aa`, `0x100`, `3`, `3`, and `3` respectively.

Each stack models the common exact 23J220 selector layer: a `0x800` global-index ceiling, eager first logical backing, one 64-bit outer availability bitmap, 64 32-bit inner free words, lowest-free selection, per-backing live-use counts, logical growth only when no free index remains, and inverse release. Linux `trailing_zeros()` expresses the same least-significant-set-bit choice as the host's `rbit`/`clz` sequence.

A selected global index is represented by an RAII lease. Dropping the lease decrements its logical backing use count and restores the corresponding inner/outer availability bits. Separate grouped lease objects preserve the exact host lifetimes: timestamp plus scheduler are retained by command-queue lifetime, while channel-state plus cached/uncached channel-memory leases belong to the dormant AGXChannel lifetime.

Actual G15 Queue construction now acquires the timestamp/scheduler logical leases before taking the general kernel allocator lock. If any later Queue construction step fails, normal Rust drop returns those global indices automatically. The zero-caller unpublished Compute-channel assembler similarly acquires the three channel-resource leases and rolls them back on later failure.

This does **not** promote E127's local backing proof indices into global indices. Those local slot numbers remain byte-image proof coordinates only. E148 also allocates no firmware resource-stack GPU backing and a lease cannot produce an FWVA. `queue/compute.rs`, `fw/compute.rs`, and `workqueue.rs` are unchanged, so this patch creates no RunCompute writer or submission path.

The remaining ownership boundary is now explicit: close the normal-J615 block release/shrink policy, then replace the E127 local proof blocks with device-global mapped backing arrays so a selected global index can resolve through `index / elements_per_backing` and `index % elements_per_backing` to the exact backing and in-block FWVA.

Linux checkpoint: `1308998922f430f77c5e19ecee33dc5799bfdb8f`, tree `8c1d9c20f68a199ae0ca777bb51ce75265da545c`.

No module was installed and no RunCompute or other custom GPU command was issued.
