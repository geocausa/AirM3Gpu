# G15 Compute SKU slot retirement/reuse

E105/E106 close how the exact 23J220 Compute SKU encoder prevents reuse of a still-live slot, without making any Linux SKU slot reachable from submission.

Exact `AGXSKUEncoder::beginEncoding()` starts at `(current + 1) % 0xf0`, scans at most all `0xf0` host events with `IOGPUEventMachine::testEvent()`, and selects the first completed slot. It then copies the CL producer's command-associated `IOGPUEvent` into that slot via `copyEvent()` before assigning the CPU write pointer. If no event is reusable Apple takes an explicit `No free block found for SKU stream` failure path.

The selected byte offset is `slot * 0x300`. `finishEncode()` adds that offset to the persistent E103 GPU backing, converts the selected address to FWVA, and returns it; G15 CL submission stores that exact FWVA at RunCompute `+0x760`. `scrubEvents()` only scrubs the `0xf0` host events.

Linux commit `69af01482ad8fbcba160559c308b8905598e3c6e` adds an unreachable conservative translation using the existing per-submission `JobFence`. It initializes the rotation point to `-1`, scans in Apple's exact order, treats only unbound/completed guards as reusable, binds the new fence before returning a slot, and fails with `ENOSPC` rather than allowing overwrite. Using a whole-submission fence can delay reuse compared with Apple's per-command event but cannot make reuse too early.

The guard has no call sites, does not write the E104 backing, does not call the E102 serializer, and leaves RunCompute `+0x760` unchanged/fail-closed.

Validation: tree `d9b6b43aec68a22ea9b84728dc410c418fca65f0`; module SHA-256 `77c7ca7cb034a416c45127c3eeec9822979da6d380f78fffa6740a69d3d5273b`; exact 24-warning baseline; strict checkpatch 0/0/0; exact-tree reconstruction PASS.

No module was installed and no RunCompute or other GPU command was issued.
