# G15 pre-MSG_INIT startup-read audit

Research state: 2026-08-25.

The last live checkpoint starts RTKit application endpoints EP20 and EP21, then
tears the manager/ASC down without sending InitData. The next firmware-visible
step is intentionally blocked while the first G15 `MSG_INIT` read-set is closed.

## Why `MSG_INIT` is still blocked

The exact G15 firmware first-init consumer copies the 24-qword InitData root,
follows q3 to the runtime wrapper/HwDataB, and immediately reads fields that are
not correct in the current promoted source.

### Mandatory RGX firmware register base

Compiler-authoritative layout reconstruction gives:

- `io_mappings` begins at `HwDataB + 0x640`.
- each record is `0x20` bytes.
- `HwDataB + 0x6a8` is record 3 `virt_addr`.
- record 3 is `RGXRegs`.

Apple's host path fills this field with the firmware-visible address of the RGX
MMIO mapping. Firmware imports it immediately during first init. The promoted
T8122 configuration still has an empty I/O-mapping list, so this value would be
zero today. Sending `MSG_INIT` in that state is therefore unsafe.

### J615 chip-info words

Apple's exact J615/G15G block at `HwDataB + 0xa28` is:

- `+0xa28 = 0x8122` — chip ID
- `+0xa2c = 2` — revision high component
- `+0xa30 = 0` — revision low component
- `+0xa34 = 4` — process node

The current promoted Linux builder does not yet reproduce the final two words.
Firmware imports this block during first init.

### G15 tail

The G15 host writer also initializes generation-specific startup words around
`0x17b4..0x1848`. Exact recovered writes include:

- `+0x17fc = 1`
- `+0x1808 = 0x0000000100000001`
- qwords at `+0x1818`, `+0x1820`, `+0x1828`, `+0x1830`, `+0x1838`, `+0x1840`
  initialized to all ones
- `+0x1848 = 0`
- `+0x1858` is a G15 runtime flag consumed by firmware

Inherited older-generation defaults are not accepted as proof for this region.

## J615 firmware I/O-mapping ABI

Apple's HwDataB record format and its accelerator-side producer are now
reconstructed. There are 31 G15 records. Active records carry a read/write flag,
element count, physical source, firmware virtual address and byte size.

Mechanically closed J615 sources include:

| Index | Resource | Physical source | Size / access |
|---:|---|---:|---|
| 0 | FenderRegs | `0x290d00000` | G15 overrides record element size to `0x104000`, RW |
| 1 | AICTimerRegs | `0x20e100000` | `0x4000`, RO |
| 2 | AICSWIntRegs | `0x2d1014000` | `0x4000`, RW |
| 3 | RGXRegs | `0x290000000` | `0x20000`, RW |
| 9 | MetrologySensorRegs | `0x290e08000` | `0x8000`, RW |
| 10 | GMGIFAFRegs | `0x290d0d000` | `0x1000`, RW |
| 29 | GFXCLKGEN_MGPU | `0x290e1c000` | `0x4000`, RO |

AICSW is not inferred from an older SoC: J615's exact Apple DeviceTree
`meta-sw-interrupt` value contains `0x2d1014048`; the Apple host applies its
mapping-alignment mask and produces `0x2d1014000`.

Fender is independently cross-checked by the SGX secondary resource: J415
`reg[1]` is the known T8112 Fender aperture, and J615 `reg[1]` translates to
`0x290d00000`.

## Still unresolved before promotion

A compile-clean 31-slot reconstruction exists in the private bring-up worktree,
but it is deliberately not part of the public tested patch series yet. In
particular, the exact J615 UVWarn/UVD resource-selector decision and less-central
later mapping records must be checked against the J615 personality before the
mapping table is promoted.

The selector is not a trivial constant getter: the exact G15G vtable target
walks a host property object and dispatches a typed getter. Older-SoC constants
must not be substituted for this proof.

## Next gate

Before the first `MSG_INIT` live test:

1. close the remaining J615 mapping records/resource selector;
2. correct the chip-info words;
3. reconstruct the G15 startup tail/scalars consumed by first init;
4. add a build-only byte-level validator for the complete firmware first-read
   image; and
5. exercise that validator while still tearing ASC down and returning ENODEV.

Only after that closure passes should `MSG_INIT` become a separate live
checkpoint.
