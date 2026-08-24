# G15 RTKit crashlog preallocation

Checkpoint: 2026-08-24, J615 / T8122 G15G C0.

## Result

RTKit system endpoint EP1 supplies its crashlog as firmware-owned physical DRAM, not as an AGX UAT virtual address.

Observed request:

- raw RTKit message: `0x1041000192c000`
- size: `0x4000`
- supplied address: `0x1000192c000`

The iBoot-populated live ADT places that address inside `/chosen/carveout-memory-map` `region-id-25`:

- start: `0x10001888000`
- size: `0x6ec000`
- end: `0x10001f73fff`

The requested page is at offset `0xa4000` inside that firmware carveout.

## Why it is not an AGX UAT mapping

Read-only page-table walks were performed both when EP1 announced the buffer and after the complete RTKit system-endpoint management handshake.

Neither form has a TTBR0 leaf mapping:

- full address: `0x1000192c000`
- historical low-40 AGX view: `0x192c000`

Firmware also does not install either mapping later during the management handshake.

## Physical backing proof

A bounded read-only `memremap(WB)` of the exact firmware-selected address succeeds. The first four qwords read as:

```text
0xefefefefefefefef
0xefefefefefefefef
0xefefefefefefefef
0xefefefefefefefef
```

The diagnostic mapping was immediately dropped. This establishes that the buffer is CPU-readable ordinary firmware-carved DRAM rather than MMIO or an opaque GPU VA.

## Linux model

The tested Asahi RTKit path now supports two backing classes:

1. Linux-allocated GEM/UAT shared buffers;
2. G15 firmware-preallocated physical buffers retained with `memremap(WB)` for the RTKit buffer lifetime.

The G15 physical mapping path is deliberately constrained by aperture, page alignment, and maximum size while bring-up remains fail-closed.

Kernel commit:

`335b42046b85bf91fa78dbbb30862742a3571d97`

## Live PASS

Tested artifact set:

- kernel release: `7.1.6-m3-g15-eot-gb3d38d16c076-dirty`
- kernel Image SHA-256: `9f40eba872327e595577b7b74a12b3e2593620e3999967f6bff2ff28be1e2ed2`
- `asahi.ko` SHA-256: `a847692304126ea53694c27626e24b0ba93a79f0b199fe2348e02ece95e91d90`
- m1n1 `boot.bin` SHA-256: `a81993b122b7f990bc99ccab42decab7ef92adb94741dadf0ee79d24fdda3e0d`

Postconditions:

- zero `RTKit: failed buffer request` messages for the GPU
- RTKit protocol v12 management PASS
- EP20 firmware + EP21 doorbell discovered but not started
- q21 untouched
- no `MSG_INIT`
- no DRM render node
- ASC stopped after the preflight
- persistent GRUB default remains the golden kernel

## Safety boundary

This result closes EP1 crashlog backing only. It does not authorize application endpoint startup, InitData handoff, `MSG_INIT`, DRM registration, or queue submission.
