# J615 client UMA weak-slot placement (E140)

E140 places only the non-owning shared-pool slot state at the exact client address-space lifetime established by E139. It does **not** make a UMAPool live.

Exact 23J220 `AGXShared::free()` explicitly releases and zeros `AGXShared +0x1b8`, the `AGXUMASharedPoolContainer` created by `AGXShared::init()`. `AGXUMASharedPoolContainer::free()` performs no traversal or release of its four pool slots. Together with E137, this confirms that the container is client-lifetime state while its pool slots are weak/non-owning.

Linux already places the bank-0 range-5 allocators in `file::Vm`, the address-space owner that E139 identified as the correct source for FList Page/Backup List mappings. E140 therefore adds one `G15ClientUmaPoolContainerState` to the same lifetime.

The state contains exactly four `Option<G15UmaPoolIdentity>` entries. These values are identities only; there is no `Arc<G15SharedComputeUmaPoolOwner>` or other strong pool reference in the container. The object is wrapped in the kernel mutex/Arc pattern so every future Queue targeting the same VM can eventually share the same synchronization boundary, but E140 exposes no Queue accessor.

This intentionally stops before weak-slot promotion. The kernel Rust `Arc` implementation in this tree provides no usable `Weak` facility for this owner, and E137 requires a nonzero-only try-retain/equivalent state machine before a slot may become a direct channel pool reference. E140 therefore initializes the four slots empty, consumes no global pool ID, creates no FList/UMAPool, and changes no RunCompute producer.

Linux checkpoint: `e48a1f854d3adf675374aad6b6bff352805904bf`, tree `0975d080969e61298bb5b36f6f0d4e1748586a89`.
