# J615 CL WorkQueue / channel lifetime (E155)

Exact 23J220 makes the lazy CL channel an owned child of the CL WorkQueue. `AGXCommandQueue::chooseCLWorkQueue()` calls the J615 `newCLChannel` accelerator factory and immediately stores the returned channel at `AGXCLWorkQueue +0x1e8`. `AGXCLChannel::init()` forwards that WorkQueue into `AGXChannel::init()`, which stores the reverse lifecycle backpointer at channel `+0x1d8`.

Teardown is explicitly ordered. `AGXCLWorkQueue::finalize()` locks channel `+0x1e0`, clears channel `+0x1d8`, unlocks, then chains to `IOGPUWorkQueue::finalize()`. `AGXCLWorkQueue::free()` releases the channel and clears WorkQueue `+0x1e8` before inherited WorkQueue teardown. The `chooseCLWorkQueue()` failure path performs the same nested channel-first then WorkQueue release sequence.

That ordering matters for Linux because `SubQueueJob`, `workqueue::Job`, and the event manager can retain `Arc<WorkQueue>` independently of the parent `SubQueue`. A channel owner stored only beside `SubQueue.wq` could therefore die too early.

Linux checkpoint `440e026d0d9c3a04555a9f9446bf35cd1a364e15` creates one empty G15 Compute-channel slot and also retains an opaque clone as the first field of the Compute WorkQueue. Every WorkQueue Arc clone therefore preserves the future channel lifetime, and the final WorkQueue drop releases the channel anchor before base WorkQueue state. Vertex/fragment WorkQueues receive no anchor.

The slot remains `None` and has no setter. The E154 unpublished non-foreground channel assembler remains definition-only, Compute writer files are unchanged, and the E075 runtime gate remains closed. E155 models lifetime only; it does not create a CL channel or publish RunCompute.
