# G15 wrapper +0x24c control state

The G15 runtime wrapper field at +0x24c points to an exact 0x60-byte firmware control/state object (Apple CPU/GPU pair identified in the allocation table; the GPU/FW address is installed by `AGXArmFirmware::initFirmwareSharedData`).

Direct RTKit-2419.140.12 firmware accesses prove:

- +0x00 u32 state; state `2` is a special firmware mode and the firmware clears the word on transition.
- +0x04 u32 request-pending flag; firmware control message type 5 sets it to 1.
- +0x08 u32 request-latched flag; firmware writes `(request_pending != 0)`.
- +0x0c/+0x10/+0x14 three contiguous u32 counters, walked by a 4-byte loop.
- +0x18 u32 aggregate counter.
- +0x1c unaligned u64 timestamp/state value.
- +0x24 u32 active flag.
- +0x28 u32 active ID.
- +0x2c unaligned u64 secondary timestamp/state value.
- +0x34 u32 event count.
- +0x4c unaligned u64 converted timestamp. Firmware computes `(ticks * 0x7d + 1) / 3`, the 24-MHz tick-to-nanosecond conversion.
- remaining bytes are currently kept as exact padding.

The kernel model uses an exact 0x60 `G15ControlState`, hard size/offset assertions, and a dedicated `GpuObject` backing allocation instead of an opaque byte array. No runtime G15 dispatch exists yet.
