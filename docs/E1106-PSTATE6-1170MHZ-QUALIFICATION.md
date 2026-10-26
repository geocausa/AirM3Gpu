# E1106 — J615 pstate 6 / 1170 MHz qualification on Linux 7.3

Date: **2026-10-25**
Result: **PASS**

E1106 deliberately widens only the armed T8122 experiment ceiling to state 6; ordinary non-experimental M3 policy remains unchanged.

- Kernel source commit `a922ef259ba...` (`asahi: admit bounded T8122 pstate 6 experiment`) is retained in `geocausa/linux:wip/m3-7.3-j615` ancestry.
- One-shot kernel `7.3.0-m3-j615-e1106+` published `cap 6 (1170 MHz), boot request 6 (1170 MHz)`.
- Firmware genuinely exercised state 6; peak/actual/current state-6 observations were retained.
- State 6 is **1170 MHz**, 850 mV core / 850 mV SRAM.
- Two 300-frame FIFO/`nowlts` rounds returned 0; DCP advanced `19 -> 313` and `325 -> 612`, `failed=0`; global DCP reached 626 before cleanup and every fatal scan was empty.
- Honeykrisp GNOME remained hardware-qualified and exact Golden recovery succeeded.

This remains a short no-temperature-feedback qualification, not thermal soak or a maximum-frequency claim.

Private evidence hashes: `RESULT.md` `1cbcf10bda5b44a5958e1ee626d1ecaecbdf6fdbf19b77e03298cb214c15dd7a`; `EVIDENCE-SHA256.txt` `d7822ba4d39dd7fa11ec0015dafa1f9b732b2e9f4941809d5b1e78df30031b56`.
