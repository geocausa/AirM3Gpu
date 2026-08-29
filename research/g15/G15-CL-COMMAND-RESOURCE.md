# G15 CL Command Resource

E114 closes the exact macOS 14.8.3 / 23J220 J615 CL-channel command-resource geometry used by the stock-empty Compute SKU stream. `AGXCLChannel::init()` derives a per-region stride of `0xf400` from 10 cores and one MGPU, a one-MGPU span of `0x800`, and allocates one persistent logical `0x1f400`-byte `AGXResource`. The resource uses normal eGartRange 5 / option 3, the already-proven G15 uncached range-5 PTE class.

The same exact-target pass also closes two submit-side sources: Compute `JobMeta.evctl_index` comes from `AGXChannel +0x38`, and `flush_stamps` is the boolean `(descriptor +0x48 != 0)`. These are source facts only; E114 does not make them live in Linux.

E115 promotes only CL command-resource ownership into compile-only Linux. Commit `059d34701e480252829c6b397dad5ee2eb8881dc` adds an unreachable `G15ClCommandResourceBacking` using the dedicated range-5 uncached allocator, pins the `0xf400/0x800/0x1f400` geometry, and makes dormant SKU finalization derive `channel_command_region_base_fwva` from that owned resource instead of an external input. Exact-tree reconstruction passes.

The owner graph remains definition-only. No Queue/submission caller is added; live G15 RunCompute `+0x14`, `+0x83e`, and `+0x857` remain zero and the live `+0x760` producer is unchanged. No module was installed and no RunCompute was executed.
