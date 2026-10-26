# E1104 — J615 pstate 4 / 928 MHz qualification on Linux 7.3

Date: **2026-10-25**
Result: **PASS**

E1104 advances one performance state beyond E1103 while retaining the same pinned accelerated GNOME/Honeykrisp userspace, `MESA_VK_WSI_DEBUG=nowlts`, two-round 600-requested-frame workload, post-baseline fault scanning and Golden-first recovery transaction.

- InitData published `cap 4 (928 MHz), boot request 4 (928 MHz)` and firmware highest state 4.
- Firmware genuinely exercised state 4: repeated actual/current state-4 samples and firmware peak state 4 were observed.
- The live table identifies state 4 as **928 MHz**, 755 mV core / 790 mV SRAM.
- One hardware-qualified Honeykrisp GNOME Shell persisted before/after with no software renderer or missing pinned mappings.
- Both 300-frame FIFO/`nowlts` rounds returned 0; DCP advanced `20 -> 310` and `324 -> 621`, with `failed=0`; the global post-baseline GPU/DART/RTKit/panic scan was empty.
- Automatic recovery returned exactly to Golden `7.1.6-ubuntu-m3-usbpd-gc5037a961e4d`.

This is short bounded qualification, not thermal-soak or maximum-frequency evidence. Linux still exposes no effective J615 SoC-die temperature zone to the driver.

Private evidence hashes: `RESULT.md` `4e5b9e67b1c844b328fc8df9e1c728888c4777d58ef207026b6ce6c78d71b1e7`; `EVIDENCE-SHA256.txt` `7d7716c427d2205c2429cb5144b05519e178d0393a4c5d308fb30e7b511f53b4`.
