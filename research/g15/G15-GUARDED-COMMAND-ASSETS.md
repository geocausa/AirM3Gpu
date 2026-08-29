# G15 guarded unpublished stock-empty Compute assets

E110 integrates the exact event-control and SKU slot retirement guards with the private E109 asset materializer while keeping the whole path unreachable from Queue submission.

Linux commit `63bb6ffe4f22abcd86db40b86e349a5aaea99bc2` requires an already-armed submission `JobFence`, keeps a busy selected event-control slot as the retry target instead of skipping it, binds the selected event slot, performs the exact next-slot SKU scan/bind, and only then materializes the private event/SKU/Page-Pool/HWMetrics/HardwareBuffer token. Fresh guard bindings are rolled back on selection/materialization failure; FList reference rollback remains owned by E109.

The wrapper has zero Queue/submission call sites. RunCompute `+0x14/+0x83e/+0x857` stay zero and `+0x760` remains unchanged. Validation: tree `3878c44828b1e20edb26d7ce98a3ca9ea8fdd84a`; module SHA-256 `7482c77f7883530ebb6130baf72173a962c54a07bf77aefe57e6e603e945ce68`; exact 24-warning baseline; strict checkpatch 0/0/0. No install or RunCompute.
