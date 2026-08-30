# J615 private VM lifetime (E160)

E160 advances only the private userspace VM create/destroy boundary beyond E159 discovery. A fresh `mmu::Vm` creates its own bank-0 page-table root and host VA allocator state. Firmware-visible user-context publication remains behind `Uat::bind()`, whose active driver call sites are Queue-side and remain gated.

The E139/E140 per-VM range-5 allocators are constructor-only here: `HeapAllocator::new()` allocates no backing GEM/PTE until a later allocation request. The four-slot client UMAPool container starts empty and consumes no pool ID or GPU allocation. VM creation's automatic dummy GEM mapping changes only the new private root and uses no q22 notifier.

The one-shot E160 candidate live-created and destroyed a valid J615 VM, proved GEM_CREATE and QUEUE_CREATE still returned `ENODEV` while the VM existed, repeated the full create/gated-GEM/gated-Queue/destroy sequence 16 more times, and remained fault-free for more than three minutes. No user VM bind/slot marker or post-bootstrap q22 runtime mapping publication appeared.

Linux checkpoint `14e6cd225690021730d1723200367e0bab39fe04`, tree `a1522ab12d9d1749e54e55cf97a6b4e811b1d547`. No RunCompute/custom GPU command was issued.
