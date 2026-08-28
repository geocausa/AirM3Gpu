# G15 / 23J220 empty-Compute UMA tail

Status: exact-target static reconstruction. No Linux RunCompute is enabled by this note.

## Exact empty-path record values

The stock E068 empty Compute command supplies zero qwords for the UMA record values copied into descriptor `+0x640/+0x648`. Exact 23J220 `AGXUMAPool::prepareLocked()` reads those qwords for backing-pool sizing/validation but never rewrites them; on successful preparation it copies them to descriptor `+0x628/+0x630`. `AGXCLChannelSKU::submitBuffer()` copies those directly to RunCompute `+0x847/+0x84f`.

Therefore the exact stock empty path has:

- RunCompute `+0x847 = 0`;
- RunCompute `+0x84f = 0`.

These fields are not the return values of `getMinPoolSize()` / `getIdealPoolSize()`. Those policy functions size and validate backing memory.

## Page-Pool State at `+0x83e`

`AGXCLCommandDescriptor` embeds `AGXUMAData` at descriptor `+0x5e0`. The selected `AGXUMAPool` owns an `AGXUMAFList`; its `+0x1c0` member is Apple's dedicated 0x70-byte `UMA Page Pool State` GPU mapping. Successful preparation publishes that object's live address at AGXUMAData `+0x18` = descriptor `+0x5f8`. `submitBuffer()` translates that address to RunCompute `+0x83e`.

The already modeled Linux `G15UMAPagePoolState` layout is therefore the correct ABI object for Compute too. Its dependent fields include the Page Pool List, Backup Page List, FW Uncached State, hardware-buffer ID / descriptor-table index, and shared-compute flags. Linux still lacks the corresponding producer/lifetime/ID-manager path; a captured address must not be substituted.

## UMA hardware metrics at `+0x857`

`AGXChannel::init()` constructs channel `+0x190` as `AGXUMAHWMetrics`. `processCompute()` stores it at descriptor `+0x618`. `AGXUMAHWMetrics::init()` owns one mapped page and stores its GPU/FW base at object `+0x30`.

The CL descriptor allocator explicitly initializes descriptor `+0x620 = 0`, and the stock empty path does not rewrite it before submission. Exact `submitBuffer()` therefore exports the base metrics-page FW address to RunCompute `+0x857`.

Linux currently has no equivalent Compute UMA metrics owner, so this pointer must remain fail-closed until that mapped page and its lifetime are modeled.

## Context / completion boundary

The same exact descriptor allocator initializes UMA prepared state to zero; E071 proves successful UMA preparation changes the exported RunCompute `+0x846` value to `1`. Context generation at `+0x85f` is separately allocated and remains dynamic.

Linux also has no G15 HardwareBuffer-ID allocator yet, even though the 256-entry UMA Page Pool descriptor table is already modeled in InitData. Therefore making `+0x83e` nonzero without the ID lifecycle would be incomplete. Live RunCompute remains blocked on that ID/descriptor-table lifecycle, the metrics page, dynamic context generation, the exact SKU-stream producer at `+0x760`, and stamp/notifier completion/recovery ownership.
