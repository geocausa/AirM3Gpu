# G15 HardwareBuffer-ID synchronized owner

E080 wraps the exact E079 0x100-entry HardwareBuffer-ID state machine in the same `Arc<Mutex<...>>` pattern already used by the Asahi driver and adds an FList-side sticky binding owner. The wrapper serializes allocation/completion state and preserves the binding across zero-reference periods, exposing the exact first/final-reference booleans used by Apple's prepare/complete callbacks.

Linux commit `ed17ac035ad2` deliberately creates no runtime instance in GpuManager, InitData, queues or an FList object. It therefore performs no callback, GPU allocation, Page-Pool descriptor publication or RunCompute submission.
