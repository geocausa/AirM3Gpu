# G15 Compute event-control FWVA — E089

Exact 23J220 E088 proves RunCompute `+0x14` is not a generic queue notifier pointer. `AGXCommandBuffer` owns 36 rotating 0xc0-byte event-control blocks, keeps parallel CPU/GPU address arrays for them, and copies the selected GPU address into the CL descriptor before G15 submission converts it to FWVA.

Linux had inherited the older-generation model in which one `fw::event::Notifier` object is allocated per DRM queue and every Compute command exports that same address. That lifetime is incompatible with the exact G15 producer even though the older public m1n1 `EventControl` concept helped identify what to investigate.

Linux commit `167c037a91a0b85fa10480b90e883de31fa88e0d` makes the command ABI generation-specific:

- non-G15 retains the legacy `notifier` pointer;
- G15 names the field `g15_event_control_fwva_14`;
- G15 writes zero until the exact rotating event-control owner exists;
- the exact 0x880 RunCompute size and `+0x14` offset remain compile-time asserted.

This is deliberately fail-closed. It removes a wrong-lifetime pointer without pretending the 36-state owner is already implemented. Exact-release module construction passes at the existing warning baseline; strict checkpatch and exact patch-tree reconstruction both pass.

No module was installed and no RunCompute was issued. Error/recovery ownership, FList first activation, HWMetrics, and the exact G15 SKU-stream producer remain separate blockers.
