# G15 Compute real-dispatch producer transfer — exact 23J220

E256 narrows the first-real-Compute failure after the exact 23J220 direct-launch control correction. This is static same-build reconstruction only: no Linux source mutation and no GPU command are authorized by this note.

## Target provenance

The producer/consumer chain uses the matching macOS 14.8.3 / 23J220 components already retained by the project:

- `AGXMetalG15G_C0`, source version `282.14.2`, SHA-256 `d262a98d865cde1d9af8df0ed318bd5109efcfbc940968d8ef9fa8402565ae88`;
- `AGXG15G.kext`, CFBundleVersion `282.14.2`, SHA-256 `e29327fd1eeec53ea47bba91572d393cd6bc38ab77b9ac3c9ab62cc70f94854b`.

No Apple binary, kernelcache, disassembly, or decompiler output is distributed here. Only independently reconstructed transfers are recorded.

## Linux phase-2 does not synthesize real-dispatch payload state

The selected Linux G15 phase-2 finalizer repairs the command-facing ownership fields that cannot exist in the phase-0 image:

- RunCompute `+0x14` event-control FWVA;
- `+0x760/+0x768` SKU stream FWVA/size;
- `+0x83e` Page-Pool-State FWVA;
- `+0x857` HWMetrics FWVA.

It deliberately requires stock-empty `prepared=1`, `UMA min=0`, and `UMA ideal=0`, and does not rewrite the RegisterArray or JobParameters2 workload state. Therefore a real CDM grafted onto the stock-empty image remains a real producer mismatch even after phase-2 ownership is correct.

## Exact userspace producer chain

`ComputeContext::setPipelineCommon()` folds the bound ComputePipeline requirements into context state:

- context `+0x654` from pipeline `+0xf0`;
- `+0x658` from pipeline `+0xf4`, aligned by pipeline/context `+0xfc/+0x660`;
- `+0x65c` from pipeline `+0xf8`;
- `+0x660` from pipeline `+0xfc`;
- `+0x668` from pipeline byte `+0x104`;
- `+0x664` is derived from the combined resource requirement.

The pipeline quartet in turn comes from ComputeProgramVariant `+0x244/+0x248/+0x24c/+0x250`, which is populated from compiler reply metadata. These values are workload/compiler dependent and are not fixed G15 constants.

On an untouched empty encoder, context `+0x658/+0x65c` remain zero and `endComputePass()` skips the late non-empty synthesis. With real pipeline state, `endComputePass()` can populate raw Compute `+0x138..+0x168` and `+0x178..+0x1b4`.

## Raw Compute to command transfer

Exact 23J220 `AGXComputeHardwareKernelCommand::parseAndValidate()` transfers the non-empty resource block as follows:

| Raw Compute | Parsed wrapper | Descriptor after `processComputeSetup()` |
| --- | --- | --- |
| `+0x138` | `+0x98` | `+0x640` |
| `+0x140` | `+0xa0` | `+0x648` |
| `+0x148` | `+0xa8` | `+0x650` |
| `+0x150` | `+0xb0` | `+0x658` |
| `+0x158/+0x15c` | `+0xb8` | `+0x660` |
| `+0x160/+0x164` | `+0xc0` | `+0x668` |
| `+0x168` | `+0xc8` | `+0x670` |

`processComputeSetup()` additionally zero-extends raw u32 values `+0x158/+0x15c/+0x160` into descriptor `+0x460/+0x468/+0x470`.

The later raw block is copied directly:

| Raw Compute | Descriptor |
| --- | --- |
| `+0x178` | `+0x430` |
| `+0x180` | `+0x438` |
| `+0x188` | `+0x440` |
| `+0x190` | `+0x448` |
| `+0x198` | `+0x450` |
| `+0x1a0` | `+0x458` |
| `+0x1a8` | `+0x478` |
| `+0x1b0/+0x1b4` | `+0x480/+0x484` |

This proves the late producer state is not host-only bookkeeping.

## Firmware-facing consequences

Combining the exact transfer above with the exact 23J220 G15 RegisterArray producer gives these direct hardware-facing edges:

| Raw Compute source | Final consumer |
| --- | --- |
| `+0x138` | UMA minimum request -> descriptor `+0x628` -> RunCompute `+0x847` |
| `+0x140` | UMA ideal request -> descriptor `+0x630` -> RunCompute `+0x84f` |
| `+0x158/+0x15c` | register `0x101d0`; `+0x158` also controls optional leading `0x17e1` |
| `+0x160` | register `0x0d478` |
| `+0x178` | register `0x1a088` |
| `+0x180` | register `0x1a090` |
| `+0x188` | register `0x1a058` |
| `+0x190` | register `0x1a060` |
| `+0x198` | register `0x1a0b8` |
| `+0x1a0` | register `0x1a0c0` |
| `+0x1b0` | contributes to dynamic `0x107a0` when descriptor `+0x460` is nonzero |
| `+0x1b4` | register `0x1a0e8` through descriptor dword `+0x484` |
| byte `+0x1bb` | selects the J615 `0x1a458` synthesized value |
| bytes `+0x158/+0x1b9/+0x1bf/+0x1bd` | G15 JobMeta `engine_state` bytes |

Raw `+0x98/+0xa0`, although populated by a real enqueue, terminate at descriptor `+0x420/+0x428` and have no corresponding exact G15 submit/RegisterArray consumer. They are not a candidate execution-state fix.

## Cross-build discriminator retained, not promoted

The older stock-macOS E068 oracle captured both an empty and one-thread 0x1d0 raw Compute payload. Their SHA-256 values are:

- empty: `daba7c272a8bd1009fc04d0128e546892ee4eee70a5859904b1e4ac38b803611`;
- one-thread: `457f415f84077982ffb3e2dbb5e252e87fa31b52b400c3c16ccabeaf55e5fd32`.

Only the hashes and summarized empty values remain in the Linux-side archive; the one-thread raw bytes are not present there. The capture is also from a newer userspace build, so even when recovered it is a discriminator, not authority for 23J220 constants.

## Current boundary

The exact failure boundary is now **producer state**, not command transport, code-page write permission, or the real-enqueue `+0x98/+0xa0` bookkeeping pair.

A defensible next Linux mutation requires one of two equivalent closures:

1. recover the one-thread oracle and use it only to identify which raw fields change, then prove each candidate through the matching 23J220 producer; or
2. recover the matching minimal pipeline's ComputeProgramVariant `+0x244/+0x248/+0x24c/+0x250` compiler requirements and mechanically run the exact `setPipelineCommon()` / `endComputePass()` synthesis.

Until then, copying provisional UMA sizes or unrelated m1n1 constants would move away from the target authority. The guarded Linux real-dispatch path remains unchanged.
