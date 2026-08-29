# G15 cached/uncached channel-memory backing — E122/E123

Exact macOS 14.8.3 / 23J220 static reconstruction closes the two firmware resource stacks that feed selected `_AGFIChannelState` QueueInfo `+0x00/+0x08`.

Normal J615 uses `fw_queue_count = 0x50`, so both stack element sizes are exactly `0x60 | (0x50 << 7) = 0x2860`. Each stack allocates page-rounded `0x8000` backing blocks containing three complete elements at offsets `0`, `0x2860`, and `0x50c0`, with `0x6e0` trailing slack.

The stacks remain semantically distinct. `AGXUncachedFWChannelMem` uses eGartRange 7, exact outer option `0x700000007`, compact UAT `0x007`, and leaf `0x00c0000000000447`. `AGXCachedFWChannelMem` uses eGartRange 8, exact outer option `0x20800000007`, compact UAT `0x003`, and leaf `0x00c0000000000443`. No new PTE class is introduced.

`AGXChannel::init()` selects one element from each stack and stores their GPU addresses at channel `+0x98/+0xa0`; `resetChannelState()` converts those addresses into QueueInfo `+0x00/+0x08`. `AGXChannel::free()` returns the selections through their respective resource-stack lifetimes before clearing the channel pointers.

Linux E123 encodes the exact J615 geometry and adds two independent unreachable backing owners plus dedicated semantic range-7/range-8 allocator constructors. Neither owner exposes a selected element FWVA, neither is connected to the live WorkQueue path or dormant Compute owner graph, and RunCompute publication remains fail-closed.
