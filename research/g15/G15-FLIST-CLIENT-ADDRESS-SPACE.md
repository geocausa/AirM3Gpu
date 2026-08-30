# J615 FList client address-space provenance (E139)

E139 closes the allocator/address-space ambiguity left after placing the device-global UMA host state.

Exact macOS 14.8.3 / 23J220 host analysis shows that `AGXUMASharedPoolContainer` is **not** accelerator-global. `AGXShared::init()` constructs it at `AGXShared +0x1b8`; the container is initialized with the accelerator plus its owning `AGXShared *`.

The inherited shared-object initializer stores the incoming client `task *` at `AGXShared +0x50`. Its virtual `+0x1a0` call resolves through the AGXShared vtable to `AGXShared::createUserGPUTask(task *)`, and the resulting client `IOGPUTask *` is stored at `AGXShared +0x58`.

`AGXUMAPool::init()` carries those two values into pool `+0xd8/+0xd0`, and `AGXUMAFList::init()` carries them again into FList `+0x30/+0x38`.

The exact FList allocations then separate into two address-space classes:

- **Page Pool List**: eGartRange 5, option `0x20000000003`, client `task *`, client `IOGPUTask *`.
- **Backup Page List**: eGartRange 5, option `0x20000000003`, kernel/global backing task argument, but the **same client `IOGPUTask *`**.
- **Page Pool State**: 0x70 bytes, range 8, no client IOGPUTask.
- **FW-Uncached State**: 8 bytes, range 7, no client IOGPUTask.

Thus both persistent range-5 list mappings belong to the owning client's IOGPUTask/bank-0 address space even though their backing-task arguments differ. The range-7/8 fixed objects remain accelerator-shared bank-1 resources.

## Linux consequence

The existing G15 range-5 allocators owned by Linux `file::Vm` already map into that client `mmu::Vm`. They are therefore the correct address-space source for future FList Page/Backup List backing. A new device-global range-5 allocator would be wrong.

The remaining ownership problem is narrower: Apple's four weak shared-pool slots live at `AGXShared` lifetime, so the Linux shared Compute-pool selection state must be scoped to a client VM and shared by every Queue targeting that VM. It must be neither one pool/container per Queue nor part of E138's accelerator-global UMA state.

E139 is static-only. Linux remains at E138 commit `f2cb03001da02b1bde833550f38700b247ca30c2`; no module is installed and no RunCompute is issued.
