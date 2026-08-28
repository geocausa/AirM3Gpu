# G15 Compute ABI — exact macOS 14.8.3 / 23J220 cross-check

E066 closes the outer G15 Compute firmware-container geometry against the exact Apple ABI targeted by the M3/J615 Linux bring-up. This is static/oracle work only; no Linux `RunCompute` command was issued.

## Exact Apple components

The matching macOS 14.8.3 build 23J220 KDK contains the T8122/G15 kernel components used for this cross-check:

- `AGXG15G.kext` version `282.14.2`, SHA256 `e29327fd1eeec53ea47bba91572d393cd6bc38ab77b9ac3c9ab62cc70f94854b`
- `AGXFirmwareKextG15GRTBuddy.kext` version `282.14.2`, SHA256 `2b196050a4d14a59e2a6774e5c91483ae538afa65b14a9f35a9f691ca79e7fb7`

Apple binaries are not redistributed here. Only derived constants, offsets, and checksums are recorded.

## Firmware command-pool sizes

`AGXFirmware::configurePoolElementSizes()` reads the exact 23J220 literal table:

- TA `0x940`
- 3D `0xc80`
- FastBlit `0x980`
- Compute/CLE `0x880`
- Barrier `0x40`

The important result is the Compute/CLE command object: **exactly `0x880` bytes**. The Linux candidate independently reconstructed `RunComputeG15V14_7` with a compile-time `size_of == 0x880` assertion, so this boundary is now same-ABI validated instead of inferred from the newer 25F84 oracle.

## Accelerator-ring submission entry

The exact 23J220 `AGXArmFirmware::encodeAcceleratorRingCommand()` produces a 0x18-byte `AGFIAcceleratorDataMasterEntry`:

- `+0x00` submission timestamp
- `+0x08` work-queue pointer
- `+0x10` data-master / pipe type
- `+0x14` queue write-pointer / sequence field
- `+0x16` event-slot byte
- `+0x17` new/continuation byte

`AGXFirmware::submitCLChannel()` selects data-master type **2**; 3D selects 1 and TA selects 0. The Linux G15 `RunWorkQueueMsg` independently has the same `0x18` size and exact field offsets, with Compute pipe type 2.

## Resource retirement

Exact 23J220 `AGXArmFirmware::submitReleaseResource()` initializes its device-control message with opcode **`0x11`**, independently confirming the G15 correction established during the registration lifecycle.

## Remaining boundary

The candidate still intentionally leaves the G15 Compute RegisterArray empty/fail-closed. The newer 25F84 oracle already exposes the hardware-facing register program, including `0x1a420` for the raw CDM stream and the J615 dynamic IDs `0x101d8/0x107a0`, but those values are not being copied blindly into the 23J220 target.

The next same-ABI requirement is the exact 23J220 user-space G15 RegisterArray producer (or an equivalent exact producer trace). Only after that producer and the remaining per-command state are mechanically closed is a bounded Linux `RunCompute` probe justified.
