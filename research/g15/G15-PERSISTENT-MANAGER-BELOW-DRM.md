# J615 persistent manager below DRM

E157 retires the obsolete E075 probe-level stop only far enough to exercise the current persistent J615 manager bootstrap. The bounded E075 range-7/range-8 UAT preflight still runs and tears down first; the restarted persistent path then constructs `GpuManagerG15V14_7`, boots RTKit, sends G15 `MSG_INIT`, waits for q21 `firmware_ready == 1`, and activates E152 q22 only from an empty ring. DRM registration is deliberately omitted, so File/VM/Queue/QueueInfo/channel/submission paths remain unreachable.

This matters because E147-E152 materially changed the pre-RTKit manager graph after the older E031-E033 persistent-runtime tests: five accelerator-global firmware-resource stacks now own real eager mapped backings with E151 bootstrap-silent q22 policy. E157 is the first live proof that this newer graph is accepted by the exact J615 firmware through the q21-ready/q22-active boundary.

The one-shot candidate `7.1.6-gc6fa9b794ba9` reached the exact marker `T8122 G15 E157 persistent manager + RTKit + q21-ready/q22-active PASS; DRM registration blocked` and remained idle for more than two minutes. There was no panic, Oops, SError, DART/IOMMU/GPU fault, firmware/RTKit crash, MTR alarm, watchdog or BUG marker. `/dev/dri/renderD128` never appeared, while the Asahi platform driver remained bound. No post-activation q22 pressure/publication message was observed because no DRM Queue could exist.

E156's lazy CL-channel ensure remains zero-caller, and `queue/compute.rs` plus `fw/compute.rs` are unchanged. No RunCompute or custom GPU command was issued.

The one-shot GRUB entry was consumed and the machine returned to the golden kernel. The sacrificial candidate module and initrd were restored byte-for-byte to their pre-E157 SHA-256 values. The next staged boundary is DRM registration and passive client/VM/Queue lifecycle with ordinary G15 submission still fail-closed; it is not a RunCompute enablement.
