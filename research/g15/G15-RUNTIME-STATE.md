# G15 compact-wrapper firmware runtime state

This note records firmware-owned fields inside the exact 0x490-byte G15 wrapper. Evidence is from RTKit-2419.140.12 decompilation; all work is offline.

## Region +0x2d8 .. +0x3af

Firmware binds an internal pointer directly to `wrapper + 0x2d8`. On first initialization it clears and seeds this block itself. The observed host initialization methods do not populate this range, so Linux keeps its compile-only initial image zeroed while the firmware-owned layout is modeled explicitly.

Using a u32 pointer based at wrapper +0x2d8, firmware performs the following first-init writes:

```
base[0]       = 0                         -> +0x2d8
memset(base+1,  0, 0x10)                 -> +0x2dc..+0x2eb
memset(base+5,  0, 0x10)                 -> +0x2ec..+0x2fb
base[9]       = 1                         -> +0x2fc
*(u64*)(base+10) = 0                      -> +0x300
*(u64*)(base+12) = 0                      -> +0x308
base[15]      = 0xabcdabcd                -> +0x314
memset(base+0x10, 0, 0x0c)               -> +0x318..+0x323
memset(base+0x13, 0, 0x48)               -> +0x324..+0x36b
base[0x28]    = 0                         -> +0x378
*(u64*)(base+0x29) = 0                    -> +0x37c
*(u64*)(base+0x2b) = 0                    -> +0x384
*(u64*)(base+0x2d) = 0                    -> +0x38c
*(u64*)(base+0x2f) = 0                    -> +0x394
*(u64*)(base+0x31) = 0                    -> +0x39c
*(u64*)(base+0x33) = 0                    -> +0x3a4
```

The +0x310 word is intentionally retained across firmware re-entry and is tested when the internal runtime pointer is already installed, so it is modeled as firmware state rather than declared a host constant.

## Proven live semantics

`+0x300` and `+0x308` are 64-bit active masks. Firmware ORs resource masks into them and excludes their bits during subsequent selection.

`+0x318`, `+0x31c`, and `+0x320` are three u32 activity counters used by firmware scheduling/statistics paths.

`+0x324..+0x36b` is exactly 0x48 bytes. Firmware indexes it as 16-bit counters with `wrapper + 0x324 + 2 * type`, proving a 36-entry u16 array.

`+0x36c` is another firmware-mutated u32 activity counter. Its surrounding +0x370..+0x377 bytes remain semantically unresolved.

The `0xabcdabcd` word at +0x314 is a firmware-written initialization marker.

## Wrapper tail +0x459 .. +0x48f

`AGXArmFirmware::init()` directly clears four unaligned qwords:

```
+0x46d = 0
+0x475 = 0
+0x47d = 0
+0x485 = 0
```

These occupy 0x20 bytes inside the 0x37-byte tail. The prefix +0x459..+0x46c and final +0x48d..+0x48f are still unresolved.

## Kernel representation

The isolated kernel now represents +0x2d8..+0x3af as an exact 0xd8-byte `G15RuntimeState`, with compile-time size and offset assertions for the firmware-proven fields. The wrapper tail is an exact 0x37-byte `G15WrapperTail` with assertions for the four host-cleared unaligned qwords.

No runtime G15 dispatch is enabled by this work.

### Wrapper tail bootstrap closure

The whole 0x490 wrapper is allocated through `AGXFirmware::allocateSharedData()`, which zeroes the complete mapped allocation before publishing its pointers. Therefore the previously unresolved tail prefix `+0x459..+0x46c` and suffix `+0x48d..+0x48f` are also exact zero at bootstrap. `AGXArmFirmware::init()` additionally re-clears the four unaligned qwords `+0x46d/+0x475/+0x47d/+0x485`. The kernel now models the complete 0x37-byte tail as zero-initialized while leaving its runtime semantics intentionally unnamed.
