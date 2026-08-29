# G15 FList range-5 Page/Backup List PTE class — E082

E082 closes the last unresolved mapping class in the exact 23J220 `AGXUMAFList` four-resource constructor. This is static/oracle plus compile-only Linux work; no module was installed and no RunCompute was issued.

## Corrected caller option

For both `FList +0x50` (UMA Page Pool List) and `FList +0xb0` (UMA Backup Page List), exact `AGXUMAFList::init()` executes `mov x5,#3` followed by `movk x5,#0x200,lsl #32` before `AGXAccelerator::newGPUMappedBuffer(..., eGartRange=5, ...)`.

That value is **`0x20000000003`**. Older E074/E077 prose omitted two zeroes when transcribing the immediate; the exact instruction stream and E082 derivation use the corrected value.

`newGPUMappedBuffer()` merges range 5 and mandatory low bit 2, giving outer IOGPU mapping option **`0x20500000007`**.

## Exact SecureGart class

Exact 23J220 `AGXSecureMemoryMap::init()` produces mapping flag byte `+0x58 = 0x20`: eGartRange 5 is not a standard-FW mapping, outer bit 41 is set, and bit 39 is clear.

Across the complete range-5 aperture, the corresponding `AGXSecureGart::map()` path reduces the low class to zero. The low `|7` mapping makes the commit maparg bit zero, leaving the high block **`0x300`**. Therefore both persistent FList list mappings use compact UAT option:

`0x300`

Exact bank-0 UAT encoding has protection bits **`0x0080000000000000`**: GPU access enabled, AP=0, cached memory, PXN=UXN=0. Before physical-address OR, the exact leaf is `0x0080000000000c03` including AF/type and Apple's bank-0 bit.

This is bit-identical to the cached range-5 class already present in Linux, but the FList list resources retain a separate semantic name and lifetime.

## Linux checkpoint

Linux commit `724674ad034ee3502aa9448324cd0056b7b0c670` adds `PROT_G15_RANGE5_FLIST_LIST`, compile-time pins its protection bits, and exposes it through the side-effect-free `G15FListResourcePlan`. The plan still owns no allocator or GPU object, so this checkpoint cannot create or publish a Page-Pool State.

Validation:

- base: `affdd1fba79d333530cac22d069212b6e65ef9d4`
- tree: `8d1327b8eb514abd506e751f578bab40ebe937c1`
- module SHA-256: `f2d6d6838511448cd048800eecf8264811f43a54e8713dd6bb73d1fa46f661f7`
- exact existing 24-warning bring-up baseline
- strict checkpatch: 0 errors, 0 warnings, 0 checks
- no install and no RunCompute

## Remaining boundary

All four persistent FList mapping classes are now mechanically closed. Persistent resource construction still waits on the exact J615 producers for `M = AGXUMAPool +0x48` and `B = +0x50`, or a proof that the 2-GiB/4-MiB fallback values are the active target values. HWMetrics, exact G15 SKU execution-stream ownership, and stamp/notifier completion/recovery also remain live RunCompute blockers.
