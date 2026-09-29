# G15 E324 exact-CodeHeap body live discriminator

Research state: E324, 2026-09-29.

This note records the sanitized live conclusion. Raw boot logs, kernel journals,
candidate images and Apple-derived evidence remain machine-local.

## Candidate

E323 proved that the direct-Compute shader body is allocated from the exact
23J220 AGXMetal CodeHeap and that its bank-0 protection class is
0x0080000000000008: G15 range-5 uncached GPU-RW.

E324 changed only the STOP/body allocation on the E321 STOP-only baseline from
the Linux range-5 executable/code class to the exact range-5-uncached class.
The selected shader remained the independently validated four-byte STOP
0e 00 00 00. ESL entry, CDM stream, argument table, statics, application
output, profile helper, loader bytes and outer submission envelope were held
fixed.

Kernel branch: wip/g15-e324-codeheap-body-class
Kernel commit: 31fb1a826f3e3ebd620bb8a7f01253fb3bf7c616

## Live result

One protected one-shot normal-UAPI Compute probe reproduced the existing
failure boundary:

- bounded Compute submission was accepted;
- selected Compute/2 reached the expected submitted state;
- first RunWorkQueue was accepted by the scheduler;
- GPU timeout fired immediately after scheduler acceptance;
- scheduler release later returned ETIMEDOUT.

The STOP body was therefore fetched from the exact Apple-equivalent CodeHeap
mapping class, yet the command still did not retire.

This proves the earlier body-PTE mismatch was real but is not sufficient to
explain the engine stall. Production shader-body semantics had already been
removed by E322, and E324 additionally removes the body allocation/PTE class as
the remaining sufficient cause.

## Recovery and next discriminator

The candidate boot returned to protected Golden Ubuntu. The sacrificial slot
was restored byte-for-byte and no experimental module remains loaded.

A new concrete mapping discrepancy follows from combining earlier E289 with
E323: E289 proved that the inactive-profile helper uses the same Device
Heap<true> instance at Device +0x158, while E323 proved that this CodeHeap maps
as range-5 uncached. The current Linux helper still resides in the older
range-5 executable/code allocator.

The next isolated discriminator is therefore to move only that retained
16-byte inactive-profile helper to the exact range-5-uncached class while
holding E324 body placement and all other execution state fixed.
