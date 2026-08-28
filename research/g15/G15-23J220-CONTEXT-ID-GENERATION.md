# G15 / 23J220 context-ID and generation lifecycle

Status: exact-target static reconstruction plus compile-only Linux implementation. No Linux RunCompute is enabled by this note.

## Managed G15 context IDs

The exact macOS 14.8.3 / 23J220 G15 driver uses a 64-entry `AGXContextIDManager`. ID 0 is reserved; normal client contexts use IDs 1..63. The manager keeps an 8-bit generation byte per context ID, initialized to zero.

On a fresh assignment, Apple registers the selected context ID with the GART first. Only after that registration succeeds does it increment the ID's generation byte. Reacquiring the same still-owned GART/context mapping reuses both ID and generation without incrementing. If the ID is later reassigned, the byte increments again and naturally wraps at 8 bits.

`AGXCLCommandDescriptor::prepare()` stores the selected ID at descriptor `+0x490` and its generation at `+0x494`. Exact `AGXCLChannelSKU::submitBuffer()` exports those to RunCompute `+0x10` and `+0x85f` respectively.

## Linux convergence

Linux already had the right context-ID machinery rather than needing a second Apple-style manager:

- UAT has 64 hardware context entries;
- ID 0 is reserved and users receive 1..63;
- `Uat::bind()` publishes the G15 two-qword GPTBAT entry with the selected context ID;
- the sticky `SlotAllocator` already distinguishes unchanged reacquisition from slot reassignment.

Therefore existing `vm_bind.slot()` is already the correct G15 RunCompute `+0x10` context ID. The missing state was only the generation byte.

Linux commit `03fdbb86230f` adds a generation byte to each UAT slot. For G15, a changed slot increments that byte only after GPTBAT publication succeeds. Sticky reuse preserves it. `VmBind` snapshots the paired generation, and the G15 Compute encoder writes it to RunCompute `+0x85f`.

This produces the same initial behavior as Apple: a zero-initialized user slot's first successful assignment exports generation `1`.

## HardwareBuffer-ID lifetime

The same exact-target pass also closes the shape of the separate UMA HardwareBuffer-ID manager. G15/G15G initializes it with **0x100 IDs**, matching the already reconstructed 256-entry Page-Pool descriptor table.

`AGXUMAPool::prepareLocked()` obtains the ID for the selected FList. A fresh allocation invokes the FList resource-preparation callback, preparing the Page Pool List, 0x70-byte Page-Pool State, FW-Uncached State, Backup Page List, and grow mappings. Final HardwareBuffer-ID completion invokes the matching completion callback and releases those mapping preparations.

Consequently a constant HardwareBuffer ID or a naked nonzero RunCompute `+0x83e` pointer would be wrong. Linux still needs the corresponding FList mapping owner/ID lifecycle before that pointer can become live.

## Validation

- Linux base: `f73b9e5516589fde5820ea487911fd830fac958c`
- Linux checkpoint: `03fdbb86230f1c431bb6958dfa6bc752ad35b1cd`
- checkpoint tree: `999f7f6cc80a2e5cfd92c8de88dbc1a05d6c3de3`
- `git diff --check`: PASS
- strict checkpatch: 0 errors, 0 warnings, 0 checks
- Asahi module build: PASS
- vermagic: `7.1.6-m3-gpu-e073-context-gen SMP preempt mod_unload aarch64`
- module SHA-256: `a54dee9c2b3a435c7f3c5fcebb33cde71dacf81b0b1af4df846851631f97b376`
- patch 0007 exact-tree reconstruction: PASS

## Remaining live boundary

Dynamic Compute context generation is now closed in Linux, but live RunCompute remains fail-closed. The remaining hard requirements include the FList-owned Page-Pool State and its HardwareBuffer-ID/mapping lifetime at `+0x83e`, the channel-owned HWMetrics mapping at `+0x857`, the exact G15 SKU execution-stream producer at `+0x760`, and stamp/notifier completion and recovery ownership.
