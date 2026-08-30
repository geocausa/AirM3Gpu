# G15 stock-empty pre-micro raw-Compute sources — E133

E133 closes the remaining stock-empty RunCompute command-body values at `+0x740/+0x748/+0x750` and corrects an older source attribution. This is exact macOS 14.8 / 23J220 static reconstruction plus a compile-only Linux semantic correction. No module was installed and no RunCompute/custom GPU command was issued.

## Corrected exact kernel source chain

An older reconstruction stopped at `AGXCommandQueue::processComputeSetup()` and treated the temporary register used by its pre-micro snapshot as `AGXCommandQueue this`. Replaying the exact callers shows that alias was wrong.

`AGXCommandQueue::processSegmentKernelCommand()` allocates and parses the `0xe0` `AGXComputeHardwareKernelCommand` wrapper using exact `AGXComputeHardwareKernelCommand::parseAndValidate()` at `0xfffffe0008e344ec`. The Compute dispatch passes that parsed wrapper in `x2` to `AGXCommandQueue::processCompute()`.

Exact `processCompute()` then carries incoming `x2` through `x23` and calls `processComputeSetup()` with `x4 = x23`:

- `0xfffffe0008e2cd54 mov x23,x2`
- `0xfffffe0008e2ce28 mov x4,x23`
- `0xfffffe0008e2ce34 bl 0xfffffe0008e2cef8`

The `processComputeSetup()` prologue saves `x4` at stack `+0x40`; immediately before the snapshot it reloads that same value into `x19`:

- `0xfffffe0008e2cf24 str x4,[sp,#0x40]`
- `0xfffffe0008e2d5e8 ldr x19,[sp,#0x40]`
- `0xfffffe0008e2d60c ldp q0,q1,[x19,#0x20]`
- `0xfffffe0008e2d610 stp q0,q1,[sp,#0x100]`
- `0xfffffe0008e2dab4 ldr x8,[sp,#0x100]`
- `0xfffffe0008e2dab8 str x8,[descriptor,#0x3f0]`
- `0xfffffe0008e2dabc ldr d0,[sp,#0x118]`
- `0xfffffe0008e2dac0 str d0,[descriptor,#0x3f8]`

Thus the snapshot source is the parsed Compute wrapper, not queue state.

## Raw Compute mapping

Exact `AGXComputeHardwareKernelCommand::parseAndValidate()` independently maps:

- raw Compute `+0xc0` -> wrapper `+0x20`;
- raw Compute `+0xd8` -> wrapper `+0x38`.

Combining this with the already-proven `submitBuffer()` export gives:

| Exact source | RunCompute | Width |
| --- | --- | ---: |
| raw Compute `+0xc0` | `+0x740` | 64-bit |
| raw Compute `+0xd8` low | `+0x748` | 32-bit |
| raw Compute `+0xd8` high | `+0x750` | 32-bit |

The earlier `AGXCommandQueue +0x20/+0x38` attribution is therefore superseded.

## Exact same-build stock-empty values

E133 reuses the E132 matching 23J220 `AGXMetalG15G_C0` oracle, SHA-256 `d262a98d865cde1d9af8df0ed318bd5109efcfbc940968d8ef9fa8402565ae88`.

Exact `AGX::ComputeContext::beginComputePass()` zeroes the whole raw `0x1d0` payload and then explicitly writes:

- `0x1d9879624 str xzr,[raw,#0xc0]`
- `0x1d9879628 str xzr,[raw,#0xd8]`

The complete exact no-dispatch `endComputePass(impl, 0, 0x16)` raw-pointer write audit contains no later store to either offset. Therefore exact stock-empty 23J220 has:

- RunCompute `+0x740 = 0`;
- RunCompute `+0x748 = 0`;
- RunCompute `+0x750 = 0`.

The retained newer-userspace capture is deliberately not used for these values; its raw `+0xc0` differed, again demonstrating why the same-build rule matters.

## Linux semantic correction

Linux E131 already emitted zero at all three locations, so patch 0049 changes no command bytes. Commit `f09bed89530e5fa482240c55653545a3bed3e292` only corrects the names/comments and preserves hard offset locks:

- `g15_queue_state_20_740` -> `g15_raw_compute_c0_740`;
- `g15_queue_state_38_lo_748` -> `g15_raw_compute_d8_lo_748`;
- `g15_queue_state_38_hi_750` -> `g15_raw_compute_d8_hi_750`.

Strict checkpatch is clean, and the established external Asahi module build passes at the existing 24-individual-warning bring-up baseline. Module SHA-256 is `b4f6dddbaa80a53ee853ac18d424f6e91cb13969849ff50912c26eba13a2444f` with vermagic `7.1.6-gc6fa9b794ba9 SMP preempt mod_unload aarch64`.

E133 closes the last stock-empty active command-body source called out by E112/E132. The remaining work is integration/lifetime ordering: the dormant two-phase asset/SKU transaction still has no live RunCompute writer and remains fail-closed.
