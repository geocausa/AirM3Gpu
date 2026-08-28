# G15 23J220 range-8 Page-Pool-State leaf PTE

E075 closes the UAT leaf class for the 0x70-byte `UMA Page Pool State` exported at RunCompute `+0x83e`. This checkpoint remains below InitData/RTKit and does not issue RunCompute or any other GPU work packet.

## Two distinct range-7 mapping classes

The earlier apparent conflict with the live-proven range-7 leaf was a mapping-class distinction, not an encoding error. Exact 23J220 reconstruction shows:

- ParameterManagement/q22/PM resources use the `IOGPUResource` path, compact UAT option `0x007`, and leaf `0x00c0000000000447`.
- FList FW-Uncached State uses a distinct SecureMemoryMap path and compact UAT option `0x00b`.
- FList Page-Pool State uses the same SecureMemoryMap family in eGartRange 8 and compact UAT option `0x003`.

The PM resource class therefore remains unchanged.

## Exact range-8 class

`AGXUnifiedAddressTranslator::encodePTEFlags(bank=1, option=0x003)` gives:

`0x00c0000000000443`

This is the cached firmware-RW range-8 class: AP=1, cached memory, UXN=1, GPU-access high bit set, PXN=0, plus AF/type. Linux models it as `PROT_G15_RANGE8_FW`.

The exact range-8 aperture is:

`0xfffffc200c000000..0xfffffc2010000000`

## One-shot Linux proof

Linux commit `e9f50fcc17d58244740360e484ae9904c0cd8d6c` extends the bounded shared-bank1 preflight to two range-8 child tables and one temporary leaf. On the exact J615 the live result was:

`MMU: G15 E075 range-8 leaf PTE PASS (VA 0xfffffc200c000000, bits 0x00c0000000000443, clean leaf)`

All eight Linux-owned range-7/range-8 parents then detached cleanly. The driver stopped ASC and returned `ENODEV` before persistent InitData/RTKit/DRM. The machine returned to Golden and the sacrificial module/initrd were restored byte-for-byte.

## Boundary

E075 authorizes the exact range-8 page-table class on this J615. It does not yet allocate a persistent Page-Pool-State object or populate RunCompute `+0x83e`; the normal shared-bank allocator remains range-7-only in this checkpoint.

Next is a dedicated range-8 allocator, followed by the real FList Page-Pool-State/FW-Uncached-State and 256-entry HardwareBuffer-ID prepare/complete lifetime. HWMetrics, SKU execution-stream ownership, and completion/recovery remain later RunCompute blockers.
