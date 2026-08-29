# G15 stock-empty Compute prerequisite owner graph

E108 proves the independently reconstructed stock-empty G15 Compute resources can coexist under one unreachable construction owner without making any RunCompute field live.

Linux commit `ea707e8e7726c30da653a026384d097c876db0dc` groups the exact event-control backing, HWMetrics page, FList/HardwareBuffer owner and Compute SKU backing. Shared resources clone one bank-1 owner and q22 mapping-notifier lifetime; the FList retains its separate per-VM range-5 lists and accelerator-global HardwareBuffer-ID manager.

The graph deliberately does not merge Apple's distinct activation/retirement epochs. It does not select event/SKU slots, populate the FList, advance HWMetrics, own retirement guards, expose FWVAs or construct a RunCompute command. The live constructor still leaves `+0x14`, `+0x83e` and `+0x857` at zero and does not consume the prepared SKU token at `+0x760`.

Validation: tree `047a29b09eae77f4956ebe31a2fef1c3abf201a9`; module SHA-256 `56de872a869d55f785295258c88d43ba51b15639b4742a6bd3d64cc0e1ff41d5`; exact 24-warning baseline; strict checkpatch 0/0/0; no install and no RunCompute.
