# E1107 — J615 pstate 7 / 1278 MHz qualification on Linux 7.3

Date: **2026-10-26**
Result: **PASS**

E1107 moves the explicit T8122 experiment ceiling one state higher while retaining the same workload and recovery contract.

- Source commit `beeec2e1a3201b44577d1990c795c1203d6b2ff3` (`asahi: admit bounded T8122 pstate 7 experiment`) was pushed to `geocausa/linux:wip/m3-7.3-j615` before the live test.
- `7.3.0-m3-j615-e1107+` published `cap 7 (1278 MHz), boot request 7 (1278 MHz)`.
- Firmware genuinely reached state 7, including a workload ramp through states 5 and 6 to `firmware actual 7 current 7`; firmware peak state was 7.
- State 7 is **1278 MHz**, 890 mV core / 890 mV SRAM.
- Two 300-frame FIFO/`nowlts` rounds returned 0; DCP advanced `29 -> 314` and `328 -> 617`, `failed=0`; global DCP reached 630 during the workload; fatal scans were empty.
- Honeykrisp GNOME remained hardware-qualified and exact Golden recovery succeeded.

The Linux driver still lacks effective SoC-die temperature feedback on this board, so this is not a thermal-soak or daily-driver qualification.

Private evidence hashes: `RESULT.md` `1c1dd97ee0359114a9b2355310b83c25feb4bf98f44f8580fc72d5aa7af80e73`; `EVIDENCE-SHA256.txt` `1c755a6b6cfa13a5f3c86b8401ec2960a85c16837676105c8f4fc2209ebffabf`.
