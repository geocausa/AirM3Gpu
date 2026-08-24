# G15 TA command body — J615 / G15G C0 closure

Primary evidence:
- `AGXTAChannelSKU::submitBuffer()` at `0xfffffe0008ea0b20..0xfffffe0008ea151c` (`g15-ta-submit-decomp.log`).
- G15 TA channel vptr from `AGXTAChannelG15::MetaClass::alloc()` = `0xfffffe0007c2eaa0`.
- vslot `+0x218` resolves to `AGXTAChannelG15::generatePreparseBarrierRegister()` at `0xfffffe0008e744b0`, whose body is only `bti; ret` (`g15-ta-vslot218.log`).
- `processRenderSetup()` and raw Render mappings remain canonical in `G15-TA-REGISTER-LIST.md`.

## Exact command-body geometry

- RegisterArray occupies `+0x040..+0x74f`.
- `+0x750..+0x75f`: unwritten 0x10-byte packed gap.
- `+0x760`: TPC pointer.
- `+0x768`: TPC size.
- `+0x770`: microsequence pointer.
- `+0x778`: microsequence size.
- `+0x77c`: fragment stamp slot.
- `+0x780`: fragment stamp value.
- `+0x784`: explicitly zero.
- `+0x788/+0x790/+0x798/+0x7a0/+0x7a8`: descriptor `+0xcf0/+0xcf8/+0xd00/+0xd08/+0xd80`, exactly raw Render `+0x10/+0x18/+0x20/+0x28/+0x60`; Linux now backs the G15 `+0x7a8` source with the Scene-owned 0x8e0-byte context-switch allocation (`base + 0x280`) while RegisterArray emission remains blocked.
- `+0x7b0/+0x7b8/+0x7c0`: explicitly zero.
- `+0x7c8..+0x80f`: unwritten 0x48-byte packed gap.
- `+0x810..+0x82f`: explicitly zeroed by two 16-byte stores.
- `+0x830..+0x83b`: unwritten 0x0c-byte packed gap.
- `+0x83c`: zero-extended descriptor byte `+0x6c0` = raw Render `+0x60c`.
- `+0x840`: descriptor qword `+0x6c4`, packed from raw `+0x608/+0x610` state.
- `+0x848`: descriptor dword `+0x6cc` = raw `+0x614`.
- `+0x84c`: zero-extended descriptor `+0xe08 = (raw Render[0x1bc] != 0)`.
- `+0x850/+0x854/+0x858/+0x85c`: descriptor `+0xe0b/+0xe0c/+0xe0d/+0xe0f`.
- `+0x860`: G15 JobMeta head; J615/G15G C0 `engine_state` is exactly zero (`g15-ta-engine-state-proof.txt`).
- `+0x888`: queue-local event sequence.
- `+0x88c`: `(raw Render[0x619] == 2)` via descriptor `+0x7b0`.
- `+0x890` is passed as `AGFIBarrierState` to vslot `+0x218`; the G15 implementation is a no-op.
- `+0x890..+0x8a7`: submit itself writes nothing; Linux keeps the first qword addressable for the compile-only scaffold and zeroes the remainder.
- `+0x8a8`: explicitly zero.
- `+0x8b0`: converted SegmentResourceList FWVA.
- `+0x8b8`: converted block-fence first time-slot FWVA under its exact descriptor gate.
- `+0x8c0/+0x8c8`: two U64 outputs from `AGXMTLCounterSampler::fwTokenEncode()`.

Late UMA/context/timing state at `+0x8d0..+0x91f` is already closed separately.

## Linux structural commits in this pass

- `859169243135` — document exact J615/C0 TA engine-state zero gate.
- `8ba3d4cc7b5c` — correct `+0x84c` to raw Render `+0x1bc` nonzero boolean.
- `d1b87292e451` — split `+0x7c8..+0x83b` into unwritten/explicit-zero regions.
- `7be35d67cfe7` — classify the no-op barrier-adjacent `+0x898..+0x8a7` region as padding.
- `d347404888c1` — classify `+0x750..+0x75f` as unwritten pre-TPC padding.

All changes remain compile-only/fail-closed. G15 RegisterArray emission remains empty and runtime activation remains blocked.
