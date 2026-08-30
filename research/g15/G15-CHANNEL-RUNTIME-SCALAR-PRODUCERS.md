# J615 channel runtime scalar producers (E154)

E154 closes the final three caller-provided scalars left after E153's real
selected-resource bridge, while keeping the first CL/Compute channel private and
unpublished.

Exact 23J220 kernel symbolication resolves the source of selected QueueInfo
`+0x50`. `IOGPUDevice::init()` calls `_get_bsdtask_info(task)` followed by
`_proc_pid(proc)` and stores the 32-bit PID at `IOGPUDevice +0x60`.
`IOGPUCommandQueue::init()` copies that value to queue `+0x490`,
`AGXChannel::init()` copies it to channel `+0x4c`, and
`resetChannelState()` writes it to selected QueueInfo `+0x50`. Linux checkpoint
`1e74793c7d14144b70f1c5e383e25fe1397eba08` therefore captures the DRM-opening
process PID once at per-client File lifetime and retains it in G15 Queue state.

The remaining priority branch is also exact. `IOGPUCommandQueue::updatePriority()`
calls `IOUserClient::clientHasPrivilege(task, "foreground")`; normal queue
construction seeds `{+0x444=1,+0x448=2}`. The not-foreground branch selects
priority 2. Exact AGX propagation uses integer argument 2, and
`chooseCLWorkQueue()` supplies literal QoS 2 whenever effective priority is not
1. Thus the mechanically closed non-foreground first-CL tuple is
`(effective_priority=2, integer_arg=2, qos_arg=2)`.

Linux has no direct IOGPU `foreground` entitlement model. E154 therefore does
not invent one: the zero-caller private helper is explicitly renamed
`g15_assemble_unpublished_nonforeground_compute_channel()` and derives the PID
and non-foreground priority/QoS internally. The old `G15ComputeChannelInitInputs`
bag is deleted. Foreground parity remains a later policy bridge.

`queue/compute.rs`, `fw/compute.rs`, and `workqueue.rs` are unchanged. The E075
G15 runtime gate remains closed, no module is installed, and no RunCompute or
other custom GPU command is issued.
