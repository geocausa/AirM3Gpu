# E1099 — Linux 7.3 J615 live qualification

Date: **2026-10-25**  
Target: **MacBook Air M3 J615 / T8122 / G15G C0**  
Result: **PASS**

## Proven

- A guarded one-shot boot entered `7.3.0-m3-j615-e1098+` on the physical J615.
- The experimental handoff SHA-256 was `243bccb1566251bd6214cbc63472a0c03d84412340ade2d2454ab7010b180261`.
- G15G firmware reached `firmware-running`.
- GNOME Shell created repeated hardware contexts as `zink Vulkan 1.4(Apple M3 (G15G C0) (MESA_HONEYKRISP)) vendor=Mesa`; no software-renderer compositor context was observed.
- The bounded FIFO60 Vulkan client returned `0` and selected `Apple M3 (G15G C0)`.
- The first Render job completed on the GPU in `175041 ns`; the first Compute job completed in `1273875 ns`.
- Native DCP stayed healthy: `failed=0`, while swaps advanced `3 -> 83` during the bounded test and `95` by cleanup.
- The post-baseline GPU/DART/RTKit/panic scan was empty.
- The inner runner returned `0` and recorded `COMPOSITOR_GPU_AND_FIFO60_BOUNDED_PASS`.
- Golden boot assets were restored and the machine automatically returned to `7.1.6-ubuntu-m3-usbpd-gc5037a961e4d`.
- Restored `boot.bin` SHA-256: `a81993b122b7f990bc99ccab42decab7ef92adb94741dadf0ee79d24fdda3e0d`.
- Restored `/boot/grub/custom.cfg` SHA-256: `cf011eee536420c6b32fa5b311345f507ae260d06c7f30777e2951e683b73462`.

## First-run scanner correction

The first E1099 live run reached the same hardware compositor/client path but was rejected by the old fatal scanner because it saw an Apple-DART read fault at boot time `11.803910s`. Direct comparison proved that exact line already existed in the pre-test dmesg baseline. The scanner was corrected to examine only lines appended after the baseline, with a conservative ring-wrap check. All six offline renderer controls, all seven boot-transaction/recovery tests and the real-machine preflight passed again before the successful second run.

## Kernel source provenance

The Linux 7.3 J615 source is retained in `geocausa/linux` branch `wip/m3-7.3-j615` at commit `52f5d1cb8f5983131319a3083c0f1597e871a3e9`. The built release is `7.3.0-m3-j615-e1098+`.

## Evidence hashes

- E1097 private `RESULT.md`: `5e5bed6c23002b502d39190892ffa9e194df3c5e003a3f3ecfa2ec9b4dd7516f`
- E1098 `PROGRESS.md`: `51f54fdf06e8c2726bd2a009a68e3d96cd06df4026bb6cdd47b4091a46765447`
- E1098 `KERNEL-BUILD-MANIFEST.json`: `05d6a0906201441da7ba81845b9eb8f4a28d773c644368f6430bba449c2e1014`
- E1099 private `RESULT-LIVE.md`: `2dccd8ce5acf55999bd51c610a014f6789dd2453d16cf9166b088caaeea13a48`
- E1099 private `EVIDENCE-SHA256.txt`: `5fe5fe434b369d2d034b8a0137c33836c36e7452304a5b74be8d2de54615fb4e`
- E1099 corrected `run-session.sh`: `c17e00e41bf5d83ceb6a09a81edbf64ee111a0ebbd1cc24728e839dd9009956f`
- E1099 `boot-control.py`: `4a493995f357265cf39b00050e67ddc124577edcf016d30536658b1260c62c75`

## Boundary

This proves that the Linux 7.3 candidate boots J615 and completes the bounded native GNOME + Honeykrisp Render/Compute/FIFO60 path with native DCP progress and automatic Golden recovery. It is **not** yet an endurance, suspend/resume, unrestricted-frequency, peripheral-regression or daily-driver qualification.
