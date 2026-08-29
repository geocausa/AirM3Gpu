# G15 stock-empty RunCompute field staging

E111 converts the guarded private asset set into a host-only staging record for the exact stock-empty command-facing fields without mutating a firmware RunCompute object.

Linux commit `139b745dbf0092825cad3c65bc18fb03385f2305` stages event-control `+0x14`, SKU `+0x760/+0x768`, Page-Pool State `+0x83e`, exact UMA prepared/min/ideal `+0x846/+0x847/+0x84f = 1/0/0`, and HWMetrics `+0x857`. The underlying non-Copy assets token remains embedded so HardwareBuffer completion ownership is retained until the stage is consumed.

E111 also rejects zero/mismatched staged FWVAs/SKU size and preserves FList rollback. There is no method that writes the stage into `fw::compute::RunCompute`, no outside-module consumer, and the live constructor remains unchanged. Validation: tree `a9dd74c5b7a0acb8b43e120f5e72d7cb0be99894`; module SHA-256 `ec27e7ac2f4fdb9725813017baeb1431e9361bcf9bf7083322f4082b62b3f755`; exact 24-warning baseline; strict checkpatch 0/0/0. No install or RunCompute.
