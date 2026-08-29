# G15 Compute SKU backing ownership

E103/E104 close the exact persistent GPU backing used by the 23J220 G15 Compute SKU encoder while keeping it unreachable from Linux submission.

Exact 23J220 `AGXFirmware::allocFirmwareData()` derives `0xf0` usable CL command entries on normal J615. It then calls the Compute `AGXSKUEncoder::createBuffer()` with block size `0x300` and block count `0xf0`. The encoder's logical slot storage is `0x2d000` bytes and the 16-KiB-page-rounded mapped backing is exactly **0x30000 bytes**. The E101 stock-empty stream uses `0x2c0`, leaving `0x40` bytes unused per slot. Apple's separate host reuse array is `0xf0 × 0x40 = 0x3c00` bytes.

The GPU backing maps through exact option `0x20800000007`, the independently live-calibrated special range-8 class: compact UAT `0x003`, leaf `0x00c0000000000443`. `AGXSKUEncoder::createBuffer()` retains the SysMemory/map for encoder lifetime and contains no explicit per-command `prepareMapping()` / `completeMapping()` epoch. `AGXSKUEncoder::free()` releases the host event array plus the retained GPU mapping and SysMemory.

Linux commit `d54dfc78d0592e2b8bb239aac61b9bc0489e3a78` adds one unreachable page-base range-8 `G15SkuBacking` of exactly 0x30000 bytes, with compile-time `0xf0 × 0x300` geometry. It requires q22 mapping-notifier ownership, verifies the backing is page-aligned/zero-filled, and deliberately exposes **no selected slot or FWVA accessor**. The E102 serializer has no owner reference, the owner has no runtime call site, and RunCompute `+0x760` is unchanged.

Validation: tree `16d86879298422b44475805f6cbcc2a1502579e1`; module SHA-256 `4ca508de93085f783563ce7dbf0c8043dd7bdd759ba8cbeaf221aea37fd71215`; exact 24-warning baseline; strict checkpatch 0/0/0; patch exact-tree reconstruction PASS.

No module was installed and no RunCompute or other GPU command was issued.
