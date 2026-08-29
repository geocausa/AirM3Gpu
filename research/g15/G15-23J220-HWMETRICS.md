# G15 23J220 UMA HWMetrics backing

E097/E098 close the channel-owned UMA HWMetrics backing used by G15 Compute RunCompute `+0x857` without making that pointer live.

## Exact 23J220 producer

`AGXUMAHWMetrics::init()` owns one host page. On J615/T8122 that is 0x4000 bytes. The page is mapped at page-base logical offset, prepared for the HWMetrics/channel lifetime, and zeroed during construction. HWMetrics starts with record offset zero; `AGXUMAFList::updateSubmitInfo()` advances the offset by 0x40 modulo 0x4000, giving exactly 0x100 records.

The stock empty Compute descriptor keeps its HWMetrics offset zero, so its eventual RunCompute `+0x857` value is the metrics mapping base itself.

The exact mapping option is `0x0001008700000007`, selecting eGartRange 7 but not the ordinary PM/event range-7 class. Exact SecureMemoryMap/SecureGart reduction gives compact UAT option `0x30b`. The final IOMemoryDescriptor direction filter does not fire: the allocation options force direction bits incompatible with the filter's special value 2.

Exact bank-1 `encodePTEFlags(1, 0x30b)` produces leaf bits:

`0x00e000000000040b`

Protection bits without AF/type are `0x00e0000000000008`, bit-identical to Linux `PROT_GPU_FW_SHARED_RW`.

## Linux E098 checkpoint

Linux commit `d18178f018aca2b92249ab72c5c361e19b6f45dc` adds only unreachable compile-only ownership:

- a third semantic range-7 aperture class for HWMetrics, sharing the existing collision-safe VA arena;
- hard aperture/protection pairing to `PROT_GPU_FW_SHARED_RW`;
- exact leaf validation for `0x00e000000000040b`;
- a dedicated HWMetrics allocator constructor;
- one zeroed page-base 0x4000-byte owner with exact 0x40/0x100 ring geometry.

The owner has no runtime constructor call site and exposes no FWVA. RunCompute `+0x857` remains literal zero/fail-closed.

Build validation: exact existing 24-individual-warning baseline, strict checkpatch 0/0/0, module SHA-256 `3e4f66140e3c679932d953454505fb7df0a48fe3aa3f525120a21e7d620ef42e`, vermagic `7.1.6-gc6fa9b794ba9 SMP preempt mod_unload aarch64`.

No module was installed and no RunCompute was issued.
