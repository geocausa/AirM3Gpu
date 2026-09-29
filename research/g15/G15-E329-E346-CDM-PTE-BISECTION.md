# G15 E329-E346 CDM mapping live bisection

Research state: E346, 2026-09-29.

This note records the sanitized conclusion of the terminate-only regression bisection. Raw boot logs, kernel journals, candidate binaries, and proprietary reference artifacts remain machine-local.

## Regression boundary

The contemporary corrected launch environment unexpectedly stopped reproducing the earlier terminate-only completion oracle. A protected one-shot bisection narrowed the first break to E298:

- E252 baseline terminate control: engine completion PASS.
- E283 baseline terminate control: engine completion PASS.
- E297 baseline terminate control: engine completion PASS.
- E298 baseline terminate control: GPU timeout after first RunWorkQueue acceptance.
- E299 baseline terminate control: same failure.

E298 is the change that split the direct CDM/token stream into its own retained allocation and mapped that allocation with the then-derived pool-0x16 range-5-uncached PTE class.

## Allocation split vs mapping class

E341 retained the E298 independent CDM allocation but moved only that allocation back to the older low range-5-code allocator. The protected E342 run completed normally. Therefore independent ownership/allocation is not the regression.

E343 then kept the same known-completing low VA while changing only its PTE protection to the modeled PROT_G15_RANGE5_UNCACHED class. E344 timed out immediately after scheduler acceptance. This proves the failure is in the PTE/visibility class itself, not Linux's artificial high uncached VA sub-arena.

## Uncached memory attribute is not the problem

E345 kept the same low VA and independent terminate-only CDM root but used PROT_GPU_SHARED_RW, which retains GPU-only RW access while selecting the uncached memory attribute.

The protected E346 run completed:

- first RunWorkQueue accepted;
- WorkQueue callback fired with error=None;
- selected transport state reached doneptr=1, wptr=1;
- no GPU timeout occurred.

The host result helper still reported a stale EIO because it reads an old result offset after the CDM/result ownership split. That does not affect the engine-completion conclusion.

Therefore uncached memory/coherency is valid for the CDM stream. The live failing dimension is narrowed to the access/XN encoding difference between PROT_GPU_SHARED_RW and the currently modeled PROT_G15_RANGE5_UNCACHED class.

## Consequence

This live result conflicts with the earlier static E295/E298 reduction that mapped Apple pool 0x16 to protection bits 0x0080000000000008. That static evidence is not discarded; it now requires a targeted re-audit.

The next step is to isolate the AP/UXN components one at a time on the same low-VA terminate-only control, then re-check the exact 23J220 SecureGart/UAT table reduction before changing production mapping policy.

All live runs used the protected one-shot candidate boot protocol and returned to Golden Ubuntu with the sacrificial slot restored.
