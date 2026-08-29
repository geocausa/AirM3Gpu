# G15 event-slot retirement guard — E096

Linux commit `4ff63937d4fcc1c4afc9b52c4e5cf1240f049716` models the exact E095 reuse lifetime without making the G15 event-control owner reachable.

The provisional event-control `+0x08` name is corrected to `state_sequence_08`. A separate host-only 36-slot guard set retains the existing per-submission `JobFence`: an unarmed fence cannot be bound, an occupied slot cannot be overwritten, and `try_finish()` keeps the slot unavailable until the fence's pending-command count reaches zero. This is a conservative nonblocking Linux equivalent of Apple's blocking `finishEvent()` barrier.

The GPU-visible range-7 stamp/range-8 event-control backings remain compile-only and unreachable. No selected event-control FWVA is exported and G15 RunCompute `+0x14` remains literal zero.

Validation: exact-release module build PASS at the established 24-warning baseline; module SHA-256 `7368ee7ea5959ba0ac47edfcfd1d6bdf12e18859c12d39222d1c8d98f6e99391`; strict checkpatch 0/0/0; patch 0023 exact-tree reconstruction PASS.
