# G15 root shared objects and FWBRN checkpoint

## Wrapper +0x008: FWBRN table is absent on G15

The paired AGXG15G host binary resolves the relevant accelerator virtuals as:

- `populateBootTimeFWBRNTable`: returns 0;
- `populateFWBRNTable`: BTI + RET (no-op);
- `getSizeOfFWBRNTable`: returns 0.

The firmware allocation descriptor using host/GPU pair `+0x240/+0x280` takes its size from `getSizeOfFWBRNTable()`. Therefore the G15 FWBRN backing allocation is exactly zero-sized and wrapper `+0x008` is null. The raw field is named `g15_fwbrn_table` and compile-time asserted at `+0x008`.

## Root q22 / q23

Apple's top-level 24-qword InitData maps:

- q22 from allocation pair `+0x620/+0x630`, exact size `0xc3d0`;
- q23 from allocation pair `+0x628/+0x638`, exact size `0x238`.

Firmware stores q22 as its large host/FW shared state pointer and q23 as a compact shared state pointer. The kernel compile-only model now uses exact-size typed `G15Q22Shared` and `G15Q23Shared` objects rather than byte arrays.

Direct q22 users establish fields at `+0x4030`, `+0x404c`, `+0x4054`, `+0x4568`, `+0x4570`, `+0x4580`, `+0x4590`, `+0x45a0`, `+0x45b0`, `+0x45c0`, `+0x45c4`, `+0xc3c8`, and `+0xc3cc`. Apple initializes `+0x45c4 = 1`.

The `+0xc3cc` bit is copied from accelerator config bit 28. Base `AGXAccelerator::configureDevice()` explicitly clears bit 28 (`& 0xffffffffefffffff`) before the smart-idle query, and the later G15/G15G feature ORs do not restore bit 28. Therefore the J615 initial `+0xc3cc` value is exactly zero. An earlier note incorrectly resolved the exact J615 vtable `+0xbd0` to the generic G15 smart-idle getter returning 0. The exact AGXG15G personality table instead has three `gpu,t8122` personalities, all G15G-family classes, and all three `+0xbd0` implementations return 1. That distinction affects accelerator bit 1 / q4 +0x030, not bit 28 / q22 +0xc3cc.

## q22 firmware-control / mapping ring

Apple maps q22 `+0x4568/+0x4570` from an exact `0x20` state allocation and `0x1800` ring allocation. Firmware proves:

- state `+0x00`: read index;
- state `+0x10`: write index;
- index arithmetic modulo 256;
- ring entry size `0x18`;
- entry `+0x00`: firmware-visible address;
- `+0x0c`: context ID;
- `+0x10`: page count (u16);
- `+0x12`: mapping/flush flags (u16);
- `+0x14`: reserved u32, exactly zero for normal host mapping notifications.

Thus `0x100 * 0x18 = 0x1800` exactly. The compile-only Linux model now owns these exact shared allocations and points q22 `+0x4568/+0x4570` at them.

No G15 runtime dispatch or firmware start is enabled by this checkpoint.

## q22 is the G15 firmware-control successor

The q22 `+0x4568/+0x4570` pair is now structurally identified as the G15 successor to Asahi's legacy `FwStatus.fwctl_channel`, not merely a generic cache-flush ring:

- legacy `FwCtlChannelState` is 0x20 bytes with read index at +0x00 and write index at +0x10;
- G15 q22 state is exactly 0x20 with the same index offsets;
- legacy `FwCtlMsg` is exactly 0x14 bytes: address +0x00, word +0x08, context/slot +0x0c, page count u16 +0x10, flags u16 +0x12;
- G15's firmware walker consumes exactly those same fields at the same offsets in a 0x18-byte entry, adding one new u32 at +0x14;
- 256 G15 entries therefore require the observed exact 0x1800 ring allocation.

This closes the *location/shape* question for the G15 firmware-control/mapping channel. Linux now excludes the legacy 0x14-message FwCtl channel allocation from the generated G15 manager and leaves `fwctl()` fail-closed with `ENODEV`. Although `+0x14` is now proven zero, the G15-native encoder still differs in how it derives the fields at `+0x08/+0x10/+0x12`, so enabling the legacy encoder would still be speculative.

### Exact G15 host entry construction

Direct disassembly of `AGXArmFirmware::insertNewMappingEntry(AGFAMemDescriptorEntry const&)` closes the new tail word. The host ring lives at CPU members `+0x1250` (state) and `+0x1258` (entries). The method uses a 0x18 stride, copies the first 16 bytes with `ldr/str q0`, then copies the full final 8 bytes at entry `+0x10`; write index is published at state `+0x10` after a DMB.

`AGXArmFirmware::notifyNewMapping(u64,u64,bool)` constructs the 0x18-byte stack entry after first zeroing its final qword. It subsequently writes only the u16 values at `+0x10/+0x12`, leaving the new `+0x14` dword exactly zero. Therefore G15 `+0x14` is a proven reserved-zero field for normal mapping notifications.

The same analysis also shows why directly reusing the legacy Linux `FwCtlMsg` encoder would still be unsafe: G15 constructs the old-looking `+0x08/+0x10/+0x12` positions differently, even though their boundaries are preserved. The kernel consequently continues to return `ENODEV` for G15 `fwctl()` until a native mapping/flush request encoder is reconstructed rather than guessing legacy semantics.

### Exact G15 mapping descriptor semantics

A dedicated reproducible extractor now lives at `tools/g15_mapping_ring_recon.py`; its output is `g15-mapping-ring-recon.out`.

The active G15 `halNewFirmware()` references `AGXArmFirmwareChinookV9::gMetaClass` at `0xfffffe000be02010`, so J615 uses the ChinookV9 firmware object. ChinookV9 vtable slot `+0x348` resolves to `AGXArmFirmwareChinookCommon::getFWPageShift()`, which returns **14**. The same vtable proves `convertGPUVAToFWVA()` is identity and places mapping/unmapping notifications at slots `+0x2e0/+0x2e8`.

`AGXAccelerator::start()` overwrites the AGX GART globals from the kernel's `_page_mask` and `_page_shift`; the chained-fixup sources resolve exactly to those two kernel symbols. Therefore descriptor `+0x10` is not an arbitrary legacy page count: Apple computes

`1 << (_agxk_gart_page_shift - getFWPageShift())`.

On the M3/ChinookV9 16 KiB path both shifts are 14, hence this field is **1**.

The normal `AGXMemoryMap` page-walker callbacks establish the rest of the descriptor exactly. A cross-check through `AGXLegacyMemoryMap::__walkMappingPagesUpdatePTE()` and `AGXLegacyGart::updatePageTableEntry(u64,u64,u8,bool)` proves the callback pair is **GPU virtual address + physical address**, not address + byte length: the first value is decoded through the GART virtual-page index masks, while the second is masked/shifted into the physical-address bits of the PTE. The same pair is passed directly to `notifyNewMapping()`.


- `+0x00 u64`: GPU VA; G15 conversion to FW VA is identity.
- `+0x08 u32`: physical address shifted right by 12 (4 KiB physical page number).
- `+0x0c u32`: consumed as a context ID by the firmware secure-flush path; normal mapping emits `0`, normal unmapping emits `0xffffffff`.
- `+0x10 u16`: FW-page count/ratio described above; exactly `1` on J615.
- `+0x12 u16`: bitfield. Bit 0 means mapping, bit 1 marks the special 64 MiB aperture, bit 2 carries the map-property bit from `AGXMemoryMap +0x58 bit 3`.
- `+0x14 u32`: exact reserved zero for both map and unmap constructors.

The special-aperture test is exact: `(gpu_va >> 26) == 0x3fffff0803`, i.e. `0xfffffc200c000000..0xfffffc200fffffff`, and is only active when `AGXArmFirmware +0x201 == 0`. Mapping flags are `(special ? 3 : 1) | (property ? 4 : 0)`; unmapping flags are `special ? 2 : 0`.

This reveals an architectural difference from the old Asahi `FwCtlMsg`: G15's q22 ring is populated by **per-page mapping/unmapping notifications** generated by the memory-map walker. It should not be enabled merely by widening the old one-shot range-flush message to 0x18 bytes. The current Linux G15 path therefore remains intentionally fail-closed while the correct notification integration point is reconstructed.
### Walker granularity and eGartRange scope

The full `AGXMemoryMap::__walkMappingPages()` disassembly closes the granularity question: J615 emits one callback per 16-KiB GART page, including the repeated-physical-page padding path. `AGXAccelerator::start()` resolves kernel `_page_mask`, `_page_shift`, and `_page_size`, explicitly requires page shift 14, and copies all three into AGX GART globals. ChinookV9 FW page shift is also 14, so q22 entry `+0x10` is exactly 1.

Apple's selector is `eGartRange`, not the page-table protection object. The exact 13-entry range table and `isStandardFWMapping()` mask prove standard firmware ranges `{7,8,10,11}`; range 8 is the special 64-MiB aperture. q1 and q22's control-ring aggregate are range 7, `AGXArmFirmwareMapper::iovmMapMemory()` uses range 8, and HwDataB `+0x28` stores the exact range-11 base. See `G15-MAPPING-NOTIFY.md`. This is why a blanket hook in generic Linux `UatPageTable::map_pages()/unmap_pages()` would be architecturally wrong.
