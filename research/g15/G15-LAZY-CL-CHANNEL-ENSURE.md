# J615 lazy CL-channel ensure transaction (E156)

Exact 23J220 `AGXCommandQueue::chooseCLWorkQueue()` does not publish a newly created CL WorkQueue into the command queue until the local WorkQueue has initialized, the J615 CL channel has been created and initialized, its priority has been applied, and success bookkeeping is complete. Failure releases the local channel and WorkQueue without publication. `processComputeSetup()` consumes the returned WorkQueue only after this boundary and then reads its `+0x1e8` channel.

Linux already owns an inherited Compute WorkQueue eagerly. E156 therefore treats E155's WorkQueue-owned empty channel slot as the equivalent publication point. A private ensure helper locks that slot, reuses an existing channel if present, otherwise constructs the complete E154 channel bundle in local RAII state, and writes `Some(channel)` only after construction succeeds.

The slot mutex has no reverse acquisition path in the existing pool/resource/range-5/q22 graph, so it safely serializes hypothetical future concurrent first-use attempts. The helper itself has zero callers. `queue/compute.rs`, `fw/compute.rs`, `workqueue.rs`, and the E075 submission gate remain unchanged.

Linux checkpoint: `4463837493947d05a8a1e44b9d11329f96467856`, tree `8fc5403bbb85848f7c49011452a9b67ea97514b1`.
