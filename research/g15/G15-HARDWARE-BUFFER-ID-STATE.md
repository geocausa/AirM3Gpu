# G15 HardwareBuffer-ID state machine

E079 models the exact 23J220 `AGXHardwareBufferIDManager` allocation state compile-only. G15/G15G initializes 0x100 IDs; FList `+0x10`, Page-Pool State `+0x08`, and the RTKit UMA descriptor-table index all use the same assigned ID.

The host manager keeps sticky object identity, a u32 reference count per ID, a 256-bit zero-reference bitmap, and a free-ID LIFO stack. Initial stack layout makes fresh allocations return IDs 0 through 255. After that stack is exhausted, the lowest set bit in the bitmap can steal a dormant sticky ID. Sticky reuse increments the same ID's refcount when manager owner and object identity still match. The `first_reference` condition is exactly the 0→1 transition passed to `prepareBufferResources()`; final completion is the 1→0 transition that invokes `completeBufferResources()`.

Linux commit `865f24f2a9fc` reconstructs this state machine with a nonzero owner cookie standing in for Apple's object pointer. It intentionally does not instantiate or synchronize the manager yet, so it cannot affect runtime or publish a Page-Pool State pointer.
