# G15 Compute RegisterArray — exact 23J220 target validation

E067 removes the remaining build-version ambiguity around the G15 Compute register program used by the M3/J615 Linux target.

## Exact producer

The matching macOS 14.8.3 / 23J220 KDK exposes the producer in `AGXG15G.kext` rather than in the Metal userspace bundle:

`AGXCLChannelG15::generateRegisterList(AGFIChannelCommandCL *, AGXCLCommandDescriptor *)`

Target provenance:

- `AGXG15G.kext` CFBundleVersion `282.14.2`
- binary SHA-256 `e29327fd1eeec53ea47bba91572d393cd6bc38ab77b9ac3c9ab62cc70f94854b`
- producer VM range `0x81118..0x81b9b`, length `0x0a84`
- extracted-function SHA-256 `495020a4205780e2f916c0e9d1e7f9462b0a237d93cb422dc3f5236f59ebeb09`

No Apple binary or disassembly is distributed here; only independently reconstructed offsets, values and hashes are retained.

## Container geometry

The exact producer confirms:

- RegisterArray begins at RunCompute `+0x20`;
- descriptor backing plus `0x20` is recorded at command `+0x720`;
- count/size state is at command `+0x728`;
- each normal encoded register entry occupies `0x0c` bytes.

The Linux model independently asserts the same 12-byte Register entry and 0x710-byte RegisterArray geometry.

## Exact ordinary sequence

An optional leading `0x17e1 = 1` is emitted only when descriptor `+0x460` is nonzero. The core list then follows this order:

| Register | Target-ABI source/value |
| --- | --- |
| `0x1a510` | descriptor `+0x3b0` |
| `0x1a420` | descriptor `+0x358`, the raw CDM control-stream pointer |
| `0x1a4d0` | descriptor `+0x3c0` |
| `0x1a4d8` | descriptor `+0x3c8` |
| `0x1a4e0` | descriptor `+0x3d0` |
| `0x1a4e8` | descriptor `+0x3d8` |
| `0x1a440` | G15 synthesized CDM configuration; mirrored to command `+0x7c4` |
| `0x1a458` | G15 synthesized configuration value |
| `0x12090` | descriptor dword `+0x5cc & 0x1f` |
| `0x101d8` | same low-five-bit value on J615/G15G |
| `0x1a088` | descriptor `+0x430` |
| `0x1a090` | descriptor `+0x438` |
| `0x1a058` | descriptor `+0x440` |
| `0x1a060` | descriptor `+0x448` |
| `0x1a0b8` | descriptor `+0x450` |
| `0x1a0c0` | descriptor `+0x458` |
| `0x101d0` | `(desc+0x460 & 0xffffffffffe0ffff) | (desc+0x468 << 16)` |
| `0x0d478` | descriptor `+0x470` |
| `0x1a0e8` | descriptor dword `+0x484 & 0xfffffff8` |
| `0x107a0` | `0x00ff0000` when `desc+0x460 == 0`; otherwise `(u32(desc+0x480) << 16) | 0x01000000` |

The J615 dynamic register-ID getters are independently fixed in the same target KDK: `0x101d8` and `0x107a0`.

## J615 synthesis

For the measured J615/G15G configuration, accelerator setup leaves the alternate table-driven `0x1a440` branch inactive. The normal value simplifies to:

`0x154024200 | (raw_compute[0x170] ^ 1)`

Register `0x1a458` is `0x10c08860` or `0x10c08ae0`, selected by raw Compute byte `+0x1bb`. J615 has one MGPU, so the optional `| 0x1c` branch is not taken.

## Optional feature tail

Under a separate performance/feature gate Apple can emit a shared value to `0x1a540`, `0x014a8` and `0x0a350`, with a further optional `0x1a430/0x1a438` pair. This is not part of the established ordinary/default path and is kept separate from base bring-up.

## Cross-build conclusion

The exact 23J220 producer matches the register IDs, descriptor source offsets, synthesized `0x1a440`/`0x1a458` behavior, dynamic J615 IDs and optional-tail structure previously reconstructed from the newer 25F84 oracle. Therefore the G15 hardware-facing RegisterArray program is stable across those two builds and is now verified against the exact firmware generation used by the Linux target.

This closes the **list identity** blocker. It does not close every Linux value producer: several Apple descriptor fields originate in raw/private Compute payload state that the current Linux UAPI does not yet produce. Full G15 RegisterArray emission therefore remains fail-closed until each ordinary-path value is mechanically sourced or independently proven constant/absent.

A stock-Metal zero-dispatch Compute encoder is independently known to complete on M3, but its private raw command fields were not captured. They must not be assumed zero merely because no shader launch occurs.

## Current gate

No live Linux RunCompute is authorized by E067 alone. Next work is a row-by-row producer matrix against Linux state, followed by compile-only population only if the ordinary zero-dispatch path becomes complete. The Golden kernel remains the persistent recovery/default boot.
