# J615 client UMA Queue context bridge (E143)

E143 carries the exact E139-E142 client shared-pool context through Linux Queue lifetime without selecting or constructing a pool.

Exact 23J220 `AGXChannel::init(AGXCommandQueue const*, ...)` loads the owning command queue's shared object at `+0x488` and, on the shared UMA path, dereferences that object's `+0x1b8` `AGXUMASharedPoolContainer`. The same client shared object is therefore retained across command-queue lifetime and consulted later when the channel is actually constructed.

Linux `file::Vm` already owns the exact client-address-space pieces: both range-5 allocators and the E140-E142 shared-pool container. E143 clones that container at `queue_create()`, passes it through `GpuManager::new_queue()`, and stores it privately in G15 `QueueInner`.

This is context retention only. There is no Queue accessor to the E142 selection/create transaction, no `G15SharedComputeUmaPoolOwner` constructor call, no global pool-ID consumption and no Compute command producer change. In particular, E143 does not attach pool creation to the currently eager generic `WorkQueue::new()` path; E138 proved Apple's actual TA/3D/CL channel/pool construction is lazy, so that equivalence would be unjustified.

Linux checkpoint: `848a9d426b2d1bb25a3f917ed37b15300b6d7f53`, tree `198d50e59b1e8a238d8ff39d49f6627a118f05cf`.
