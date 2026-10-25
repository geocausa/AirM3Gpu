# E1103 — J615 pstate 3 / 796 MHz qualification on Linux 7.3

Date: **2026-10-25**  
Result: **PASS**

E1103 advances one state beyond E1102 while retaining the same kernel, pinned accelerated GNOME/Honeykrisp userspace, `nowlts` path, two-round 600-requested-frame workload and Golden recovery transaction.

- Live J615 InitData published `cap 3 (796 MHz), boot request 3 (796 MHz)`.
- Firmware genuinely exercised state 3: `firmware peak state 3` and repeated `firmware actual 3` / `current 3` samples were recorded.
- The same hardware-qualified Honeykrisp GNOME Shell PID remained valid before and after the workload.
- Both 300-frame clients returned `0`; DCP progressed `18 -> 321` and `335 -> 624`, with `failed=0`.
- Both per-round and the global post-baseline GPU/DART/RTKit/panic scans were empty.
- Exact automatic Golden recovery succeeded.

As with E1102, J615 currently has no effective SoC-die temperature zone under Linux, so this is deliberately short bounded qualification rather than thermal-soak evidence.

Private evidence: `RESULT.md` SHA-256 `27b4bc9b07f89325e3fd7a348e8826fcfe643173c016be6d6b1e558db2af8b18`; `EVIDENCE-SHA256.txt` `4ec8305ff8e1f01a1594cfeb8a53d599ec6d9f570f7c9c667b3b8b8679c2a8d8`; runner `52dce145c354fcc8e88fd26fcff25cb014641c0c94e002cbde95b082de01ad6c`; controller `ad350ca5548cfc91015464ff12184e226d5344a03cf40913d8b77d6b1d1ef2ad`.

Next gate: **pstate 4 / 928 MHz**.
