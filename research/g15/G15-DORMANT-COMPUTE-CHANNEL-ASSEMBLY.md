# J615 dormant Compute-channel assembly (E145)

E145 type-connects the already-proven J615 ownership tiers without attaching them to live submission. The client-VM shared-pool transaction now reaches the device-global UMA namespace and cached range-5 FList allocator only on the replacement path, while exact E142 serialization keeps replacement construction under the client-container mutex. Existing live pool slots are promoted without consuming another global pool identity.

After pool selection returns, the private Queue helper constructs the separate channel/command resources from E144's retained uncached range-5 plus bank1/q22 handles and the manager-owned `G15StatsComp`. Failure naturally drops the channel's modeled direct/active pool reference. The host-only HardwareBuffer owner cookie is no longer a free input: it reuses the globally unique nonzero UMAPool identity rather than creating a parallel guessed namespace.

The assembled `G15UnpublishedComputeChannel` remains private and has zero callers. `queue/compute.rs` is unchanged, no WorkQueue stores the object, no pool ID can be consumed by the live driver through this patch, and no RunCompute field is written.

Linux checkpoint: `899f1b6b8ba7bbf81b33c7aa75dcbdd56a1c9ed4`, tree `1d826ecb520897149f09a2b2336fee7c93ab38d2`.
