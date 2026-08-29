# G15 event-slot finish/reuse — E095

Exact macOS 14.8.3 / 23J220 closes the reuse lifetime behind the 36-state command-buffer event-control ring.

`AGXCommandBuffer::nextCommandBufferState()` advances the selector modulo 36, then calls mutable `IOGPUEventMachine::finishEvent()` on the matching host event **before** clearing the corresponding 0xc0 event-control block or four-byte GPU-facing stamp. `finishEvent()` waits for every active stamp dependency, then resets the host event; it does not release stamp indices or free GPU resources.

E095 also corrects a provisional E092/E094 interpretation: event-control `+0x08` is not a stamp index. It is the unmasked `AGXShared` command-buffer-state sequence. The selected-state seed is therefore `+0x00=stamp FWVA`, `+0x08=shared state sequence`, `+0x0c=0`, `+0x10=0x50` on normal J615, `+0x14/+0x18=0`, and `+0xa8=-1`, with the remaining reset bytes zero.

Linux must preserve a host-side retirement guard before slot reuse, but does not need to clone the old IOGPU 0x40 host-event layout if its existing fence lifetime expresses the same barrier. RunCompute `+0x14` remains zero/fail-closed.
