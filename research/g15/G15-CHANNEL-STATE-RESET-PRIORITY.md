# G15 selected channel-state reset and priority

E118 is an exact macOS 14.8.3 / 23J220 static audit of the selected `_AGFIChannelState` image. It makes no Linux source change and enables no GPU command.

`AGXChannel::resetChannelState()` zeroes the complete 0x24c0 selected slot, then rebuilds the first 0xb0-byte QueueInfo. Reset writes priority class 4 at `+0x30`, `0xffffffff` at `+0x2c/+0x4c`, channel priority/config at `+0x50`, converted GpuContext at `+0xa4`, and exact J615 CDM backoff byte 4 at `+0xac`; untouched bytes remain zero.

`AGXChannel::setPriority()` is a later mutation boundary. Exact `AGXArmFirmware::setChannelPriority()` writes `+0x30/+0x34/+0x38/+0x40/+0x44/+0x48` from context-priority, integer and QoS arguments. The inherited Linux `raw::PRIORITY` table is therefore not mechanically valid as a direct G15 priority-index table; at least one inherited entry does not match any corresponding exact Apple branch.

E118 also proves the first `AGXChannel::init()` integer is channel `+0x38` (the SKU `evctl_index` source), while command-queue fields feed channel `+0x4c/+0xf0/+0x1e8`. Those ordinary CL-channel constructor/priority call-site arguments remain the next exact source boundary.

Live G15 submission remains fail-closed. No module was installed and no RunCompute was issued.
