# G15 E351 — production-PPL execution resource matrix

Research state: E351, 2026-09-30.

E350 established the production G15 UAT-PPL leaf encoder. E351 resolves the remaining low-control ambiguity and classifies the execution-facing resources that had previously been collapsed into one range-5 uncached class.

## Direction discriminator

Ordinary IOGPU resources use `IOBufferMemoryDescriptor` backing with descriptor direction 3. This is derived from the exact resource-creation option values and the inherited `IOMemoryDescriptor::getDirection()` path.

Direction 3 keeps the ordinary inherited mapping control at 3. AGX selector 0 reduces that to low control 1; selector 1 preserves 3; an unmarked/default range-5 resource also preserves 3.

## Exact production classes

| Resource | Compact | PPL protection |
|---|---:|---:|
| DataBuffer pool 0x16 CDM | 0x108 | 0x0080000000000088 |
| CodeHeap body + profile helper | 0x108 | 0x0080000000000088 |
| DataBuffer pool 5 finalized ESL | 0x308 | 0x00c0000000000088 |
| DataBuffer pool 0x0a Statics | 0x308 | 0x00c0000000000088 |
| DataBuffer pool 3 UserBuffer pointer table | 0x308 | 0x00c0000000000088 |
| application-output Shared parent | 0x308 | 0x00c0000000000088 |

In the current Linux protection vocabulary, the first class matches `PROT_GPU_SHARED_RO`; the second matches `PROT_GPU_SHARED_RW`.

## Consequence

The previous Linux experimental path put all of these resources into one AP=0 uncached allocator. Production Apple uses two PPL classes instead.

The next candidate should model two distinct range-5 uncached allocator classes and place each resource according to the matrix above. A live shader launch remains gated on a successful build and static allocation audit.
