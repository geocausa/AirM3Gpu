# G15 E329-E349 CDM mapping and UAT-permission bisection

Research state: E349, 2026-09-29.

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

Therefore uncached memory/coherency is valid for the CDM stream.

## AP isolated by E348

E347 retained the same low VA, independent allocation, uncached memory attribute and PXN=0, but changed the completing GPU-only RW mapping to the GPU-only read-only form. This clears the UXN/RW permission bit while preserving AP=2. The protected E348 run still completed normally.

E343/E344 and E347/E348 therefore form an AP-only pair: AP=0 times out while AP=2 completes with the other relevant PTE fields held fixed. The current Linux hardware path is sensitive specifically to the AP field.

## Exact Apple PTE re-audit

E349 then re-audited the exact 23J220 UAT page-table path and found that the prior raw Apple leaf reduction was not arithmetically wrong. The corrected virtual-call topology is:

- UAT page-map method at vtable +0x1c0;
- `encodePTEFlags` at vtable +0x190;
- the page-map method derives bank from VA bit 42, calls `encodePTEFlags`, then ORs the encoded flags with the physical page address.

For the low range-5 bank-0 mapping and compact `0x108`, the exact 23J220 encoder still yields leaf protection bits `0x0080000000000008`: bit 55 set, AP=0, uncached, PXN=UXN=0. There is no hidden post-encoder AP rewrite on that leaf path.

This changes the interpretation of the problem. Older UAT permission semantics predict exactly the Linux live result: with bit 55 set, AP=0/UXN=PXN=0 is not GPU-readable while AP=2 is. Apple nevertheless uses the AP=0 leaf on G15. The remaining mismatch is therefore the surrounding **G15 UAT execution mode/context contract**, not the pool-0x16 resource classification or leaf encoder arithmetic.

Linux already publishes the firmware-visible G15 UAT-mode header and GPTBAT roots, but that does not yet prove that every hardware/protected-runtime prerequisite controlling the non-legacy permission interpretation is active. No production PTE change is justified until that mode/context path is understood.

## Consequence

The next step is static: trace exact G15 UAT-mode activation and context publication outside the leaf encoder, including the firmware consumer of the UAT-mode field and any separate protected/runtime setup. No further live GPU command is needed for the AP discriminator itself.

All live runs used the protected one-shot candidate boot protocol and returned to Golden Ubuntu with the sacrificial slot restored.
