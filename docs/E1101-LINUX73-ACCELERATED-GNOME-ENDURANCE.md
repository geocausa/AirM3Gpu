# E1101 — Linux 7.3 accelerated GNOME endurance control

Date: **2026-10-25**  
Result: **PASS**

E1101 extends E1099 from a 60-frame smoke test to a bounded multi-round accelerated-desktop control on `7.3.0-m3-j615-e1098+`. `asahi.t8122_pstate_cap=1` remained fixed strictly as the known-good control point; it is **not** the M3/G15G maximum frequency.

## Proven

- One GNOME Shell PID (`2363`) survived the entire workload. Hardware qualification passed before and after with repeated `zink Vulkan 1.4(Apple M3 (G15G C0) (MESA_HONEYKRISP))`, no software renderer and no missing pinned mappings.
- Eight consecutive FIFO/`MESA_VK_WSI_DEBUG=nowlts` clients requested 600 frames each: **4,800 requested frames total**. All eight returned `0` and selected `Apple M3 (G15G C0)`.
- The eight client-round wall intervals totalled **80.536783 s**.
- DCP `failed` stayed `0`; every round advanced swaps. Global DCP progressed from `3` before GNOME startup to `4872` after endurance (`4878` after cleanup).
- All eight per-round post-baseline GPU/DART/RTKit/panic scans and the global post-baseline scan were empty.
- Shell RSS moved from `325504 kB` before the rounds to `325936 kB` after (+432 kB); observed `VmHWM` after the run was `334464 kB`. This is a bounded observation, not a leak proof.
- The runner recorded `ACCELERATED_GNOME_7_3_ENDURANCE_CONTROL_PASS`, then the controller automatically returned to Golden `7.1.6-ubuntu-m3-usbpd-gc5037a961e4d` with exact known-good boot assets and no E1101 service/owner/one-shot left behind.

## Frequency boundary discovered for the next experiment

The actual J615 7.3 boot log publishes eight performance states: **338, 618, 796, 928, 1056, 1170, 1278 and 1338 MHz**. The current T8122 experiment intentionally accepts `t8122_pstate_cap=1..5`, so the present controlled escalation range is 338 through 1056 MHz. J615 currently exposes no SoC-die thermal zone to this driver; higher-cap tests therefore run without effective temperature feedback and must be short, one state at a time, with the existing hard recovery/fault gates. The first escalation target is state 2 / **618 MHz**, not the top state.

## Private evidence hashes

- E1101 `RESULT.md`: `afa5cba950d5508a7b6fad528e4a7adebf73a26f73e5bb71c8e816e70f8fc0ee`
- E1101 `EVIDENCE-SHA256.txt`: `4a6e478c333e8b790e29c473af260343ad0e8b7d51e86afeee488027fe44a381`
- E1101 `run-session.sh`: `1061b6d9e2eae6f77170854007b9368e2515fb8ed07f21d880b577c1ceeec5ea`
- E1101 `boot-control.py`: `4ac07bd2d575c11ac134759ebe22048cbd4a64c68efda119a2d4105cc04b3c73`
- E1101 `session.py`: `d0ec9d90720b4f7ea6f042eb686213b5da8ab924d33797c2c77894f28f2c9fa7`
