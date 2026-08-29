# G15 coherent channel-lifetime owner graph

E127 integrates the exact channel prerequisites closed by E116-E126 into the dormant stock-empty Compute owner graph while preserving a fail-closed live path.

The graph now owns separate timestamp-queue (range 7), scheduler-state (range 8), channel-state (range 8), uncached channel-memory (range 7) and cached channel-memory (range 8) backings alongside the existing CL command resource, event-control, HWMetrics, FList/HardwareBuffer and SKU resources.

Each firmware resource stack keeps an independent local selection index. No timestamp/scheduler/channel-state/cached/uncached index equality is assumed. A definition-only phase-1 model derives QueueInfo `+0x00/+0x08/+0x10/+0x18/+0xa4` from the selected owned resources, including `+0x18 = channel_state + 0xb0`.

The E113/E115 SKU finalize input therefore no longer accepts an external `channel_state_fwva`; it consumes the privately prepared channel-state token. Timestamp update mode, queue `+0x4c`, effective priority/QoS and several command/SKU values remain explicit runtime inputs.

Linux commit `8f6bd394ceb5bdcd62bfa2ca3817c85bd7cabf9d` remains unreachable from Queue submission and provides no RunCompute writer. Validation: tree `87431d2ef1ea1a35882d5a499117d73ae295ea30`; module SHA-256 `a2d0cbf1d48639b4271f9136e60c70e8d80113848a3e13cda2db1b3a1d69a48a`; exact 24-warning baseline; strict checkpatch 0/0/0; independent distinct-index channel-pointer audit PASS.

No module was installed and no RunCompute or other GPU command was issued.
