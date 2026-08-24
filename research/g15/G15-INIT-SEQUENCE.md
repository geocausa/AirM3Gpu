# G15 / G15G AGFA init sequence and root-zero proof

## q1 AGFA sequence

The G15 top-level init root q1 points at the GPU mapping paired with `AGXArmFirmware +0x1220/+0x1228`. `AGXArmFirmware::allocFirmwareData()` allocates this buffer through `IOBufferMemoryDescriptor::withOptions(0x13, ...)`. G15 firmware parses q1 as 0x18-byte AGFA records:

- `+0x00`: u64 value
- `+0x08`: u32 register offset
- `+0x0c`: u32 shift
- `+0x10`: u32 record kind
- `+0x14`: u32 reserved

Firmware kinds are 0=end, 1=32-bit register write, 2=64-bit register write, 3=64-bit shifted write. The parser advances exactly 0x18 bytes per record.

Apple host helpers `AGXArmFirmware::setInitReg32`, `setInitReg64`, and `setInitReg64PA` generate this exact record shape and advance the host write offset by 0x18. `prepareFirmwareForBoot()` resets that write offset, calls accelerator vtable slot +0xa58 (`populateInitSequenceFirmware()`), then emits a zero/type-0 terminator.

For both the active G15 and G15G vtables, `populateInitSequenceFirmware()` is exactly `BTI C; RET`. Therefore J615 has an empty AGFA sequence: the first record is a type-0 terminator and the remainder of the page is unused/zero.

`AGXArmFirmwareChinookCommon::getFWPageShift()` returns 14, so the one-page backing size is exactly 0x4000. The compile-only Linux model now represents q1 as a typed 0x4000 page whose first 0x18-byte record is explicitly constructed as the terminator.

## q2 / untouched-root zero proof

Apple's G15 root constructor writes q0, q1, q3..q18, q21..q23 and q5; q2 is never written. q19/q20 are likewise not written and are the final 0x10 bytes of the structural q6..q20 UAT block's zero padding.

The root backing allocation is also created with `IOBufferMemoryDescriptor::withOptions(0x13, ...)`. The exact kernel implementation resolves the virtual init call to `IOBufferMemoryDescriptor::initWithPhysicalMask`. On the 0x13 path it reaches the aligned allocator at `0xfffffe0008c554e0` with flags `0x1004`; that allocator ORs in `0x30000` before its kalloc call, yielding `0x31004`. This is exactly the same allocation flag word used by exported `IOMallocZero()` at `0xfffffe0008c55404`.

Thus the backing memory is zero-initialized before AGX writes the root fields. Since q2/q19/q20 have no host writes, their Apple-visible initial values are exactly zero. Linux's existing zero values for these fields are therefore exact rather than placeholders.

All evidence here is offline from the paired Apple kernelcache/AGXG15G image; no firmware execution or live GPU access is involved.
