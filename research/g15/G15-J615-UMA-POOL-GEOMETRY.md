# J615 G15 UMA pool geometry — E083

E083 promotes the formerly conditional G15 UMA fallback values to exact J615 / 23J220 target facts. This is static exact-target plus compile-only Linux work; no module was installed and no RunCompute was issued.

Exact G15G getters load accelerator `+0x1e80` and `+0x1e78`. If zero they return `M=0x80000000` (2 GiB) and `B=0x400000` (4 MiB).

The accelerator allocation path is zero-filled by `OSObject_typed_operator_new(... Z_WAITOK_ZERO)`. The complete G15 constructor does not write either field. Across the exact AGXG15G Mach-O, the only six references to the two offsets are getter loads; configure/start/property paths do not write them, no computed 0x1e00/0x1e70 writer base exists, and no raw 32/64-bit 0x1e78 or 0x1e80 value exists for an offset table. Therefore the zero values survive to the getters on J615.

With exact 16-KiB host/UAT page size, Apple's FList formulas resolve to:

- Page Pool List: `0x400000` bytes (4 MiB)
- Page Pool List capacity: `0x80000` 8-byte entries
- Backup Page List: `0x8000` bytes (32 KiB)

Linux commit `b88369c26ffe82ce73765b8bbba64b2db771ef76` pins these target constants, adds checked `G15FListGeometry::j615()` and a side-effect-free `G15FListResourcePlan::new_j615()`. It still owns no GPU allocator or object.

Validation: base `724674ad034ee3502aa9448324cd0056b7b0c670`; tree `44e9f3c3c84933d5679dd90a647ebcd377855ee8`; module SHA-256 `c1259bd681ac030e38b75a4df2b77066423af48bb1f205f15be21acf609c3632`; exact existing 24-warning baseline; strict checkpatch 0/0/0; no install.

The next boundary is compile-only construction of the four persistent FList resources using the already-closed range-5/range-7/range-8 mapping classes and HardwareBuffer ownership. Command publication remains disabled.
