# G15 InitData +0x18 wrapper reconstruction

Updated: 2026-08-21T10:04:12+01:00

Scope: exact Apple AGXG15G host driver paired with RTKit-2419.140.12.release and the firmware captured on this T8122 machine. This is an offline ABI reconstruction; no GPU firmware was started by these steps.

## Proven top-level binding

- Top-level InitData signature: `0x0c08e21e83800490`.
- InitData `+0x18` is the GPU VA of a new G15 wrapper allocation.
- Apple host object members for this one allocation:
  - CPU VA: `AGXFirmware + 0x518` (decompiler index `param_1[0xa3]`).
  - GPU VA: `AGXFirmware + 0x538` (`param_1[0xa7]`).
  - CPU alias `wrapper + 0x18`: host `+0x520` (`param_1[0xa4]`).
  - CPU alias `wrapper + 0x1b8`: host `+0x528` (`param_1[0xa5]`).
- Exact wrapper allocation size: **0x490 bytes**.

These aliases are constructed immediately after `allocateSharedData()`, proving that `+0x18` and `+0x1b8` are offsets inside the same wrapper, not separate allocations.

## Strongest layout result

`AGXArmFirmware::initFirmwareSharedData` writes:

- `wrapper + 0x000` = GPU pointer to host member `+0x250`.
- The allocation table proves `+0x210/+0x250` is one allocation of size `0x1860`.
- `AGXArmFirmware::initFirmwareData` uses the CPU side at host `+0x210` as the HwDataB object (including the G15 mapping array and the confirmed SRAM pointer at HwDataB `+0xa20`).

Therefore **wrapper +0x000 is exactly the GPU pointer to HwDataB**.

## Embedded old-style channel/pointer aggregate

Base-class `AGXFirmware::initFirmwareData` writes through the host alias `wrapper + 0x18`.

For four iterations it fills a 0x60-byte block per pipe. Each block contains 12 GPU-pointer qwords grouped as three groups of four. The source objects advance by 0x28 per pipe. This is a G15-expanded form of the familiar per-pipe channel/pointer aggregate, not a replacement of the whole concept.

After those four pipe blocks, the same base routine writes additional mapped pointers at inner offsets `0x180, 0x188, 0x190, 0x198`.

The second alias, `wrapper + 0x1b8`, is exactly inner offset `0x1a0`. Base code fills eight more GPU pointers there. This is direct proof that old RegionB/RuntimePointers-style pointer content survives inside the new compact G15 wrapper.

## Known wrapper fields

All offsets below are relative to wrapper base.

| Offset | Width | Proven host initialization / meaning |
|---:|---:|---|
| 0x000 | 8 | GPU pointer to HwDataB (exact) |
| 0x008 | 8 | mapped host member +0x280 (semantic name unresolved) |
| 0x010 | 8 | mapped host member +0x288 (semantic name unresolved) |
| 0x018..0x197 | 0x180 | four 0x60-byte per-pipe pointer blocks |
| 0x198 | 8 | first post-pipe mapped pointer (source is a method on host descriptor +0x558) |
| 0x1a0 | 8 | second post-pipe mapped pointer (same descriptor family) |
| 0x1a8 | 8 | third post-pipe mapped pointer (same descriptor family) |
| 0x1b0 | 8 | mapped host member +0x578 |
| 0x1b8 | 8 | mapped host member +0x5c0; backing allocation size 0x30 |
| 0x1c0 | 8 | mapped host member +0x5e0; backing allocation size 0x3800 |
| 0x1c8 | 8 | mapped host member +0x5c8; backing allocation size 0x120 |
| 0x1d0 | 8 | mapped host member +0x5e8; backing allocation size 0x15000 |
| 0x1d8 | 8 | mapped host member +0x5d0; backing allocation size 0x30 |
| 0x1e0 | 8 | mapped host member +0x5f0; backing allocation size 0x7000 |
| 0x1e8 | 8 | mapped host member +0x5d8; backing allocation size 0x30 |
| 0x1f0 | 8 | mapped host member +0x5f8; backing allocation size 0x4000 |
| 0x1f8 | 8 | mapped host member +0x540; backing allocation size 0x51000 |
| 0x230 | 4 | initialized to zero |
| 0x234 | 8 | mapped host member +0x258 |
| 0x23c | 8 | mapped host member +0x260 |
| 0x244 | 8 | mapped host member +0x268 |
| 0x24c | 8 | mapped host member +0x278 |
| 0x2a8 | 8 | optional mapped accelerator pointer from accelerator +0x2bc0 |
| 0x2b0 | 8 | PBDescriptorTable address returned by `getPBDescriptorTableGPUAddress()` |
| 0x2b8 | 8 | translated firmware-visible PBDescriptorTable address; cached by firmware at global `0x...10f928` |
| 0x2c0 | 8 | UMAPagePoolDescriptorTable address returned by `getUMAPagePoolDescriptorTableGPUAddress()` |
| 0x2c8 | 8 | translated firmware-visible UMAPagePoolDescriptorTable address; cached by firmware at global `0x...10f9a0` |
| 0x2d0 | 4 | copied from accelerator +0x1db0 |
| 0x2d4 | 4 | copied from accelerator +0x1e0c |
| 0x3b0 | 1 | initialized to 0xff |
| 0x3b1..0x440 | 0x90 | initialized to zero by packed qword stores |
| 0x441 | 8 | mapped host member +0x270 |
| 0x449 | 8 | copied from accelerator +0x1d80 |
| 0x451 | 8 | copied from accelerator +0x1d88 |
| 0x459..0x48f | 0x37 | unresolved tail |

Notes:
- G15 uses deliberately unaligned 64-bit fields (for example +0x234, +0x441, +0x449, +0x451), so Linux must use its packed/unaligned pointer/U64 wrappers rather than normal aligned Rust u64 fields.
- The wrapper is only 0x490 bytes, far smaller than the old large RuntimePointers/RegionB object. Large backing/scratch allocations are now pointed to from the wrapper instead of living inline.

## HwDataB delta now mechanically explained

Public V13.5 HwDataB has 25 x 0x20-byte I/O mappings. G15 has exactly 31 x 0x20-byte mappings. The six added descriptors add **0xc0 bytes**, moving the immediately-following SRAM pointer from V13.5 offset 0x960 to the Apple-confirmed G15 offset **0xa20**. No extra pre-array redesign is required to explain this shift.

Two G15 mappings are explicitly named by Apple's host driver:

- PBDescriptorTable: accelerator mapping object `+0x24c8`, GPU address field `+0x24d0`.
- UMAPagePoolDescriptorTable: accelerator mapping object `+0xafc8`, GPU address field `+0xafd0`.

The exact G15 accelerator vtable also resolves slot `+0xdd0` to `getPBDescriptorTableGPUAddress()` and slot `+0xdd8` to `getUMAPagePoolDescriptorTableGPUAddress()`. Those are the two calls used to populate wrapper `+0x2b0/+0x2b8` and `+0x2c0/+0x2c8`, respectively. Firmware then caches the translated values and actively uses them at runtime.

## Firmware version / generation evidence

- Live chosen property: `asahi,os-fw-version = 14.7`.
- Local m1n1 firmware table defines V14_7 as compat triple `{14, 7, 0}`.
- Captured GPU firmware identity: `RTKit-2419.140.12.release`.
- Therefore the compile-only Asahi ABI tag for this machine is **G15 / V14_7**; runtime T8122 dispatch remains intentionally disabled until the wrapper is encoded and validated offline.

## Safety boundary

Production remains untouched. No GPU ASC start, no additional GPU MMIO, no experimental module load, and no reboot are part of this reconstruction. The last powered GPU operation remains the previously validated one-register identity read (10 cores / 1 cluster).

## FWLog sequence state at +0x200/+0x218/+0x230

The paired G15 firmware function at `FUN_fffffc000003da5c` identifies the two six-dword arrays immediately after the FWLog payload pointer:

- `+0x200..+0x217`: six per-FWLog-channel payload-slot sequence counters. On a successful log-ring enqueue, firmware increments the selected channel counter, stores its old low 8 bits in the 0x38-byte FWLog ring record, and uses that value as the 0..255 payload-buffer slot.
- `+0x218..+0x22f`: six per-channel message sequence counters. Firmware increments these for every attempted message, including the ring-full/drop path; on a successful record the old value is written into the payload. Sequence gaps therefore expose dropped firmware-log messages.
- `+0x230`: FWLog enable gate. `AGXArmFirmware::initFirmwareSharedData()` explicitly clears it before firmware start; the logging path exits immediately while it is zero.

`AGXFirmware::allocateSharedData()` zeroes the complete mapped allocation before publishing subobject CPU/GPU pointers. Thus both sequence arrays have exact all-zero bootstrap state even though the host does not individually initialize their dwords.

## Wrapper tail zero bootstrap

The same zero-fill proves `+0x459..+0x48f` starts entirely zero. `AGXArmFirmware::init()` then explicitly re-clears four unaligned qwords at `+0x46d/+0x475/+0x47d/+0x485`. No host write targets the final `+0x48d..+0x48f` bytes. Their runtime semantics remain unknown, but their bootstrap value is exact zero rather than an opaque placeholder.
