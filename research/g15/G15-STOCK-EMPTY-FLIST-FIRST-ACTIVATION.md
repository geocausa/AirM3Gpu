# G15 stock-empty FList first activation

E100 closes the exact first-activation Page/Backup list contents for Apple's stock no-dispatch Compute command without enabling submission.

Exact 23J220 `prepareLocked()` only grows UMA backing when the desired size exceeds fresh `FList +0x68`. Its growth path also updates `AGXUMAPool +0x38/+0x40`. The stock type-5 `getDeviceUMAPoolSizes()` oracle was rerun around a successfully completed empty Metal Compute command and remained `0,0,0,0` before/after queue creation, encoding, completion and a one-second hold. Thus that stock-empty first prepare performs no nonzero pool-memory growth.

Consequently `populatePagePool()` sees no chained allocation: Page Pool List and Backup Page List contents remain zero, page count/cursors are zero, and Backup extent count is zero. The mapped resources remain real, with J615 Page Pool List capacity `0x80000` and a real HardwareBuffer ID.

Linux commit `bd95b9c01004` adds an unreachable helper that verifies the owned lists are still zero and feeds the exact empty values into the E099 firmware-state population boundary. Queue-derived priority remains dynamic. No Page-Pool-State FWVA is exposed and RunCompute `+0x83e` remains zero/fail-closed.
