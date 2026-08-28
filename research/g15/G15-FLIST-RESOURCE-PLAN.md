# G15 FList resource plan

E081 combines the exact E077 symbolic FList geometry with the synchronized E080 HardwareBuffer owner while remaining incapable of GPU allocation. `G15FListGeometry` requires the future mechanically proven `M = pool+0x48`, `B = pool+0x50`, and host page size `P`, then computes Page Pool List bytes as `align_up(M >> 9, P)`, Page Pool List entries as bytes/8, and Backup Page List bytes as `align_up((M / B) * 64, P)` with checked arithmetic.

The fixed Page-Pool State and FW-Uncached State sizes are compile-time tied to the already-proven 0x70- and 8-byte raw ABIs. `G15FListResourcePlan` contains no `GpuObject`, `GpuArray`, allocator or mapping-class choice, so constructing it cannot create or publish GPU memory. Linux commit `affdd1fba79d` is compile-only.

Next boundary: mechanically close the FList Page Pool List / Backup Page List range-5 SecureGart protection class before a real resource owner can be defined.
