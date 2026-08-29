# G15 FList prepare / complete residency lifetime — E087

Exact target: macOS 14.8.3 / 23J220 AGXG15G plus the matching KDK IOGPUFamily. This note records derived semantics only; no Apple binary or raw disassembly is published here.

`AGXHardwareBufferIDManager::alloc()` uses the FList `prepareBufferResources(bool)` callback. On the fresh/full branch, exact `AGXUMAFList::prepareBufferResources(true)` prepares the four persistent mappings in this order: Page Pool List, 0x70-byte Page-Pool State, 8-byte FW-Uncached State, then Backup Page List. Failure unwinds the mappings already prepared. It then prepares any chained backing mappings and, on the first initialized epoch, runs `populatePagePool()` followed by `populateFirmwareState()`.

`AGXUMAFList::completeBufferResources()` completes those four mappings and any prepared chained mappings and clears the active-prepared flag. It does not destroy the mapping objects and does not clear the persistent initialized-state flag.

The matching IOGPUFamily closes the important lower-level lifetime. `IOGPUMemoryMap::prepareMapping()` forwards to `IOGPUMemory::prepareMemory()`. The first nonresident preparation reference wires/pins memory when necessary and calls `commit_mappings()`, which commits each mapping PTE. Repeated preparation references do not recommit the PTE. `completeMapping()` forwards to `completeMemory()`, which drops one preparation reference and may notify resource completion on the final transition; it does **not** release the PTE. PTE removal is separate and occurs through `release_mappings()` / `release_pte()`.

Therefore a Linux FList implementation does not need per-command map/unmap. A conservative bring-up model may activate and map the four exact backings on the first HardwareBuffer 0→1 reference, retain those mappings through later zero-reference idle periods, and unmap only when the FList itself is torn down after all users retire. The HardwareBuffer reference/sticky-ID state still has to track every command epoch independently.

This result does not authorize RunCompute. Dynamic page-pool contents/growth, HWMetrics, execution-stream production, and recovery still remain separate requirements.
