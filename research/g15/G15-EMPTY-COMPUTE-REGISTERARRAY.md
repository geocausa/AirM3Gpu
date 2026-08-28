# G15 empty-Compute RegisterArray closure

E068 closes the ordinary J615/G15G RegisterArray for Apple's stock Compute encoder with no dispatch. This is an oracle/static checkpoint only; no Linux `RunCompute` was executed.

## Stock Apple empty-Compute oracle

The stock Metal path was compared against a normal one-thread Compute dispatch on Apple M3. After command-buffer commit, both contain an exact `0x1d0` raw Compute payload accepted by `AGXComputeHardwareKernelCommand::parseAndValidate()`.

The empty raw payload has only four non-zero qwords (`+0x008`, `+0x0b8`, `+0x0c0`, `+0x0c8`). Every raw field consumed by the ordinary G15 Compute RegisterArray is zero, including the CDM stream pointer and the state fields at `+0x058`, `+0x070..+0x088`, `+0x0a4`, `+0x158..+0x160`, `+0x170`, `+0x178..+0x1b0`, and `+0x1bb`.

Oracle raw-payload SHA256 values:

- empty: `daba7c272a8bd1009fc04d0128e546892ee4eee70a5859904b1e4ac38b803611`
- one-thread control: `457f415f84077982ffb3e2dbb5e252e87fa31b52b400c3c16ccabeaf55e5fd32`

## Final descriptor-field closure

E067 left descriptor `+0x460/+0x468/+0x470` as the last unknown inputs to the ordinary list. Exact host-driver control flow now closes their source chain:

`raw Compute +0x158/+0x15c/+0x160`
→ `AGXComputeHardwareKernelCommand +0xb8/+0xbc/+0xc0`
→ `AGXCLCommandDescriptor +0x460/+0x468/+0x470`.

All three values are zero in the stock empty Compute payload. Therefore the conditional leading `0x17e1` entry is absent, `0x101d0 = 0`, `0x0d478 = 0`, and G15G dynamic register `0x107a0 = 0x00ff0000`.

## Exact ordinary list

The empty J615/G15G list is exactly 20 entries, or `0xf0` bytes at the 12-byte RegisterArray stride. Logical form-1 register IDs are encoded with low bit set in the list.

| Logical register | Form | Value |
| --- | ---: | ---: |
| `0x1a510` | 0 | `0` |
| `0x1a420` | 0 | `0` |
| `0x1a4d0` | 0 | `0` |
| `0x1a4d8` | 0 | `0` |
| `0x1a4e0` | 0 | `0` |
| `0x1a4e8` | 0 | `0` |
| `0x1a440` | 0 | `0x154024201` |
| `0x1a458` | 0 | `0x10c08860` |
| `0x12090` | 1 | `0` |
| `0x101d8` | 1 | `0` |
| `0x1a088` | 1 | `0` |
| `0x1a090` | 1 | `0` |
| `0x1a058` | 1 | `0` |
| `0x1a060` | 1 | `0` |
| `0x1a0b8` | 1 | `0` |
| `0x1a0c0` | 1 | `0` |
| `0x101d0` | 1 | `0` |
| `0x0d478` | 1 | `0` |
| `0x1a0e8` | 1 | `0` |
| `0x107a0` | 1 | `0x00ff0000` |

On J615/G15G, `0x1a440` is exactly `0x154024200 | (raw[0x170] ^ 1)`, hence `0x154024201` for the empty command. `0x1a458` is `0x10c08860` because raw `+0x1bb` is zero and J615 has one MGPU.

The optional feature tail is absent on this empty path.

## Linux checkpoint

Linux commit `1d264651a20410af426cb3ee269ede2ec15011dd` models this exact empty list and mirrors `0x1a440` into the G15 command's `+0x7c4` field. Native `asahi.o`/module construction passes at the established 24-warning baseline.

This does **not** enable G15 Compute execution. Ordinary G15 `SUBMIT` remains rejected with `ENODEV`; the signed bring-up path remains the barrier-only QueueInfo registration diagnostic and does not construct `RunCompute`.

The next execution boundary remains the complete harmless no-launch container: SKU/microsequence, UMA Page Pool State, stamp/timestamp/completion state and lifecycle must be closed together before a live Linux `RunCompute` probe is justified.
