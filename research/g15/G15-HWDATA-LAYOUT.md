# G15 HwDataA / HwDataB layout checkpoints

## HwDataB

Apple allocates G15 HwDataB as exactly `0x1860` bytes. Compile-time offset probing of the inherited V14.7/G15 generated layout showed that it is byte-position-compatible through `unk_b68` at `+0x183c`; the old inherited trailer then consumed `0x104` bytes and produced the incorrect `0x1944` total.

G15 replaces that old trailer with an exact `0x20` bytes at `+0x1840..+0x185f`, giving the exact Apple allocation size. Firmware early init reads `+0x1858`; the Apple host writes the inverse of accelerator-global flag bit 4 there. The kernel model now hard-asserts total size `0x1860`, SRAM pointer `+0xa20`, `unk_b68 +0x183c`, and G15 tail `+0x1840`.

## HwDataA

Apple allocates G15 HwDataA as exactly `0x4360` bytes. The generated G14-style/G15 object before a G15-specific extension ends at exactly `0x421c`. The required difference is therefore exactly `0x144` bytes.

Firmware directly uses fields in this missing extension, including:

- `+0x4298` as a feature/gating word,
- `+0x4324` as a runtime accumulator/reset word,
- `+0x4328` as a floating-point runtime accumulator,
- `+0x432c/+0x4330/+0x4338` in runtime state/timestamp paths,
- `+0x433c` as a progress/epoch-like value used by legacy progress checking,
- `+0x4340/+0x4348` as runtime accumulation/timestamp values,
- `+0x4358` as a firmware-updated state value.

The compile-only kernel model appends an exact `0x144` G15 tail at `+0x421c`, names only the firmware-proven offsets, and keeps unresolved bytes zero. This makes `HwDataAG15V14_7` exactly `0x4360`. Wrapper `+0x441` is now populated with the HwDataA GPU address, matching the Apple wrapper ABI.

This is structural/ABI progress only. It does not prove all inherited pre-`0x421c` field semantics and does not enable G15 runtime dispatch.
## HwDataB +0x28: exact G15 eGartRange-11 base

Direct AGXG15G host disassembly in `AGXFirmware::initFirmwareData()` materializes `0xfffffc2011800000`, calls firmware vtable `+0x2c8` (`convertGPUVAToFWVA`), then stores the result through host CPU member `+0x210` at object offset `+0x28`. `+0x210` is the exact CPU mapping of HwDataB. ChinookV9 conversion is identity, so G15 HwDataB `+0x28` is exactly `0xfffffc2011800000`, the base of eGartRange 11. In the generated G15 layout the inherited `timestamp_area_base` member lands at exactly `+0x28`; this proves that member must carry range-11 base on G15 rather than Linux's legacy timestamp VA. The compile-only builder now emits the Apple value only for G15; older generations retain their prior initializer.


## HwDataB +0xa6c / q4 +0x05e production value

The generated G15 `HwDataB` layout places `power_sample_period` exactly at `+0xa6c`. Apple does not source this field from the normal dynamic power configuration on J615: `AGXFirmware::initFirmwareData()` writes 200 only when `_gSharedProbeConfig+0x12 bit4` is set, otherwise 1.

That bit is derived from the `model-slow` provider property. The exact J615 23J220 ADT omits `model-slow`, so the Apple production path writes `HwDataB+0xa6c = 1`. `AGXArmFirmware` later copies/defaults the same value into q4 `+0x05e`. The Linux compile-only G15 initializer now mirrors both exact values and compile-time asserts the HwDataB offset.
