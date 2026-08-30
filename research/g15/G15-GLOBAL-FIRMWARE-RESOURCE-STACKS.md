# J615 global firmware resource stacks

E147 closes the ownership and selector scope of the five exact 23J220 firmware resources used by the dormant Compute-channel model.

All five allocators live in the accelerator-owned `AGXFirmware` object, not under an individual command queue or channel:

- `_AGFIChannelState` at `AGXFirmware +0xa98`, range 8, element size `0x24c0`;
- `_AGFITimeStampQueue` at `+0xb60`, range 7, element size `0x18`;
- `AGXUncachedFWChannelMem` at `+0xc28`, range 7, J615 element size `0x2860`;
- `AGXCachedFWChannelMem` at `+0xcf0`, range 8, J615 element size `0x2860`;
- `_AGFISchedulerState` at `+0xdb8`, range 8, element size `0x40`.

Exact `AGXFirmware::allocFirmwareData()` initializes the timestamp, channel-state and both channel-memory stacks and immediately creates their first backing. `AGXArmFirmware::allocFirmwareData()` does the same for the scheduler stack. The backing lifetime is therefore firmware/device-global rather than channel-local.

The five template instances share the same two-level free-space transaction. Under the stack mutex they select the least-significant available outer bitmap bit, then the least-significant free bit in that 32-bit inner word, forming a global resource index as `inner + outer * 32`. The index selects an allocation block and an element within that block; selection increments the backing block's live-use count and clears the corresponding availability bits. Growth extends the same global index space up to `0x800` entries. Release accepts the global index, decrements the owning block use count, restores the free bits, and only then applies the stack's release/shrink policy.

Selected indices are independently owned by their exact host objects:

- timestamp index: `AGXTimeStampQueue +0x18`;
- scheduler index: `AGXCommandQueue +0x8c8`;
- channel-state index: `AGXChannel +0x40`;
- uncached channel-memory index: `AGXChannel +0x44`;
- cached channel-memory index: `AGXChannel +0x48`.

This makes a fixed first-slot assumption invalid. Scheduler and timestamp selections can already exist before the first CL channel, and TA/3D/CL channels draw from the same three channel resource stacks. None of these five indices can be hard-wired to zero, coupled to one another, or reused as an event-control/UMAPool identity.

The E127 local selected-owner graph remains useful as a byte/layout proof, and E145/E146 remain safe because their unpublished assembly helper has no callers. They are not yet a liveable ownership placement, however: a future Linux integration must replace the per-channel local backing ownership with device-global resource-stack owners plus independent command-queue/channel selection leases before any dormant Compute bundle can become reachable.

No Linux source change is made by E147. Live G15 RunCompute remains fail-closed.
