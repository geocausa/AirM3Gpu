# G15 stock-empty JobParameters2 / JobMeta defaults — E132

E132 closes the remaining same-build userspace-produced stock-empty command-body values identified by the E112 integration audit. This is exact macOS 14.8 / 23J220 static reconstruction only. No Linux source change, module install, RunCompute, or custom GPU command was made.

## Exact 23J220 userspace oracle

The matching `AGXMetalG15G_C0` was recovered from the arm64e system Cryptex/dyld cache of Apple's exact 23J220 OTA rather than inferred from the retained newer userspace images.

Identity:

- size `9,893,496` bytes;
- SHA-256 `d262a98d865cde1d9af8df0ed318bd5109efcfbc940968d8ef9fa8402565ae88`;
- UUID `7C1E587A-69DF-33A4-AC9F-6FE4EFEF3529`;
- macOS minimum / SDK `14.8 / 14.8`;
- source version `282.14.2`.

The Apple binary is not distributed here. The retained 24G84 image is a different binary (9,583,184 bytes, SHA-256 `4e74cf04659be4d0b16e5a4694ffbde94e53eadb1976b9db33356ba79e9ffaa9`).

## Exact stock-empty raw Compute initialization

`-[AGXG15GFamilyComputeContext initWithCommandBuffer:config:]` calls the exact 23J220 `AGX::ComputeContext<...>::beginComputePass(false, 0x16)` implementation. That function allocates a `0x1f8` command record, records `record + 0x20` as its raw Compute payload, and zeroes the entire `0x1d0` raw payload before explicit field initialization.

Neither initial construction nor the no-dispatch `deferredEndEncoding -> endComputePass(impl, 0, 0x16)` close path writes raw offsets:

`+0x008, +0x010, +0x018, +0x020, +0x028, +0x030, +0x038, +0x040`.

The exact 23J220 kernel source map already proves those raw qwords become the G15 `JobParameters2` command locations below:

| Raw Compute | RunCompute | stock-empty 23J220 |
| --- | --- | ---: |
| `+0x008` | `+0x774` | `0` |
| `+0x010` | `+0x77c` | `0` |
| `+0x018` | `+0x784` | `0` |
| `+0x020` | `+0x78c` | `0` |
| `+0x028` | `+0x7a4` | `0` |
| `+0x030` | `+0x7ac` low | `0` |
| `+0x038` | `+0x7b4` | `0` |
| `+0x040` | `+0x7bc` low | `0` |

The E131 Linux command image already writes zero at all eight locations. E132 therefore needs no Linux patch: those values are now exact same-build target defaults rather than unverified fail-closed placeholders.

## Exact Compute `G15JobMeta.engine_state`

The exact kernel path repacks RunCompute `+0x7e4..+0x7e7` from four raw bytes in this order:

`raw[0x158], raw[0x1b9], raw[0x1bf], raw[0x1bd]`.

The userspace constructor initializes context words `+0x658 = 0`, `+0x65c = 0` and byte `+0x609 = 0`. `beginComputePass()` initially zeroes all four raw bytes. In `endComputePass()`, exact ARM64 reloads `+0x658/+0x65c`, tests their sum and branches directly to the empty finalization tail when both are zero, skipping the non-empty synthesis that can rewrite raw `+0x158..`. The final `raw[0x1bf]` value is copied from context `+0x609`, also zero; `raw[0x1b9]` and `raw[0x1bd]` remain zero.

Thus exact stock-empty 23J220:

`G15JobMeta.engine_state = 0x00000000`.

Linux already emits that value.

## Cross-build guard validated

The exact 23J220 producer unconditionally writes `raw +0x0a4 = 0x1c` during `beginComputePass()`, while the retained newer-userspace empty capture had `raw +0x0a4 = 0`. Raw defaults therefore do change across AGXMetal builds. E112 was correct to refuse promotion of the newer capture's nonzero `raw +0x008` into 23J220; the matching producer proves the target value is zero.

## Remaining boundary at E132, corrected by E133

E132 originally left RunCompute `+0x740/+0x748/+0x750` open under an `AGXCommandQueue +0x20/+0x38` interpretation. E133 replays the exact caller/prologue register chain and corrects that attribution: the snapshot register is the parsed `AGXComputeHardwareKernelCommand` wrapper, with wrapper `+0x20 <- raw Compute +0xc0` and wrapper `+0x38 <- raw +0xd8`. The matching 23J220 userspace producer initializes both raw qwords to zero on the stock-empty path, so E133 closes all three command values without changing their bytes.
