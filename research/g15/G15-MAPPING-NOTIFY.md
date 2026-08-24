# G15 firmware mapping notification ABI

This note records only behavior mechanically established from the paired AGXG15G host binary and RTKit-2419.140.12 firmware. It is intentionally not a runtime enablement plan.

## q22 ring shape

Root q22 (`0xc3d0` bytes) owns the firmware mapping/control channel through pointers at `+0x4568/+0x4570`:

- state allocation: `0x20` bytes;
- entry ring: `0x1800` bytes;
- 256 entries, exact stride `0x18`;
- state read index at `+0x00`, write index at `+0x10`.

`AGXArmFirmware::insertNewMappingEntry()` copies all `0x18` bytes, executes a DMB, and only then publishes the new write index. Firmware consumes the ring modulo 256.

## Exact descriptor

`AGXArmFirmware::notifyNewMapping()` / `notifyNewUnmapping()` plus the firmware walker establish:

| Offset | Width | Meaning |
|---|---:|---|
| `+0x00` | 8 | FW-visible GPU VA. G15 `convertGPUVAToFWVA()` is identity. |
| `+0x08` | 4 | physical address `>> 12` (4-KiB physical-page number). |
| `+0x0c` | 4 | secure context/op word. Normal map emits `0`; normal unmap emits `0xffffffff`. |
| `+0x10` | 2 | `1 << (_agxk_gart_page_shift - getFWPageShift())`. |
| `+0x12` | 2 | mapping flags. |
| `+0x14` | 4 | reserved, exactly zero in both normal constructors. |

Flags at `+0x12` are:

- bit 0: map;
- bit 1: special 64-MiB aperture;
- bit 2: map property propagated from `AGXMemoryMap +0x58 bit 3`.

Map encoding is `(special ? 3 : 1) | (property ? 4 : 0)`. Unmap encoding is `special ? 2 : 0`.

The special aperture test is `(gpu_va >> 26) == 0x3fffff0803`, exactly `0xfffffc200c000000..0xfffffc200fffffff`.

## J615 page granularity

`AGXAccelerator::start()` resolves and copies the kernel `_page_mask`, `_page_shift`, and `_page_size` into AGX GART globals. The active path explicitly requires page shift `0xe`, i.e. 16 KiB. ChinookV9 `getFWPageShift()` also returns 14, so descriptor `+0x10` is exactly `1` on J615.

Direct disassembly of `AGXMemoryMap::__walkMappingPages(...)` proves callback granularity:

- the normal loop emits one callback per GART page;
- after each callback, the physical address advances by `_agxk_gart_page_size`;
- the padding/repeated-page path also emits one callback per GPU page, while reusing one physical backing page.

Therefore a normal J615 q22 record describes one 16-KiB GPU/FW page, while its physical page number is still encoded in 4-KiB units at `+0x08`.

## eGartRange, not Linux Prot

Apple's mapping class is the `eGartRange` enum. The static host range table contains **13** entries (`0..12`), each `0x20` bytes. `AGXGart::returnGartRange()` independently confirms all 13 address intervals.

| Range | Base | Size | table `+0x10` | Standard FW? |
|---:|---:|---:|---:|---|
| 0 | `0` | `0` | `0` | no |
| 1 | `0x0000001000000000` | `0x5c00000000` | `0` | no |
| 2 | `0x0000006e00000000` | `0x100000000` | `0` | no |
| 3 | `0x0000006f00000000` | `0xffc00000` | `0` | no |
| 4 | `0x0000006fffc00000` | `0x400000` | `0` | no |
| 5 | `0x0000010000000000` | `0x20000000000` | `0` | no |
| 6 | `0xfffffc1fffe00000` | `0x200000` | `0x10` | no |
| 7 | `0xfffffc2000000000` | `0x0c000000` | `0x14` | **yes** |
| 8 | `0xfffffc200c000000` | `0x04000000` | `0x14` | **yes; special aperture** |
| 9 | `0xfffffc2010000000` | `0x01400000` | `0x18` | no |
| 10 | `0xfffffc2011400000` | `0x00400000` | `0` | **yes** |
| 11 | `0xfffffc2011800000` | `0x04000000` | `0x14` | **yes** |
| 12 | `0xfffffc2015800000` | `0x04000000` | `0x14` | no |

`AGXMemoryMap::isStandardFWMapping()` is exact: it accepts only type `< 12` with bit set in mask `0xd80`, giving `{7, 8, 10, 11}`. Range 12 exists but is deliberately excluded from this set.

Additional ownership proofs:

- `AGXGart::getCodeGartRangeInfo()` returns range 1's exact base/size (`0x1000000000`, `0x5c00000000`), so range 1 is the code GART aperture.
- q1/AGFA init-sequence backing is mapped with `createFWGPUMapping(..., eGartRange=7)`.
- q22's mapping-ring/state shared allocation uses parent shared-data descriptor mapping type `7`.
- `AGXArmFirmwareMapper::iovmMapMemory()` calls `createFWGPUMapping(..., eGartRange=8)`, directly tying range 8 to the firmware mapper/IOVM aperture.
- `AGXUMAFList::init()` has concrete allocations in ranges 8 and 7 (as well as non-standard range 5).
- `AGXFirmware::initFirmwareData()` writes `convertGPUVAToFWVA(0xfffffc2011800000)` to HwDataB `+0x28`; ChinookV9 conversion is identity. In the generated G15 layout `timestamp_area_base` is exactly the member at `+0x28`, so its G15 initializer is now the exact range-11 base.

Ranges 10 and 11 must not be given stronger semantic names until their allocation users are reconstructed. The exact address/standard-FW status is proven; the higher-level purpose is not yet fully named.

## Notification scope and ordering

Notifications are not a generic consequence of every PTE write.

For `AGXLegacyMemoryMap`, constructor logic enables the alternate notification path only when the G15 capability flag is present, the map type is one of `{7,8,10,11}`, and the relevant option gate allows it. Commit/release then use `notifyFirmwareOfMapping()` / `notifyFirmwareOfUnmapping()` on that path; ordinary mappings use the legacy direct PTE walker without q22.

Secure mappings similarly perform their secure page-table operation and then notify firmware for the standard-FW class on successful paths.

This rules out adding q22 notification to generic Linux `UatPageTable::map_pages()` / `unmap_pages()`. Linux `Prot` currently describes PTE access/memory attributes only and does not preserve Apple `eGartRange` identity. A future G15 implementation needs an explicit range/ownership model before the q22 channel can be safely enabled.

## Reproducible artifacts

- `tools/g15_mapping_ring_recon.py`
- `g15-mapping-ring-recon.out`
- `g15-walker-full-disasm.out`
- `g15-egart-call-sites.out`
- `g15-egart-all-direct-calls.out`

No runtime G15 dispatch, firmware start, module load, or live MMIO is justified by this note.

## Firmware resource-stack range split

The six embedded `AGXFirmwareResourceStack` instances now give a direct semantic split between standard-FW ranges 7 and 8. Their template `init(...)` method stores the `eGartRange` argument at stack `+0x40`; Apple initializes:

- firmware `+0xb60` / selector `+0xba0`, `AGFITimeStampQueue`: range **7**, element `0x18`;
- firmware `+0xe80` / selector `+0xec0`, `uint64_t` late-eval stack: range **7**, element `0x8`;
- firmware `+0xc28` / selector `+0xc68`, `UncachedFWChannelMemory`: range **7**;
- firmware `+0xa98` / selector `+0xad8`, `AGFIChannelState`: range **8**, element `0x24c0`;
- firmware `+0xcf0` / selector `+0xd30`, `CachedFWChannelMemory`: range **8**;
- firmware `+0xdb8` / selector `+0xdf8`, `AGFICmdQueueSchedState`: range **8**, element `0x40`.

The cached/uncached class strings are direct Apple host evidence: range 7 is the uncached FW resource class and range 8 is the cached FW resource class. The cached/uncached channel stacks use the same dynamic element size, exactly `0x60 | ((fw_queue_count & 0x0fffffff) << 7)`.

This also corrects a tempting but false inference: the timestamp **queue** stack uses range 7. HwDataB `+0x28` independently stores the range-11 base in the inherited `timestamp_area_base` member; the two objects are not the same allocation class. Reproduction: `tools/g15_resource_stack_ranges.py` / `g15-resource-stack-ranges.out`.
