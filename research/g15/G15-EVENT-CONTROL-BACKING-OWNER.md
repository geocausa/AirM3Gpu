# G15 event-control shared backing owner

E093 promotes the exact E092 backing contract into unreachable, compile-only Linux ownership without activating a command.

Linux commit `ddcbd85be239fa4db5143826bc3ca0588bacbd34` adds:

- exact 0x90 / 36×4 stamp-pool geometry beside the existing 36×0xc0 event-control pool;
- a partial exact 0xc0 event-control layout with compile-time assertions for the observed `+0x00/+0x08/+0x0c/+0x10/+0x14/+0x18/+0xa8` writes, while all still-unknown bytes remain padding;
- a hard-wired normal-range7 shared-event allocator class, distinct from the FList FW-Uncached range7 class;
- an exact-base shared-data array helper that deliberately bypasses ABI-shifting debug headers/trailing overflow padding and zeroes the complete rounded backing, matching `AGXFirmware::allocateSharedData()`;
- an unreachable owner for 36 shared `Stamp` words in range 7 plus 36 event-control blocks in range 8, requiring q22 mapping-notifier ownership and seeding only each control `+0x00` with its matching stamp FWVA.

The owner has zero call sites. It exposes no selected event-control FWVA, does not perform `nextCommandBufferState()` rotation-time initialization, and does not populate RunCompute `+0x14`. That field remains literal zero/fail-closed.

The remaining activation gate is exact event-control `+0x10`: 23J220 proves its producer from accelerator configuration fields, but E093 does not guess the J615 numeric value or semantic. Rotation-time stamp index/config/sentinel state stays inactive until that is closed.

Validation: Linux tree `2301c705e7d31892c9e06861aa4eb179e3809c2e`; module SHA-256 `dbb29792369f0a3ee8b66a5abd00501fd4a18d45d503ef701fb59c8e293319b4`; vermagic `7.1.6-gc6fa9b794ba9 SMP preempt mod_unload aarch64`; build PASS at the established 24-individual-warning baseline; strict checkpatch 0/0/0; patch 0021 exact-tree reconstruction PASS.

No module was installed and no RunCompute was issued.
