# J615 global selected firmware-resource state (E153)

E153 removes the final E127-era duplicate selected-resource backing layer and
moves the already-proven J615 reset images onto the real accelerator-global
mapped resource leases established by E147-E152.

The remaining timestamp-mode ambiguity is closed against exact 23J220 userspace
and kernel code. An ordinary Metal command queue creates a fresh
`MTLCommandQueueDescriptor`, whose `_openGLQueue` byte is explicitly zero. Metal
copies that zero to the queue, `IOGPUMetalCommandQueue` passes it as argument 3
to `IOGPUCommandQueueCreateWithQoS`, and that routine stores it directly at
`IOGPUDeviceNewCommandQueueArgs +0x404`. Kernel `IOGPUCommandQueue::init()` maps
that byte to queue `+0x439`, which `AGXCommandQueue::init()` passes to
`AGXTimeStampQueue::setUpdateMode(bool)`. Thus the ordinary stock path calls
`setUpdateMode(false)` exactly and its selected timestamp state has `+0x10 = 0`.

Linux checkpoint `4a72f743dd25bcd3d50d77722c153d395a23dbad` therefore initializes the real
selected timestamp and scheduler resources once at Queue construction lifetime.
The still-unpublished lazy CL/Compute-channel constructor selects and initializes
the real global uncached-memory, cached-memory, and channel-state resources,
using the Queue-owned timestamp/scheduler FWVAs. The three channel leases remain
owned for the full unpublished channel lifetime.

Deleted are the five old local backing owners, all five local slot inputs, and
the per-command `timestamp_update_mode_2` input. Command phase 1 no longer
selects or resets any of those global resources. Its channel-state pointer comes
from a typed initialized-channel token instead.

The remaining dormant channel-construction scalar inputs are only
`channel_4c_value`, `effective_priority`, and `queue_qos`; E153 intentionally
does not guess them. `queue/compute.rs`, `fw/compute.rs`, and `workqueue.rs` are
unchanged, the retained E075 hardware gate remains closed, and no module was
installed or RunCompute issued.
