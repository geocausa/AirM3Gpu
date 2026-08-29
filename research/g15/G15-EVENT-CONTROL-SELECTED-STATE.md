# G15 event-control selected-state seed — E094

E094 closes the normal-J615 numeric image written when exact macOS 14.8.3 / 23J220 rotates to one of `AGXCommandBuffer`'s 36 event-control states. The 23J220 oracle remains the ABI source of truth; older public Asahi structures are not used for offsets.

`AGXCommandBuffer::nextCommandBufferState()` computes event-control `+0x10` as `cap == 0 ? record_count : min(record_count, cap)`, where `record_count` is accelerator `+0x2420` or the G15 fallback `+0x678`, and `cap` is accelerator `+0x2428`. Normal J615 leaves `+0x2420` and `+0x2428` zero, while G15 configure writes `+0x678 = 0x50`, so selected event-control `+0x10` is exactly **80**. The deeper firmware meaning remains deliberately conservative: effective record count/cap.

A whole exact-binary writer audit found `+0x2428` only at that read; no raw offset-table entry exists and base/G15/G15G configure plus accelerator start do not write it. The prior zero-filled accelerator allocation proof therefore closes the normal-target zero.

The exact RTKit-2419 type-3 handler at `0xfffffc00000251e8..0x256db` independently proves RunCompute `+0x14` is active scheduler metadata: the firmware copies that FWVA into the per-stamp execution record at `+0x28/+0x30`. It does not dereference event-control `+0x10` there, so no stronger inner semantic is claimed.

Linux commit `42bca8d2e9dc` adds only an unreachable selected-state seed helper. After the prior event is known retired, it clears the matching stamp and complete 0xc0 block, restores stamp FWVA, caller-provided stamp index, exact J615 `+0x10 = 80`, and `+0xa8 = -1`. It exposes no selected FWVA, has no call sites, and G15 RunCompute `+0x14` remains literal zero/fail-closed.

Validation: exact tree `a492ce367bee16fab63aa8c80f465b83614655d8`, module SHA-256 `617064c5fa58080aa45e03d9d3a5166a4b1554338a3d65d8c94071656f4d5b1e`, sacrificial-kernel vermagic `7.1.6-gc6fa9b794ba9 SMP preempt mod_unload aarch64`, exact existing 24-warning baseline, strict checkpatch 0/0/0. No module install and no RunCompute.

The next boundary is exact event-slot reuse: `IOGPUEventMachine::finishEvent()` is called on the rotated slot before its stamp/control storage is reset. That ownership must be modeled before a selected event-control FWVA can be published.
