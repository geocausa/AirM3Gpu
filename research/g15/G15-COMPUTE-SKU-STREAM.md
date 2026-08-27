# G15 Compute SKU Stream

Research state: 2026-08-27

Target: J615 / T8122 G15G C0, macOS 25F84 host contract and RTKit-2419 firmware.

This note follows `G15-COMPUTE-LAUNCH-BOUNDARY.md`. E061 established that a normal type-3 RunCompute is inherently hardware-facing: firmware installs RunCompute `+0x760` as the engine-2 execution-stream pointer, while Apple's host always builds a G15 register list and SKU stream. E062 closes the SKU stream packet grammar itself.

## Exact stream skeleton

`AGXCLChannelG15::encodeCLCommandSKUStream()` emits the following baseline sequence:

| Order | Record | Exact size |
| --- | --- | ---: |
| 1 | opcode `0x0000000b` + payload | `0x1bc` |
| 2 | start timestamp, opcode `0x80000003` | `0x3c` |
| 3 | Compute WFI dword `0x00000001` | `0x04` |
| 4 | end timestamp, opcode `0x00000003` | `0x3c` |
| 5 | trailing opcode `0x0000000c` record | `0x7c` |
| 6 | finish dword `0x40000002` | `0x04` |

The opcode-`0xb` record contains an exact `0x1b8`-byte payload. Among its mechanically established fields are FW addresses for the RunCompute RegisterArray at command `+0x20` and Compute JobParameters2 at `+0x76c`.

The unrounded baseline is therefore `0x2b8` bytes. `AGXSKUEncoder::finishEncode()` rounds the reported stream size upward to a `0x40` boundary, making the ordinary no-feature stream exactly **`0x2c0` bytes**.

A host feature predicate can insert a `0x90000004` record before opcode `0xb` and a paired `0x10000004` record after WFI. Each is `0x14` bytes. With both present the unrounded stream is `0x2e0`, producing an exact rounded size of **`0x300` bytes**.

## Compute WFI

The complete G15 `AGXCLChannelG15::encodeComputeWFI()` implementation is only 0x20 bytes of code. It:

1. loads the SKU write pointer;
2. advances it by four bytes;
3. stores dword `1` at the old pointer;
4. returns.

Thus G15 Compute WFI is exactly `0x00000001`. There is no hidden command-dependent payload in this method.

## Compute timestamp records

`AGXSKUEncoder::encodeTimeStamp()` reserves exactly `0x3c` bytes per record. For command type 3:

- the start record begins with `0x80000003`;
- the end record begins with `0x00000003`;
- the packet carries FW addresses into RunCompute's timestamp area beginning at `+0x810`;
- start/end selection uses the `+0x818/+0x820` pair;
- optional user-timestamp selection tests command `+0x828/+0x830`;
- another command-relative pointer uses `+0x868`;
- the final dword carries the command type.

This is sufficient to reproduce the timestamp packet geometry without assigning speculative semantics to every qword.

## J615/G15G dynamic Compute register IDs

The two remaining virtual register-ID hooks in the G15 Compute register generator devirtualize to fixed J615/G15G values:

- accelerator vslot `+0x10a8` -> `getAgxCrAnisoConfigCdmOffset()` -> **`0x101d8`**;
- accelerator vslot `+0x1090` -> `getAgxCrUscTpCdmResourceCfg0Offset()` -> **`0x107a0`**.

These are G15G-specific. The A0 subclass differs, so the values must not be generalized across all generation-15 variants.

Together with the existing descriptor-source reconstruction, the mandatory G15G register generator is now mechanically enumerated. Crucially, register `0x1a420` receives raw Compute payload `+0x000`, the CDM execution/control-stream pointer. The register list therefore cannot be made an inert scheduler-only structure merely by reproducing packet framing.

## Parser observation

`AGXComputeHardwareKernelCommand::parseAndValidate()` bounds-checks an exact `0x1d0` bytes and copies selected state into the kernel-side command object. It does not perform value-level validation of the hardware register payload. A malformed or synthetic zero-heavy command therefore cannot be assumed safe because the parser accepted it; safety depends on the later register/SKU/UMA execution contract.

## Current boundary

The SKU grammar is no longer the blocker. Before the first live RunCompute, the remaining question is whether an Apple-style **deliberately inert CDM control stream** can be mechanically proven, including its mandatory register values, UMA Page Pool State, stamps/dependencies, timestamp/completion storage, and retirement behavior.

Until that is closed, RunCompute remains blocked.
