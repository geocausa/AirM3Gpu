# G15 23J220 range-8 Page-Pool-State boundary

E074 isolates the mapping/ownership boundary for the 0x70-byte `UMA Page Pool State` exported at RunCompute `+0x83e`. No RunCompute or other GPU command was issued by this experiment.

## Exact host mapping

The exact macOS 14.8.3 / 23J220 host path places the persistent `AGXUMAFList` resources in these apertures:

- Page Pool List: eGartRange 5.
- Backup Page List: eGartRange 5.
- FW-Uncached State: eGartRange 7.
- 0x70-byte Page-Pool State: eGartRange 8.

For the Page-Pool-State object, `AGXAccelerator::newGPUMappedBuffer()` produces outer mapping option `0x20800000007`. Exact 23J220 IOGPUFamily was re-extracted to remove the former external-stub gap. The final range-8 leaf/PTE protection encoding is still intentionally unresolved; Linux does not guess it.

## E074 ownership preflight

Range 8 immediately follows the six already-proven range-7 parent slots in the same shared bank-1 L2. Linux commit `4e26fc0898606f09b9bf726ebba2c5452ee957f2` adds a one-shot read-only diagnostic which requires shared-L2 entries 6 and 7 to be zero before stopping ASC and returning `ENODEV`. It publishes no range-8 child table.

The exact J615 live run passed:

`MMU: G15 E074 range-8 parent preflight PASS (shared-L2[6..8) empty, read-only)`

The boot also completed the previously proven range-7 allocation/map/unmap teardown, reported no panic/Oops/SError/GPU/DART fault, consumed the one-shot GRUB entry, and returned immediately to the Golden kernel. Experimental module/initrd bytes were restored afterward.

## Boundary

E074 proves Linux may claim bank-1 shared-L2 entries 6 and 7 for range 8 on this exact J615 without colliding with firmware-owned parent entries at probe time. It does **not** authorize publishing child tables, populating `RunCompute +0x83e`, or enabling G15 userspace submission.

The next static task is to reconcile the distinct range-7 mapping classes and mechanically recover the exact range-8 leaf/PTE protection encoding. The later Page-Pool/FW-uncached lifecycle, 256-entry HardwareBuffer-ID ownership, HWMetrics, SKU execution stream, and completion/recovery contracts remain live-Compute blockers.
