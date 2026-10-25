# E1100 — Linux 7.3 replay of the E1095 Wayland timestamp ABCA

Date: **2026-10-25**  
Target: **MacBook Air M3 J615 / T8122 / G15G C0**  
Result: **PASS — qualified `TIMESTAMP-PATH SPLIT`**

## Proven

E1100 replayed the qualified E1095 `A1 -> B -> C -> A2` Wayland timestamp bracket on the physical machine while changing the kernel/handoff to `7.3.0-m3-j615-e1098+` and retaining the E1095 userspace/compositor topology. `asahi.t8122_pstate_cap=1` was deliberately held fixed as the previously qualified control operating point; it is **not** a claim about the maximum supported M3/G15G frequency.

All four arms used a matched 3-image FIFO topology (`500x500`, `XRGB8888`, stride `4096`, modifier `0x0`). Results:

- **A1 / passthrough timed FIFO:** `rc=124`, stalled, max steady attach gap `16934.396 ms`; timestamps `40 seen / 0 dropped`.
- **B / drop only `wp_commit_timer_v1.set_timestamp`:** `rc=0`, no stall, max steady gap `106.834 ms`; timestamps `53 seen / 53 dropped`.
- **C / `MESA_VK_WSI_DEBUG=nowlts`:** `rc=0`, no stall, max steady gap `108.869 ms`; zero commit timestamps emitted.
- **A2 / passthrough reversal:** `rc=124`, stalled, max steady attach gap `14141.215 ms`; timestamps `52 seen / 0 dropped`.

Every arm observed FIFO set/wait traffic. Every post-baseline GPU/DART/RTKit/panic scan was empty. Native DCP stayed at `failed=0` while swaps advanced in every arm: A1 `9 -> 66`, B `67 -> 110`, C `114 -> 143`, A2 `145 -> 205`.

The analyzer returned `qualified=true` and the same verdict as E1095: **timestamp-only suppression removes the FIFO60 stalls while both passthrough controls stall; `wp_commit_timer_v1.set_timestamp` is causally necessary for the failure under matched 3-image FIFO topology.**

## Causal meaning

The E1095 split survives the move from the older experimental 7.1.x lineage to Linux 7.3 while holding the userspace/compositor topology and pstate control fixed. This strongly supports the conclusion that the timed-FIFO stall is **not specific to the older kernel lineage**, and Linux 7.3 by itself does not fix it.

A1/A2 were more severe in this one replay and hit the 30-second per-arm timeout. One replay is not enough to claim a quantitative 7.3 regression.

## Recovery and transaction boundary

The successful run booted Linux 7.3 through the pinned handoff SHA-256 `243bccb1566251bd6214cbc63472a0c03d84412340ade2d2454ab7010b180261`. The controller restored Golden boot assets before the live bracket began and automatically returned to `7.1.6-ubuntu-m3-usbpd-gc5037a961e4d` afterwards. Restored `boot.bin` matched `a81993b122b7f990bc99ccab42decab7ef92adb94741dadf0ee79d24fdda3e0d`; restored `custom.cfg` matched `cf011eee536420c6b32fa5b311345f507ae260d06c7f30777e2951e683b73462`; no E1100 service, owner marker or GRUB one-shot remained.

The first E1100 attempt exited before A1 because the copied E1095 runner still expected the experimental `boot.bin` to remain installed during the workload. The E1099-derived controller intentionally restores Golden before live testing. The runner was corrected to qualify the experimental handoff from the controller startup record and require Golden to already be restored; transaction/proxy tests and real preflight were rerun before the successful replay.

## Private evidence hashes

- E1100 `RESULT.md`: `761ea439c508364d29e3fc1015d7ba9245697bea834849d77c2fe2ef32d33d03`
- E1100 `EVIDENCE-SHA256.txt`: `5a2f7fc587447c0a8d77116c7c07fc0985c29b3f13bafe6d17c32aaa6118d6fc`
- successful `BRACKET-SUMMARY.txt`: `8e08c3481e5183a293e0f78eefc09bcae2689e2b97a7ec81a88936a64bb1c1ca`
- successful `BRACKET.json`: `d393293d259ae14b6a6bc5231c060fb27295c7db5a8b81224c70a82061415d65`
- corrected `run-e1100-abca.sh`: `82f4a7e317ec13fa5cf6995a3999971074913429c261caa39e06711ffb43090d`
- E1100 `boot-control.py`: `510d6af7cd51a3f23439f382ca03ef786eb094c894855d7f2cca1384754c2e74`

Raw machine evidence remains private.

## Next boundary

With the 7.3 causal replay closed, the next clean gate is accelerated-GNOME endurance on Linux 7.3 using the known-good untimed/`nowlts` path at the fixed control pstate. Frequency/pstate escalation is a **separate** subsequent experiment so endurance is not confounded by changing clocks.
