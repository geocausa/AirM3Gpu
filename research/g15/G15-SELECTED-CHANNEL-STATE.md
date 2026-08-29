# G15 selected channel-state model

E118-E120 close the exact macOS 14.8.3 / 23J220 host boundary for a selected normal-J615 CL `_AGFIChannelState` while keeping it unreachable from Linux submission.

E118 proves Apple clears the complete selected 0x24c0-byte state before rebuilding QueueInfo. The reset image writes converted channel-owned firmware addresses at +0x00/+0x08/+0x10/+0x18, `0xffffffff` at +0x2c and +0x4c, reset priority class 4 at +0x30, the channel configuration integer at +0x50, converted GpuContext FWVA at +0xa4, and J615 CDM backoff byte 4 at +0xac. The later priority setter mutates only +0x30/+0x34/+0x38/+0x40/+0x44/+0x48. It also proves the inherited Linux `raw::PRIORITY` table is not a direct G15 priority table.

E119 mechanically closes the ordinary CL constructor sources. `chooseCLWorkQueue()` supplies the pre-increment CL-workqueue index as the channel `evctl_index`, so the first ordinary CL channel has index 0. The normal-J615 second constructor integer is exactly 0x50. Channel priority is derived from effective IOGPU priority/QoS with exact integer argument 2; runtime priority/QoS remain explicit rather than being replaced by a guessed constant.

Linux E120 commit `98d829e05a896cd7b2fa05c626aaef84128c2c9e` promotes only that proven byte-image boundary into compile-only code. One already-selected state slot is zeroed, the exact reset QueueInfo image is reconstructed, and the exact normal-J615 priority mutation is applied. Resource-stack slot index and channel `evctl_index` remain separate concepts. The helper returns only a private prepared-state token and has no conversion to SKU input, RunCompute, or a live channel.

Validation: tree `3be13579f13cdc311255cc86a2588446c516ff71`; module SHA-256 `7bb1fbc5efdb135b1d72a5c31342689dc0381991629b28031ef4c3e060b672f0`; exact established 24-individual-warning baseline; strict checkpatch 0/0/0; independent layout audit PASS. Patch 0038 reconstructs the exact tree from E117.

No module was installed and no RunCompute or other custom GPU command was issued.
