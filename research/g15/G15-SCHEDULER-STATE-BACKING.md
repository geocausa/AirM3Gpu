# G15 scheduler-state backing and QueueInfo +0xa4 provenance

E126 closes the exact 23J220 object carried by the inherited Linux `GpuContextData` naming without reclassifying the live generic allocator.

Normal G15 command queues select one `_AGFISchedulerState` from firmware resource stack `+0xdb8`. Exact stack construction uses eGartRange 8 and 0x40-byte elements. The initial two-element request page-rounds to one 0x4000 J615 backing, giving exactly 0x100 slots and no slack. This is the already-proven special-range-8 class: compact UAT option `0x003`, bank-1 leaf `0x00c0000000000443`.

The selected GPUVA is stored at command queue `+0x8d8`, converted to FWVA at `+0x8d0`, copied to channel `+0xf0`, and published by `resetChannelState()` at QueueInfo `+0xa4`.

`AGXCommandQueue::init()` clears exactly the first 0x38 bytes of the selected 0x40-byte state, then writes `+0x00/+0x01=0xff`, `+0x05=1`, `+0x22=0xff`, zero at `+0x23..+0x26`, and exact normal-J615 `AGXShared+0x100=2` at `+0x27`. Host construction does not clear `+0x38..+0x3f`; the compile-only model therefore preserves that tail on reset.

Linux commit `293ca63380179cca5bb8c6511a67349f90878b44` adds a dedicated unreachable range-8 scheduler-state allocator/backing, exact local-slot reset and private prepared token. It renames only the dormant channel-state source to `scheduler_state_fwva`. The live generic `GpuContext` lifetime and all RunCompute producers remain unchanged.

Validation: tree `2a2089948c3d9795ee385d364e70a5c39b162c92`; module SHA-256 `a5e31931d40181f8bb2aa2ce1bdc5a7c707b16603df87fd3209014554d817270`; exact 24-warning baseline; strict checkpatch 0/0/0; independent scheduler geometry/image audit PASS.

No module was installed and no RunCompute or other GPU command was issued.
