# G15 stock-empty RunCompute command sources — E131

E131 closes the remaining command-local scalar inputs at the dormant normal-J615 stock-empty SKU finalize boundary. This is exact-target static reconstruction plus compile-only Linux modeling. No module was installed and no custom GPU command was executed.

## Exact 23J220 sources

`AGXCLChannelG15::encodeCLCommandSKUStream()` reads these values from the already-initialized Compute command:

- RunCompute `+0x04` — command-queue counter;
- RunCompute `+0x10` — managed `AGXContextIDManager` context ID;
- RunCompute `+0x7f0` — `G15JobMeta.fw_stamp`;
- RunCompute `+0x7f8` — `G15JobMeta.stamp_value`;
- RunCompute `+0x808` — `G15JobMeta.uuid`;
- RunCompute `+0x80c` — `G15JobMeta.event_seq` / queue-local event sequence;
- RunCompute `+0x828/+0x830` — user timestamp pointer presence.

The queue-event-sequence equivalence is independently exact: E114 proved descriptor `+0x48c` is copied into command `+0x80c`, while the SKU setup packet reads descriptor `+0x48c`. E070 proved the timestamp record does not embed either external user timestamp address; it writes `command_fwva + 0x828` if either command pointer exists, otherwise zero.

Linux already owns the corresponding producers: the command-queue counter, managed G15 `VmBind` context ID/generation, `ev_comp.fw_stamp_pointer`, `ev_comp.value.next()`, G15 command UUID, `ev_comp.event_seq`, and typed user timestamp objects.

## Linux boundary

Patch `0048` applies from E130 checkpoint `635fca8e84b3d6ba27f6f7dd760275c635cd3495` to E131 `251a60a085f3bb9bfdcb04f1d6fd28f24dbf2cbb`, reconstructing tree `a54e4d9c9792d286e70607aa388f5c0dc61dbe9a` exactly.

The compile-only delta:

- removes `G15StockEmptySkuFinalizeInputs` completely;
- makes the dormant finalizer consume the typed `RunComputeG15V14_7` image plus its FWVA;
- derives the firmware stamp/value, command counter, managed context ID, queue event sequence, UUID and user-timestamp presence from that same command image;
- narrows generic serializer timestamp inputs to one exact `user_timestamps_present` predicate;
- gives G15 RunCompute `+0x10` its exact semantic name `g15_context_id_10` while preserving the same `VmBind::slot()` producer and byte layout;
- adds compile-time locks for source offsets `+0x04`, `+0x10`, `+0x7f0`, `+0x7f8`, `+0x808`, `+0x80c`, `+0x828` and `+0x830`;
- leaves the generic byte serializer parameterized and adds no live command writer.

Validation: strict source-diff checkpatch `0/0/0`; module build PASS at the established 24-individual-warning baseline; module SHA-256 `affcea840e5e49f362a562c4a20f0933a64741a7f44385b17ae4600ab77aec90`; exact patch-tree reconstruction PASS.

The remaining work is no longer an E102 scalar-source problem. It is transaction/lifetime and command-body completeness: placing the persistent owners at the right runtime scope and closing still-zero Apple-active pre-SKU/JobParameters2 fields before any live RunCompute writer can be considered.
