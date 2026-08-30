# J615 passive Queue lifetime (E163)

E163 enables only command Queue create/destroy beyond the E162 unbound private-VM boundary. Special-object binding and every G15 `SUBMIT` remain rejected before Queue publication or command parsing.

The current Queue constructor creates private QueueInfo/WorkQueue state but does not call `GpuManager::bind_vm()`, `g15_publish_empty()`, the E156 lazy Compute-channel ensure, or any RunCompute writer. It does own the exact E153 timestamp/scheduler global leases and Queue-local PM shared-bank1 resources, so Queue creation/destruction exercises q22-tracked runtime map/unmap lifetime without publishing a firmware context or QueueInfo.

The authoritative final candidate completed an initial Queue lifecycle, 16 additional create/destroy cycles, and a final cycle after more than three minutes. A zero `SUBMIT` returned exact `ENODEV`. Firmware-visible context/QueueInfo markers, q22 error/pressure markers, and strict GPU/firmware/kernel fault evidence were empty. This bounded test demonstrates stable q22-backed Queue resource lifetime, but does not yet prove firmware advancement of q22 `read_idx`; that cursor-consumption proof remains the next boundary.

Linux checkpoint: `0e43b64df4bf8c2c1b56f45e8077232510159448`, tree `b27c301d33790e9d38dc4bf6268bc2fa8a4166fe`.
