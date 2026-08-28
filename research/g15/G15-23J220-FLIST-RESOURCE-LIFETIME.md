# G15 23J220 FList persistent-resource lifetime

Status: E077/E078 exact-target closure, compile-only Linux modeling. No RunCompute is enabled.

Exact 14.8.3 / 23J220 `AGXUMAFList::init()` creates four persistent GPU resources: Page Pool List and Backup Page List in eGartRange 5, a fixed 0x70-byte Page-Pool State in eGartRange 8, and a fixed 8-byte FW-Uncached State in eGartRange 7. The Page-Pool State publishes those backing addresses at `+0x14/+0x34/+0x48`; firmware requires the qword pointed to by `+0x48` to equal the cached mirror at `+0x50` before descriptor refresh.

For pool maximum `M`, block size `B`, and host page size `P`, Apple computes Page Pool List bytes as `align_up(M >> 9, P)` and Backup Page List bytes as `align_up((M / B) * 64, P)`. The matching G15G vslots resolve to `getUMAPoolMaxSize()`, `getUMABlockSize()` and `getUMARingBufferSizeMax()`. Their 2 GiB/4 MiB fallback values would produce 4 MiB and 32 KiB lists, but E077 deliberately does not claim those numeric sizes as exact J615 values because the override-field producer is not closed.

A read-only stock user-client probe (type 5, selector `0x10f`) succeeds but reports four zero qwords before/through/after an empty Compute command, so it is live accounting rather than a static-limit oracle.

The two range-7 PTE classes are distinct: PM/q22 uses leaf `0x00c0000000000447` (compact `0x007`), while FList FW-Uncached State uses `0x00c000000000044b` (compact `0x00b`). Page-Pool State remains the E075-proven range-8 leaf `0x00c0000000000443`. Linux commit `886820e1f460` models the 8-byte FW-Uncached ABI and hard-wires the FList range-7 allocator class while sharing the same collision-safe range-7 VA arena. Neither fixed FList object is instantiated yet.

Next boundary: model HardwareBuffer-ID allocation/prepare/complete ownership and only then decide how to instantiate the full FList resource set without guessing override-sensitive Page/Backup List sizes.
