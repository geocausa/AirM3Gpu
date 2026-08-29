# G15 event-control shared backing — exact 23J220

E092 closes the backing classes beneath the 36-state event-control geometry without importing any older-generation layout.

Exact `AGXCommandBuffer::init()` calls ChinookV9 `AGXFirmware::allocateSharedData()` twice. The first shared-data descriptor is one contiguous logical `0x90` allocation (`36 × 4`) in eGartRange 7 with outer mapping option `0x700000007`; this is the already-proven normal range-7 compact-`0x007` / leaf-`0x00c0000000000447` class. The second descriptor is one contiguous logical `0x1b00` allocation (`36 × 0xc0`) in eGartRange 8 with outer mapping option `0x20800000007`; this is the independently proven range-8 compact-`0x003` / leaf-`0x00c0000000000443` class.

The command buffer builds parallel CPU/GPU subaddresses from those two mappings. Stamp CPU/GPU addresses use four-byte stride; event-control CPU/GPU addresses use `0xc0` stride. The selected event-control GPU address is the value later exported through descriptor `+0x148` to RunCompute `+0x14` after G15 GPUVA→FWVA identity conversion.

Construction-time state is exact: the rounded event-control backing is zeroed, then every block `+0x00` receives the FWVA of its corresponding four-byte stamp. On `nextCommandBufferState()`, Apple increments before selection modulo 36, zeroes the selected 0xc0 block and stamp, then seeds block `+0x00` stamp FWVA, `+0x08` stamp index, zero `+0x0c`, an exact but still neutrally named configuration value at `+0x10`, zero `+0x14/+0x18`, and `0xffffffffffffffff` at `+0xa8`.

The `+0x10` value is produced from accelerator `+0x2420` (fallback `+0x678`) and `+0x2428`. E092 does not assign an older-generation semantic or a J615 numeric value without an exact writer/value proof.

Consequence: Linux may model two page-base shared-bank1 backings using the already-proven range-7/range-8 classes and seed construction-time stamp links. Rotation-time activation and RunCompute `+0x14` must remain fail-closed until the exact J615 `+0x10` configuration is closed and the remaining first-command prerequisites are satisfied.

Exact macOS oracle result SHA-256: `4e1db24bfdd16eee280ef4f539275c415e5374066fddac630127e01df912bb9c`. No live RunCompute was issued.
