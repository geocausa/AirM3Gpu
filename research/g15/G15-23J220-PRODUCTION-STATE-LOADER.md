# G15 23J220 production state-loader closure

E280-E283 reduce the remaining direct-Compute execution difference to the exact pre-shader state-loader byte grammar.

## Exact compiler result

A provenance-clean 23J220 backend run produced the target G15 Compute executable. The production `_agc.main` body uses the ordinary argument-pointer ABI rather than the earlier hand-written fixed-output-address body. Replacing the diagnostic body with that production body did not change the live failure class: firmware/scheduler retirement still completed, followed by engine-completion timeout.

## Exact direct-loader descriptors

Mechanical decoding of the matching 23J220 compiler metadata gives exactly two direct-loader records for the bounded kernel:

- BufferBindings: source index 0, destination words 0..1, two 32-bit words;
- Statics: source index 0, destination words 2..3, two 32-bit words.

The exact driver maps the BufferBindings record through its UserBuffer path and emits a two-word load. The Statics record becomes an independent absolute two-word load.

The statics contents are closed as well: the compiler reply contains exactly eight zero bytes of static constants and the driver copies them at offset zero of the per-variant statics allocation. There is no remaining unknown statics payload for this workload.

## E283 live discriminator

E283 changed the E281 approximation from one four-word fetch to two independent two-word loads while preserving the production shader body and the established E274 command envelope. The candidate was statically validated, committed and pushed before one one-shot live boot.

The live result retained the established failure signature: the Compute pipe retired to `(1,1,1,1)`, the first RunWorkQueue was accepted by the scheduler, and engine completion was still absent roughly six seconds later. The machine then recovered to the persistent Golden kernel and the sacrificial candidate files were restored.

Therefore correct final register values plus the two-load topology are still insufficient. E283 must not be retried unchanged.

## Current boundary

The next discriminator is the exact 23J220 byte-generation path itself: `loadFromUserBuffer` / `loadBufferPointer`, the independent absolute Statics load, `finishRound`, and the resulting LoadShader/tail sequence. The retained newer generated G15 entry is useful as a structural cross-check but is not a substitute for exact-23J220-produced bytes.

No further live launch is justified until that exact byte grammar yields a concrete difference from E283, or proves E283 byte-equivalent and moves the fault boundary elsewhere.
