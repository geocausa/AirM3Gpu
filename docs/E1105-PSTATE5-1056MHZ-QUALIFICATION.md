# E1105 — J615 pstate 5 / 1056 MHz qualification on Linux 7.3

Date: **2026-10-25**
Result: **PASS**

E1105 qualifies the top state admitted by the original 1..5 T8122 experiment gate.

- InitData published `cap 5 (1056 MHz), boot request 5 (1056 MHz)`; firmware peak/actual/current state 5 was observed.
- The live table identifies state 5 as **1056 MHz**, 805 mV core / 805 mV SRAM.
- One hardware-qualified Honeykrisp GNOME Shell persisted through the run.
- Two 300-frame FIFO/`nowlts` rounds both returned 0; DCP advanced `20 -> 313` and `328 -> 625`, `failed=0`; all post-baseline fatal scans were empty.
- Exact automatic Golden recovery succeeded.

No effective SoC-die temperature zone is available on current Linux J615, so this remains short bounded qualification. Higher states required an explicit admission-gate change rather than treating the cap-5 policy as a hardware maximum.

Private evidence hashes: `RESULT.md` `e845b38abcdfc54aab0cdce858d1e623be27ca386e5a52525477a6f54b27a258`; `EVIDENCE-SHA256.txt` `66caf49a5c25c4562d4e3a2bb47ba492c26f61ea3b90da60290d88be8879fd0a`.
