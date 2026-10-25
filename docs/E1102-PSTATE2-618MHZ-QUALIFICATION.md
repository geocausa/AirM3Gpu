# E1102 — J615 pstate 2 / 618 MHz qualification on Linux 7.3

Date: **2026-10-25**  
Result: **PASS**

E1102 changes one performance variable from the E1101 control: `asahi.t8122_pstate_cap=1` becomes `2`. Kernel, pinned Honeykrisp/Zink accelerated-GNOME userspace, `MESA_VK_WSI_DEBUG=nowlts`, one-shot boot transaction and Golden recovery remain otherwise fixed.

## Proven

- Live J615 InitData published `cap 2 (618 MHz), boot request 2 (618 MHz)` and firmware highest state 2.
- Firmware actually exercised it: the log records `firmware peak state 2` plus repeated `firmware actual 2` / `current 2` samples. This is not merely a requested cap.
- One hardware-qualified GNOME Shell PID remained stable before and after; no software renderer or missing pinned mapping was observed.
- Two 300-frame FIFO/`nowlts` clients both returned `0` (600 requested frames total), with client intervals `5.352248 s` and `5.039651 s`.
- DCP advanced `24 -> 323` and `336 -> 634`; `failed=0` throughout. Global DCP reached `646` swaps after the workload.
- Both per-round and the global post-baseline GPU/DART/RTKit/panic scans were empty.
- Automatic exact Golden recovery succeeded.

## Thermal boundary

The driver explicitly reports that current J615 Linux has **no SoC-die temperature zone**, so this state-2 run has no effective temperature feedback. The test is therefore deliberately short and recovery-guarded. That limits the strength of the claim to bounded workload qualification; it is not thermal-soak or daily-driver qualification.

The actual live J615 table is 338, 618, 796, 928, 1056, 1170, 1278 and 1338 MHz for states 1–8. The current T8122 start experiment deliberately accepts caps only **1–5**, so the present test program advances one state at a time through at most 1056 MHz unless the thermal/admission policy is deliberately revised with new evidence.

## Private evidence hashes

- `RESULT.md`: `e8140a21e127616be971ea74490cfcdf23ed4059bb8be8e15082d783e6c854f4`
- `EVIDENCE-SHA256.txt`: `b7dee881f8577e894bdc038d9f9ce748f61db7afd1ff3e7b72f28d7354e07950`
- `run-session.sh`: `303a78739cf75784b8a2abcd129251a6d913e5b380a464c66d0ce02efaa0de90`
- `boot-control.py`: `b3e98bcc9e33d2b6af0b59211ebea5a5a36faf9e2e063f0747cafb3f644e6fce`

Next clean gate: **pstate 3 / 796 MHz** with the same short bounded workload and hard recovery/fault gates.
