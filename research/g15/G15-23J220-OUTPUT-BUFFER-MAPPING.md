# G15 23J220 Compute output-buffer mapping closure

Research state: E318/E319, 2026-09-29.

This note records only independently reconstructed conclusions. Raw Apple binaries,
dyld caches, RIDIFF payloads, decompiler output, and other proprietary artifacts are
not included.

## Exact 23J220 application-output parent mapping

A locally reconstructed macOS 14.8.3 / 23J220 SystemOS image was used only as an
offline reference. The exact historical class path for an ordinary shared Metal
buffer is:

AGXBuffer -> IOGPUMetalBuffer -> IOGPUMetalResource -> IOGPUResourceCreate.

For the project oracle/result buffer, newBufferWithLength:4096 with shared storage
enters the ordinary AGX wrapper with the AGX extended resource argument at +0x58
zero. The small-buffer primary allocation path re-enters the same wrapper, while the
IOGPU superclass initializes fields through +0x50 without overwriting the AGX
extension. The ordinary unpinned path therefore reaches AGX mapping selection with
+0x58 = 0, +0x20 = 0, and +0x28 = 0.

The resulting mapping selection is eGartRange 5. Combining that with the previously
closed ordinary low-control states yields the exact bank-0 protection class:

0x0080000000000008

This is the project's G15 range-5 uncached GPU-RW class.

## Linux consequence

E310 had already moved the copied UserBuffer argument table into the exact range-5
uncached class, but it still kept the synthetic result/output allocation in the
range-5 executable/code class.

E319 changes only that remaining mismatch: the g15_result allocation now comes
from the existing range-5 uncached allocator. The result size, initialization,
UserBuffer table, statics, ESL entry, CDM stream, shader body, profile helper,
entry/body bytes, and submission envelope remain unchanged.

Linux implementation branch:

wip/g15-e319-output-range5-uncached

Implementation commit:

16bde06d43ebab6c73a4d18a2bb89a897ccb2439

The isolated module build and static source audit pass. This publication does not
claim live shader completion; E319 was still unexecuted at the publication
checkpoint.

## Boundary

This closes the previously open production application-output PTE class. Future
real-launch testing can therefore treat both the copied UserBuffer argument table
and the shared application result buffer as range-5 uncached without relying on a
cross-version macOS observation.
