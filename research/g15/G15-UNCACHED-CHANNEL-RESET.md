# G15 uncached channel-memory reset header — E124

Exact 23J220 `AGXChannel::init()` derives channel `+0x54` as `min(second_integer, 0x80) << 4`. E119 proves normal J615 CL uses second integer `0x50`, so the value is exactly `0x500`.

Exact `AGXChannel::resetChannelState()` uses the selected uncached-channel-memory CPU pointer and writes only six u32 header locations: `+0x00/+0x10/+0x20/+0x30/+0x40 = 0` and `+0x50 = 0x500`. It does not bulk-clear the 0x2860 element and does not write the selected cached-channel-memory element in that routine.

E124 models only those proven writes on one caller-selected local element of the unreachable E123 range-7 owner. The returned private token contains only the local slot index, no FWVA. Global Apple resource-stack selection, cached-memory initialization, live WorkQueue ownership and RunCompute publication remain gated.
