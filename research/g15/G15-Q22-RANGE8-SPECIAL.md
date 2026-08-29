# G15 q22 range-8 special-aperture flags — E085

E085 closes the q22 mapping-notification flag prerequisite for the exact 23J220 range-8 Page-Pool-State mapping. It is **compile-only**: no new FList resource is instantiated and no GPU command is enabled.

## Exact behavior

The previously reconstructed 23J220 `AGXArmFirmware::notifyNewMapping()` / `notifyNewUnmapping()` path classifies `0xfffffc200c000000..0xfffffc200fffffff` as the special 64-MiB firmware aperture. q22 mapping flags are therefore:

- ordinary range-7 map: `1`;
- ordinary range-7 unmap: `0`;
- range-8 special-aperture map: `3` (`map | special`);
- range-8 special-aperture unmap: `2` (`special`).

The special bit deliberately survives unmapping. The 23J220 descriptor's separate property bit is not asserted by the current Linux owners.

## Linux change

Linux commit `9b21157497bb` makes the existing G15 q22 producer address-class aware. Range 7 retains the already-live-tested `1/0` encoding. Range 8 uses `3/2`. The CPU-side q22 preflight verifies all four encodings before exercising its ordinary range-7 roundtrip.

This is deliberately resolved **before** constructing a persistent range-8 `G15UMAPagePoolState`: the allocator could otherwise map the right PTE with the wrong firmware-notification flags.

Validation:

- base: `b88369c26ffe82ce73765b8bbba64b2db771ef76`;
- checkpoint: `9b21157497bb`;
- checkpoint tree: `dcf3368ccd71d1166d5198cab9dd21b3daa2d6f8`;
- module SHA-256: `a8d3ab97f621da84fd6d345543582fe1f59251ccd3170225ecebc8cbd4789398`;
- vermagic: `7.1.6-gc6fa9b794ba9 SMP preempt mod_unload aarch64`;
- strict checkpatch: 0 errors, 0 warnings, 0 checks;
- patch 0017 exact-tree reconstruction: PASS;
- runtime/install: none.

## Boundary

The next source step is the compile-only persistent FList resource owner. Page Pool List, Backup Page List, FW-Uncached State and Page-Pool State now have exact target geometry, mapping classes and required q22 encoding. Live RunCompute remains blocked on their complete prepare/complete ownership plus HWMetrics, exact SKU execution production, and stamp/notifier completion/recovery closure.
