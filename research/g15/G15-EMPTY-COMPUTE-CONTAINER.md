# G15 Empty Compute Container — exact 23J220 host path

E069 closes a distinction left open after E068: whether Apple's stock empty Compute record is merely parsed/serialized on the host or actually enters the normal G15 Compute command producer.

All static analysis here uses the exact macOS 14.8.3 / 23J220 KDK `AGXG15G` image already retained by E066/E067 (`AGXG15G.kext` 282.14.2, SHA256 `e29327fd1eeec53ea47bba91572d393cd6bc38ab77b9ac3c9ab62cc70f94854b`). No Apple binary or raw disassembly is published in this repository.

## Exact-target SKU grammar

The exact 23J220 `AGXCLChannelG15::encodeCLCommandSKUStream()` matches the previously reconstructed skeleton:

- optional 0x14-byte `0x90000004` packet;
- opcode `0x0000000b` plus a 0x1b8-byte payload;
- 0x3c-byte start timestamp record;
- `encodeComputeWFI()` emits exactly dword `1`;
- optional paired 0x14-byte `0x10000004` packet;
- 0x3c-byte end timestamp record;
- trailing opcode `0x0000000c`, 0x7c bytes total;
- `finishEncode()` appends `0x40000002` and rounds to a 0x40-byte boundary.

The no-feature stream is exactly `0x2b8 -> 0x2c0`; the feature-paired stream is `0x2e0 -> 0x300`.

## Empty Compute is not host-elided

The exact 23J220 path is:

`AGXCommandQueue::processCompute()`
→ successful `processComputeSetup()`
→ `addComputeToWorkqueue()`
→ `IOGPUWorkQueue::addCommandToTail()`
→ `AGXCLWorkQueue::submitCommand()`
→ ordinary CL-channel submit virtual
→ `AGXCLChannelSKU::submitBuffer()`.

There is no host-side test that rejects this path merely because the E068 empty record has a zero raw CDM/control-stream pointer or zero raw/private RegisterArray-fed fields.

The final virtual dispatch is mechanical. `AGXCLWorkQueue::submitCommand()` special-cases shared-event and remote-node command descriptors; its ordinary descriptor path uses channel vslot `+0x148`. For `__ZTV15AGXCLChannelG15`, the Itanium address point is `0x0d3c90`; `+0x148` lands at `0x0d3dd8`, whose exact target is `0x000362d4`, `AGXCLChannelSKU::submitBuffer(IOGPUCommandDescriptor*)`.

Thus the E068 stock empty Compute descriptor reaches the normal G15 producer that builds the 0x880 RunCompute/RegisterArray/SKU command container. It is not merely a host serialization artifact.

## Firmware-submit path

`AGXWorkQueue::submitCommandToFirmware()` indexes `AGXChannel::getAcceleratorSubmitFunc()::kSubmitFuncForIOFenceDMType` by data-master type. The exact table's index 2 points to `AGXAccelerator::submitCLChannel()` at `0x2d6a8`. E066 independently fixed Compute as data-master/pipe type 2, so the path agrees end-to-end.

The subsequent channel vcall at `+0x1f0` is `AGXChannel::markCommandsSubmitted()`; it is not the command producer. `submitBuffer()` has already executed on the normal-descriptor branch.

## Current safety boundary

E068's empty RegisterArray contains `0x1a420 = 0`, so the stock empty path carries no CDM launch/control-stream pointer. E069 proves Apple nevertheless carries this no-dispatch descriptor through normal G15 Compute command construction/submission machinery.

This materially strengthens the case for a bounded no-dispatch first Compute command, but it does **not** authorize a Linux live RunCompute yet. The remaining exact-target prerequisites include the UMA page-pool state/FW-uncached backing, context generation/selector, timestamp and notifier fields, stamp sequencing, completion semantics, and a fail-closed recovery contract.

No Linux RunCompute was executed for E069.
