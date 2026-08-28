# G15 dedicated range-8 allocator

E076 turns the E075 range-8 Page-Pool-State PTE proof into allocator topology without creating a persistent FList object yet. This checkpoint is compile-only.

Linux commit `6e3850dcdd51d4bd912b9c02b0ea9633c7fd7060` separates G15 shared-bank1 VA bookkeeping into two independent arenas:

- eGartRange 7 retains the existing PM/q22 allocator and caller-provided range-7 protection class;
- eGartRange 8 receives a separate `mm::Allocator` arena and an explicit aperture tag;
- `G15SharedBank1Allocator::new_range8()` hard-wires `PROT_G15_RANGE8_FW`, whose E075-proven leaf is `0x00c0000000000443`.

The existing range-7 constructor remains the default for every current caller, so E076 does not migrate or reinterpret PM/q22 resources. The new range-8 constructor is deliberately unused in this checkpoint: no persistent Page-Pool-State mapping exists and RunCompute `+0x83e` remains zero/fail-closed.

Validation: module build PASS at the exact existing 24-warning baseline, strict checkpatch clean, module SHA-256 `ec87dd9d33a4de3cd643637549c215f452a9adee2e62f14dfc6bea8b4c0a46fe`. No module was installed and no live probe was performed.

Next is the persistent FList-owned Page-Pool-State/FW-Uncached-State object lifetime and the 256-entry HardwareBuffer-ID prepare/complete ownership that gates exporting either pointer to a command.
