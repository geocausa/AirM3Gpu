# G15 selected channel-state gpu-buffer pointer

E121 closes one more exact macOS 14.8.3 / 23J220 selected-channel-state source without changing the live Linux queue path.

After `_AGFIChannelState` slot selection, exact `AGXChannel::init()` derives channel `+0x88` as selected-state GPUVA plus **0xb0**. Exact `AGXChannel::resetChannelState()` later converts channel `+0x88` to firmware address form and writes it at QueueInfo `+0x18`. The offset is exactly the proven G15 QueueInfo size 0xb0, so this pointer starts immediately after QueueInfo inside the same selected 0x24c0-byte state slot. This result does not infer the semantic size of the remaining in-slot region.

Linux commit `563b93f4bce3bb8d1ddfb12f9737acf99905fa71` removes `gpu_buf_fwva` from the private selected-state reset inputs, adds exact `G15_CHANNEL_STATE_GPU_BUF_OFFSET = 0xb0`, compile-time ties it to the G15 QueueInfo size, and derives QueueInfo `+0x18` from the selected slot address. The existing live `WorkQueue::new()` allocation model is deliberately unchanged.

Validation: tree `b51a0dee9e73cd2f4960a4ac93a3d66f8cbdeeb4`; module SHA-256 `091ed2873c01ac2ad11df958f5d0e672c61b90dc311ccc22e8d940d0dbc3d9e4`; exact established 24-individual-warning baseline; strict checkpatch 0/0/0; independent source/offset audit PASS. Patch 0039 reconstructs the exact tree from E120.

The selected-state token remains private and has no SKU or RunCompute conversion. No module was installed and no RunCompute or other custom GPU command was issued.
