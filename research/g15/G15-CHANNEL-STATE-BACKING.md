# G15 channel-state backing

E116 closes the exact 23J220 backing behind the CL channel-state FWVA consumed by the Compute SKU stream. Firmware owns a global `_AGFIChannelState` resource stack whose elements are exactly `0x24c0` bytes. It allocates page-base `0x8000`-byte blocks in eGartRange 8; three states fit at offsets `0x0000`, `0x24c0`, and `0x4980`, leaving `0x11c0` bytes of slack. The exact outer option is `0x20800000007`, reducing to compact UAT `0x003` and the already-proven bank-1 leaf shape `0x00c0000000000443`.

`AGXChannel::init()` stores the selected address as `block_gpuva + slot_index * 0x24c0` at channel `+0x90`. `resetChannelState()` zeroes the complete selected state before repopulating it; its first `0xb0` bytes are the independently reconstructed QueueInfo. Release returns the state to the same firmware resource stack.

E117 Linux commit `252ded3b63533ab89b45e965546f2f0b9d2e57a2` models only this backing geometry. It pins the exact state/block/slot constants and adds an unreachable special-range-8 `G15ChannelStateBackingBlock`. The owner exposes only pure slot-offset arithmetic: it has no selected-slot FWVA accessor, no selection/release state, no QueueInfo initialization, and no runtime caller.

RunCompute remains fail-closed. The dormant SKU `channel_state_fwva` input is intentionally retained until selected-slot lifetime and the reset/population image are represented together.
