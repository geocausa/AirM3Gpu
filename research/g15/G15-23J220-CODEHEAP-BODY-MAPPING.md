# G15 exact 23J220 CodeHeap shader-body mapping

Research state: E323, 2026-09-29.

This note records the sanitized mapping conclusion. Raw Apple binaries,
decompiler output and the Ghidra database remain machine-local.

## Exact body ownership

The exact 23J220 compiler reply used by the project is a Mach-O object whose
first compiler section is __TEXT,__text, size 0x7a. Exact
AGCDeserializedReply construction assigns __TEXT to reply section index 0.

The exact ProgramVariantESLState path consumes that section, allocates it with
AGX::Heap<true>, copies the compiler bytes into the returned CPU mapping and
publishes the returned GPU address into the program state used by LoadShader.

For ordinary direct Compute the selected heap is the embedded Device heap named
com.apple.AGXMetal.CodeHeap. This is distinct from the ICB code heap.

## Exact resource mapping

The CodeHeap is initialized from the exact static agx_generic_heap_args
resource record. The mapping-relevant fields are:

- +0x13 = 1
- +0x14 = 0
- +0x20 = 0
- +0x28 = 0
- +0x48 = 0x0000000000010000
- +0x58 = 0x0000000c08000000

The low mapping-policy word at +0x58 is 0x08000000: bit 27 is set and selector
bits 28..30 are zero.

Reducing the record through the exact 23J220 IOGPUResource and AGXResource
mapping functions selects eGartRange 5. The range-5 SecureMemoryMap/SecureGart
path cannot introduce the secure/cached property bit used by the alternate
class. The recovered bank-0 UAT encoding therefore gives protection bits
0x0080000000000008.

This is the same protection class modeled in Linux as
PROT_G15_RANGE5_UNCACHED: uncached, AP=0, G15 GPU-access bit set, PXN=UXN=0.

## Linux discriminator

The E321 STOP-only candidate still places the body blob in
_g15_range5_code, whose compile-time-pinned protection bits are
0x00c0000000000080.

That class is cached, GPU-only and UXN-marked. It is not PTE-equivalent to the
exact 23J220 CodeHeap.

This gives a new one-variable discriminator after E322: move only the STOP/body
backing to the exact range-5-uncached class while preserving the already-closed
ESL, CDM, argument-table, statics, output and submission-envelope state.

No live GPU command was issued for E323.
