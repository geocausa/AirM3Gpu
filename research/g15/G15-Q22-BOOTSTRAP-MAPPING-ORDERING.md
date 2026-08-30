# J615 q22 bootstrap mapping ordering (E151)

E151 closes the q22 ordering prerequisite left open by E150 using the exact macOS 14.8.3 / 23J220 J615 host image.

The five accelerator-global firmware-resource stacks create their first backing during firmware allocation, before mapping notification is enabled. Four stacks are eagerly backed by `AGXFirmware::allocFirmwareData()` and the scheduler stack is eagerly backed later by `AGXArmFirmware::allocFirmwareData()`. The exact `AGXArmFirmware` constructor independently proves the q22 gate starts clear: it establishes `x20 = firmware + 0xae8`, then stores zero bytes at `[x20 + 0x7a0]` and `[x20 + 0x7a1]`, exactly firmware `+0x1288/+0x1289`.

`AGXArmFirmware::notifyNewMapping()` tests `+0x1288` before entering the q22 producer. Successful `prepareFirmwareData()` is the startup transition that locks the firmware object and writes `1` to `+0x1288`. The exact `createHardwareMappingsAndBootFirmware()` path is then `prepareFirmwareData()` -> `initFirmwareData()` -> `bootFirmware()`. The eager resource PTEs are not replayed into q22. Apple therefore has an asymmetric startup contract: bootstrap resource maps are silent; later mapping mutations are q22-visible.

Patch 0063 models that contract without making E150's mapped resource owner live. `G15MappingNotifier` now carries a monotonic inactive/active state, and activation is rejected unless the q22 ring is empty. Existing shared-bank1 allocators keep their immediate-notification default. Only the still-zero-caller mapped firmware-resource factory chooses `AfterActivation`: an initial pre-activation map is silent, a pre-activation teardown is also silent, but the backing retains the notifier so teardown after activation publishes q22 unmap; growth after activation publishes the normal map/unmap pair.

The activation method has no live caller in E151. `new_mapped_device_global()` and `mapped_fwva()` remain definition-only, and `gpu.rs` is unchanged. This checkpoint therefore changes no current GpuManager runtime mapping, submission, or RunCompute path.

Validation: strict source-diff checkpatch 0/0/0; exact patch-tree reconstruction PASS; external Asahi module build PASS at the established 24-warning baseline; module SHA-256 `7de35a06a1a2f038cb4392f07429651d05d30499a089e068740b18a10a84e556`; vermagic `7.1.6-gc6fa9b794ba9 SMP preempt mod_unload aarch64`.

Linux checkpoint: `823fb161085e61788585bdfb57fdf0e8481d6aea`, tree `fcf4097b98e20b6c341e010468039056d2b7c6f8`.
