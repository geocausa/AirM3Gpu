# G15 23J220 Compute SKU source map

E070 moves the G15 Compute SKU-stream proof onto the exact macOS 14.8.3 / 23J220 ABI used by the J615 Linux target.

No Apple binary or raw disassembly is published here, and no Linux RunCompute was issued.

## Exact target identities

Matching `AGXG15G.kext` CFBundleVersion 282.14.2:

`e29327fd1eeec53ea47bba91572d393cd6bc38ab77b9ac3c9ab62cc70f94854b`

Exact target functions retained privately include:

- `AGXCLChannelG15::encodeCLCommandSKUStream()` @ `0x80a6c`, size `0x68c`, SHA256 `0e368f85b5411a982c721e78a238eb194ebad604b165ad23a6a34610c6663287`;
- `AGXCLChannelG15::encodeComputeWFI()` @ `0x810f8`, size `0x20`, SHA256 `60f30f184711499ca738305101cd821f732ec6ea2f04309ad58d2160ab6effcc`;
- `AGXCLChannelSKU::submitBuffer()` @ `0x362d4`, size `0x724`, SHA256 `120ff0cc89acfeaffac930eac51a15ba4bebfec105e33e5d8185d02359cad8de`.

## Exact same-ABI grammar

The 23J220 G15 encoder independently confirms:

1. optional 0x14-byte performance-counter packet `0x90000004`;
2. opcode `0x0000000b` plus exact `0x1b8` payload;
3. 0x3c-byte start timestamp record (`0x80000003`);
4. Compute WFI dword `0x00000001`;
5. optional paired 0x14-byte packet `0x10000004`;
6. 0x3c-byte end timestamp record (`0x00000003`);
7. trailing opcode `0x0000000c`, 0x7c bytes total;
8. finish dword `0x40000002`, then 0x40-byte size rounding.

No optional pair: `0x2b8 -> 0x2c0`. With the pair: `0x2e0 -> 0x300`.

## Submit ordering and command-relative pointers

Exact `AGXCLChannelSKU::submitBuffer()` calls G15 register generation first and SKU-stream encoding second. The latter receives descriptor `+0x350` as the GPU/FW address of the 0x880 command.

For Compute/type 3, the 0x3c timestamp packet uses command-relative addresses at `+0x810`, `+0x818/+0x820`, optional user timestamps at `+0x828`, and `+0x868`. These offsets already match the independently reconstructed Linux `RunComputeG15V14_7` layout.

The opcode-0xb and opcode-0xc payloads are populated from command, descriptor, channel and allocator state. They must be reconstructed source-by-source; this checkpoint does not treat unlabelled bytes as arbitrary padding.

## Optional pair is PerfCtr state

Exact 23J220 `AGXAcceleratorG15G::start()` allocates accelerator `+0x2488` as `AGXPerfCtrSamplerGen15`. Base sampler init explicitly clears byte `+0x54`, while later source-sampler lifecycle changes it. Therefore the optional `0x90000004/0x10000004` pair belongs to performance-counter state rather than mandatory Compute framing.

E071 closes the ordinary inactive/default path: the relevant sampler feature state starts zero, the stock E068 empty descriptor supplies the corresponding feature input as zero, and the exact payload builder preserves that zero. The stock empty-Compute path therefore uses the no-feature `0x2c0` stream; the optional pair is absent. This is an empty/inactive-path proof, not a claim that performance-counter-enabled Compute can never use `0x300`.

## UMA prepared-state handoff

Exact 23J220 `AGXCLCommandDescriptor::prepare()` reaches UMA preparation before normal submission. After successful `AGXUMAPool::prepareLocked()`, `AGXCLChannelSKU::submitBuffer()` copies descriptor byte `+0x624` directly into RunCompute byte `+0x846`. The stock prepared empty-Compute path therefore requires `+0x846 = 1`.

Linux previously initialized the compile-only G15 field to zero. Commit `f73b9e551658` corrects that single value to `1`. The exact empty-path zeros at RunCompute `+0x810`, `+0x860`, `+0x870`, and `+0x878` remain unchanged, while context-generation byte `+0x85f` remains dynamic and must not be hard-coded.

## Linux consequence

The current compile-only Linux G15 RunCompute layout is correct at `+0x760`, but its inherited producer still supplies the older StartCompute/WaitForIdle/FinalizeCompute microsequence. Exact G15 Apple code instead publishes the SKU execution stream there.

The next source step is therefore a separate compile-only G15 SKU backing/builder. Ordinary G15 userspace submission remains fail-closed. E071 closes the inactive empty-path PerfCtr predicate and prepared byte, but a live RunCompute remains gated on the exact UMA Page Pool State/FW-uncached backing and min/ideal/metrics values, dynamic context-generation/selector, stamp/notifier sequencing and completion/recovery ownership.
