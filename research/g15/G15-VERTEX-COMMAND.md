# G15 / RTKit-2419 Vertex (TA) command reconstruction

Exact host/FW target: AGXG15G + RTKit-2419.140.12.

Mechanically established command geometry:

- total command size: `0x920`
- register-list array begins at `+0x40`
- register-list metadata: `+0x740` pointer and `+0x748` descriptor word
- TPC pointer/size: `+0x760/+0x768`
- SKU/microsequence pointer/size: `+0x770/+0x778`
- fragment stamp slot/value: `+0x77c/+0x780`
- command-local field: `+0x784`
- G15 encoder takes `command_fwva + 0x84c`
- `JobMeta` begins at `+0x860`
- `unk_after_meta` equivalent lands at `+0x88c`
- G15 preparse-barrier hook receives `command +0x890`; exact G15 implementation is BTI+RET
- G15 encoder takes `command_fwva + 0x8f8`
- firmware directly consumes an unaligned pointer at `+0x8d6` and companion byte `+0x8de`
- firmware has active state through at least `+0x918`
- exact object/ring stride is `0x920`

G15 TA WFI emits literal opcode `1`, so G15 is hybrid: register-list command body but the older WaitForIdle opcode. Linux compile-only scaffolding therefore does not inherit G14X microsequence semantics wholesale.

The kernel shell intentionally leaves unclosed active G15 regions opaque and emits no G15 register-list entries yet. Runtime submission remains fail-closed.
