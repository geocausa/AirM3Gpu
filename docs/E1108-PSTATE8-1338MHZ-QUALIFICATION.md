# E1108 — J615 pstate 8 / 1338 MHz qualification on Linux 7.3

Date: **2026-10-26**
Result: **PASS**

E1108 qualifies the highest performance state published by the current J615 device-tree/firmware table under the established short accelerated-GNOME workload.

- Source commit `2866b256c82f5c71e5a9205f7cfcf7ab44a89460` (`asahi: admit bounded T8122 pstate 8 experiment`) is pushed to `geocausa/linux:wip/m3-7.3-j615`.
- Independent controls passed before hardware: 8/8 T8122 knob tests, 86/86 air-gpu tests with 19 expected skips, and 46/46 release tests with 29 expected skips.
- One-shot kernel `7.3.0-m3-j615-e1108+` published 8 performance states from 13 device-tree states, with state 8 at **1338 MHz**, 915 mV core / 915 mV SRAM and a table maximum-power value of 22000 mW.
- InitData published `cap 8 (1338 MHz), boot request 8 (1338 MHz)` and firmware highest state 8.
- State 8 was genuinely exercised: `firmware actual 8 current 8` was observed immediately after boot and repeatedly during the workload; firmware peak state was 8.
- One hardware-qualified Honeykrisp GNOME Shell persisted before/after; no software renderer or missing pinned mapping was observed.
- Two 300-frame FIFO/`nowlts` rounds returned 0; DCP advanced `21 -> 320` and `334 -> 622`, `failed=0`; global DCP advanced `3 -> 635` and reached 641 after cleanup; all post-baseline fatal scans were empty.
- Exact automatic Golden recovery succeeded, leaving no owner marker, one-shot entry or E1108 service.

**Boundary:** 1338 MHz is the highest state in the current live J615 table used by this stack. It is not asserted as a universal physical silicon maximum. Linux still reports no effective SoC-die temperature zone (`error -19`), so the next frequency-related work is telemetry plus sustained/thermal qualification—not inventing a state above the table.

Private evidence hashes: `RESULT.md` `593b1260ac58c5646d23f8d1568ac1b96a42e8dd389ff966bd1f78b3cb16f22e`; `EVIDENCE-SHA256.txt` `e50d8f4007139fc76911902e08324bed41ba7b43ebb06f75054e663fad73d2b3`.
