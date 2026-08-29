# G15 Compute prepared SKU slot token

E107 joins the exact stock-empty serializer and exact persistent SKU backing at a deliberately unpublished copy boundary.

Linux commit `f3464e7ec21633631df5e942303146069a62737e` adds `G15SkuBacking::write_retired_stock_empty_slot()`. A caller must already have selected/bound the slot using the E106 retirement guard. The method checks the index, clears the complete 0x300-byte slot, copies exactly the E102 0x2c0-byte stock-empty stream, verifies the final 0x40 bytes remain zero, derives `backing_fwva + slot*0x300`, and returns an opaque prepared token containing slot index, FWVA and reported size 0x2c0.

The token has no consumer outside its definition. The backing owner is still unreachable, E106's guard is still unreachable, and the existing RunCompute `+0x760` producer is unchanged. This deliberately separates “a byte-exact slot exists” from “firmware may execute it.”

Validation: tree `5a6003c16ef70d8539cc948f03e949bf5c83458f`; module SHA-256 `0a8797840a9b64d40d62cbc8c302f463fd812283d55a0df92202b1e5716387c6`; exact 24-warning baseline; strict checkpatch 0/0/0; exact-tree reconstruction PASS.

No module was installed and no RunCompute or other GPU command was issued.
