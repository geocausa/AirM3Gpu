# G15 stock-empty RunCompute integration gaps

E112 is a static integration audit against the current Linux G15 Compute constructor and exact macOS 14.8.3 / 23J220 `AGXCLChannelSKU::submitBuffer()` ordering. It makes no Linux source change and does not issue a GPU command.

The E111 host-only stage is **not yet safe to write into RunCompute**. Exact Apple ordering requires the command backing to exist before the SKU stream is encoded because the SKU contains command-relative addresses; the selected SKU slot FWVA is also itself an encoder input. Current dormant Linux layers instead accept a prebuilt SKU stream before slot selection and before the command FWVA exists. A future integration therefore needs a two-phase select/prepare then command-aware finalize transaction.

A second gap is fence ordering. The E110 guards require an already-armed submission fence, while current `submit_compute()` constructs the command before `fence.add_command()`. Integration needs explicit early arm plus rollback, not a weaker guard.

E112 also identifies exact runtime sources that remain outside E111: converted firmware state, CL channel state, CL command-resource region base, GART soft-fault state, accelerator `+0x654` bit 7, and the JobMeta event-control index. Current G15 source additionally leaves Apple-active pre-SKU queue state at `+0x740/+0x748/+0x750` and several JobParameters2 fields at `+0x774..+0x7bc` zero. These must be closed from exact producers rather than guessed or copied from a cross-build capture.

The audit therefore blocks a direct E111 → RunCompute writer. The next safe source step is definition-only two-phase preparation/finalization with all still-unclosed inputs explicit. Live G15 submission remains fail-closed.

Private E112 result SHA-256: `af4de75d096791378edef0c96b7e78e1c96e65c2a8d877b2bca0cbb777cc6bea`. Mechanical audit SHA-256: `f148502308e6d07b92350f7fcaa03233b3007101698a7e4dc29c48959feb55cf`.
