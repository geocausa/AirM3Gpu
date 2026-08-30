# J615 normal Compute shared-pool class (E146)

E146 closes the shared-UMAPool priority-class input for the ordinary J615 CL/Compute path. Exact `AGXChannel::init()` copies command-queue effective context priority `+0x44c` to channel `+0x1e8`, then maps context types 0 or 5 to shared-pool class 0 and every other type to class 1.

Matching IOGPU/AGX evidence proves the normal command queue begins at effective type 3 and ordinary runtime updates choose type 1 or 2. The normal J615 CL/Compute path therefore always selects shared Compute class 1, i.e. the second Compute slot in the four-slot client container. This class is separate from the firmware channel-state priority class and from the CL `evctl_index`.

Linux removes the raw class parameter from the dormant E145 pool/assembly bridge and supplies `G15_J615_NORMAL_COMPUTE_POOL_PRIORITY_CLASS = 1` internally. The assembly helper remains zero-caller, `queue/compute.rs` is unchanged, and no live pool creation or RunCompute is enabled.

Linux checkpoint: `b06c8650159bf700c3d4766542da8c976ef19048`, tree `604d859c910c383246f3316c9de1d5fe480388eb`.
