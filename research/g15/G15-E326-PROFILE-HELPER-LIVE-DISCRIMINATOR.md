# G15 E326 profile-helper live discriminator

Research state: E325/E326, 2026-09-29.

E289 proved that the persistent inactive-profile helper is allocated from the same Device Heap<true> at +0x158 used by ordinary direct program storage. E323 later proved that exact 23J220 CodeHeap maps as range-5 uncached. E325 therefore corrected the Linux helper from _g15_range5_code to _g15_range5_uncached while preserving the E324 STOP body and every other execution-facing input.

Kernel branch: wip/g15-e325-profile-helper-class
Kernel commit: 10a3e2deef0926d0964b7165f168519db8436c49

The protected E326 one-shot live run still reproduced the same boundary: normal-UAPI Compute was accepted, Compute/2 reached the submitted state, the first RunWorkQueue was accepted by the scheduler, and GPU timeout fired immediately afterward. Scheduler release later returned ETIMEDOUT.

This closes both direct CodeHeap executable allocations as sufficient explanations: the four-byte STOP body and the inactive-profile helper now use the exact range-5-uncached class, yet the command still does not retire.

Golden Ubuntu was restored and the sacrificial slot was returned byte-for-byte to its pre-live hashes.

Next boundary: generated LoadShader/program activation or another engine-side execution prerequisite. E325 must not be retried unchanged.
