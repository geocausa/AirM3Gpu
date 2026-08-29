# G15 unpublished stock-empty Compute assets token

E109 materializes one coherent stock-empty command asset set without creating a RunCompute consumer.

Linux commit `a783f835b437e43f019b58b00daa0935b6dacf84` lets the unreachable E108 owner graph reseed a caller-retired event-control slot, write the exact E102 bytes into a caller-retired SKU slot, activate/populate the stock-empty FList HardwareBuffer epoch, obtain the initialized Page-Pool-State FWVA, and take the current HWMetrics record. These values are returned only inside a private non-Copy host token together with the SKU size and HardwareBuffer ID.

First-population failure rolls back the acquired HardwareBuffer reference. Post-activation address failure also rolls that reference back. `complete_unpublished()` consumes the token, avoiding ordinary double completion. E096/E106 retirement guards remain deliberately separate and must be integrated before a command-field consumer can be considered.

The live G15 constructor still writes zero at RunCompute `+0x14/+0x83e/+0x857`, and `+0x760` remains unchanged. Validation: tree `21c8418854d9c10ac54cebf6b5b60fda24214619`; module SHA-256 `edc77c86d83582e57b7aac8f9e372ba279316740b4f04bd646c91fadb1857bc2`; exact 24-warning baseline; strict checkpatch 0/0/0. No install or RunCompute.
