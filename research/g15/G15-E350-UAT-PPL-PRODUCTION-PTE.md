# G15 E350 — production UAT-PPL PTE encoder

Research state: E350, 2026-09-30.

This note records the sanitized conclusion of the exact 23J220 UAT backend audit. Raw kernelcache material and decompiler output remain machine-local.

## The earlier AP=0 result used the wrong backend

The direct AGX UAT encoder previously audited is real, but production G15 does not use it for ordinary SecureGart mappings. Exact G15 startup enables the IOUAT path, and SecureGart consequently routes mappings through Apple's protected pmap-IOMMU / UAT-PPL service.

The pool-0x16 command stream still reaches SecureGart with the already-proven compact option 0x108. The important correction is which final PTE encoder consumes that option.

## Protected production chain

The production path is SecureGart -> IOUnifiedAddressTranslator -> protected pmap-IOMMU map dispatch -> UAT-PPL map callback.

The private UAT-PPL encoder uses a different option table from the direct AGX encoder. For compact 0x108, the production table entry supplies AP=2, uncached memory, the GPU-access high bit, and no PXN/UXN.

The resulting protection bits are:

`0x0080000000000088`

That is the same PTE shape represented by Linux `PROT_GPU_SHARED_RO` in the current experimental model.

## Independent live cross-check

E348 had already isolated AP as the live causal dimension and completed a terminate-only CDM fetch with exactly that AP=2 / uncached / PXN=UXN=0 shape. It reached WorkQueue completion with `error=None`, `doneptr == wptr == 1`, and no GPU timeout.

The E350 static production-path result therefore reconciles the Apple reference with the independent Linux live discriminator.

## 0x108 and 0x308 are not equivalent

One previous simplification must also be retired. In the production PPL encoder, compact 0x108 and 0x308 do not collapse to the same PTE class. Both use AP=2 and uncached memory, but 0x308 also selects a distinct high permission/XN bit.

That means Linux should not globally replace every existing 'range-5 uncached' use with one value. Individual Apple resource classes must be re-reduced through the PPL encoder.

## Next gate

The first exact correction is pool 0x16 itself. Before another real shader launch, the pool-5 ESL entry, UserBuffer argument table, Statics, output buffer, CodeHeap body, and profile helper should each be reclassified through the production UAT-PPL table rather than the direct fallback encoder.

No GPU command was issued in E350.
