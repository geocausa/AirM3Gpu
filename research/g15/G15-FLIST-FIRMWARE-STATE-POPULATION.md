# G15 FList firmware-state population

E099 models the exact 23J220 post-`populatePagePool()` `AGXUMAFList::populateFirmwareState()` boundary without making the FList reachable from submission.

The compile-only owner now accepts only the dynamic values with exact host producers: async-grow enable, UMA priority, current allocated bytes, rounded Backup Page List entry count, and shared-CL-pool state. It validates the sticky HardwareBuffer ID, 4-KiB page alignment, 22-bit page-count limit, Page Pool List capacity, and Backup List capacity/8-entry rounding before writing the 0x70 Page-Pool State.

The populated image uses the owner's exact Page Pool List, Backup Page List and FW-Uncached-State FWVAs, the live HardwareBuffer ID, host ring seed, page count, FW-uncached mirror, shared-compute state and required zero tail. Offset `+0x3c` remains untouched construction-zero, matching the host helper.

This is deliberately **not** a complete FList activation. The actual Page Pool List / Backup Page List contents and dynamically chained UMA backing mappings remain separately gated, the owner has no runtime call site, and RunCompute `+0x83e` remains zero/fail-closed.

Linux checkpoint: `47a8c0070a4be796f27ed64f3ca7a7f198b8af1b`.
