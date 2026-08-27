# G15 Compute control-stream boundary

Research state: 2026-08-27

Target: J615 / T8122 G15G C0, Apple 25F84 host driver and RTKit-2419 firmware.

This note follows `G15-COMPUTE-LAUNCH-BOUNDARY.md` and `G15-COMPUTE-SKU-STREAM.md`. E063 uses the macOS host producer as an oracle to close the next layer below the SKU wrapper: the real CDM control-stream pointer and Apple's Gen4 patch/reset framing.

## RegisterArray ownership

`AGXCLChannelG15::generateRegisterList()` constructs the normal G15 Compute RegisterArray at RunCompute `+0x20`. The first workload-facing edge is direct and unconditional on the normal path:

- CDM register `0x1a420` receives the raw Compute control-stream pointer from the Apple command-state field corresponding to producer `+0x358`.

The same RegisterArray carries the remaining fixed and G15G-dynamic CDM state. E062 already closes the two formerly unknown J615 dynamic register IDs as `0x101d8` and `0x107a0`.

This means the E062 SKU stream is not sufficient by itself to form an inert RunCompute. The RegisterArray explicitly directs the hardware at a separate CDM control stream.

## Gen4 patch/reset records

Apple's `ComputeIndirectExecutionContextGen4::patchCDMControlStreamAndReset()` manages continuation/end boundaries for that stream.

A pointer patch is a 16-byte record. Its first dword combines the high address bits with `0x20000000`, followed by the low address dword and the associated 64-bit state value.

When closing cached stream state Apple emits two exact token forms through separate CDM token pools:

- token `0x60000160` through pool/class `0x1b`, then a 16-byte pointer/state patch record;
- token `0x60000960` through pool/class `0x1a`, then a 16-byte pointer/state patch record.

The helper then clears its cached address/state pairs so the next Compute pass starts from a fresh boundary.

`endComputePass()` invokes this helper when its indirect-execution/control-stream state is live, before the ordinary end-of-pass `0x40000000` command and later fence/timestamp/finalization records.

## Per-pass state

`beginComputePass()` allocates and zeroes a `0x268`-byte per-pass state object. The begin/end routines preserve multiple CDM address/state pairs that feed the patch/reset helper, including the groups at context `+0x6f00/+0x6f08`, `+0x6f90/+0x6f98`, and `+0x6fa8/+0x6fb0`.

Thus the value programmed into `0x1a420` is part of a managed CDM token stream, not a free-standing shader pointer or diagnostic field.

## What E063 does not claim

The constants `0x60000160` and `0x60000960` are mechanically proven Apple patch/reset grammar. They are **not** yet classified as safe standalone execution payloads. E063 therefore does not justify a live RunCompute.

## E064 token-family classification

Cross-checking the exact E063 constants against Mesa's independently reconstructed CDM block grammar closes their block types:

- `0x60000160` is a **Barrier** (low bits 5/6/8).
- `0x60000960` is a **Barrier** (low bits 5/6/8/11).
- `0x20000000` is the **Stream Link** block type before target-address bits are added.
- `0x40000000` is **Stream Terminate**.
- conditional `0xa0000000` is block type 5, a G15/Apple extension not present in the current Mesa grammar and not required by the minimal path.

Apple's exact G15 `endComputePass()` emits `0x40000000` and advances the stream pointer by one dword. Thus the 25F84 producer's terminate token is emitted as four bytes, even though Mesa's generic XML reserves an additional zero dword.

This gives the first plausible no-threadgrid/no-shader root stream: a terminate-only G15 CDM stream. It is still not a live-test payload until an Apple-generated zero-dispatch Compute pass, or equivalent producer path, proves that the normal RunCompute container can carry such a root while the associated UMA/stamp/timestamp/completion objects remain valid.

The remaining boundary is therefore the host/container proof for a terminate-only CDM root, followed by the already-closed UMA, stamp, timestamp, completion and fail-closed recovery prerequisites before the first bounded type-3 experiment.
