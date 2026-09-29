# G15 E328 known-working entry on corrected mapping baseline

Research state: E327/E328, 2026-09-29.

E327 kept the fully corrected E325 memory/ownership baseline and four-byte STOP body, but replaced the executed exact-23J220 direct loader with the full G15 entry-state program shared by Alyssa a1006e52 successful M3 Compute and pac85.

Kernel branch: wip/g15-e327-working-entry-corrected-map
Kernel commit: d8f638d5e6050c4d6e14740256b65fb7fcc97efc

The protected E328 live run reproduced the same boundary: normal-UAPI Compute accepted, Compute/2 reached the submitted state, first RunWorkQueue accepted by scheduler, then immediate GPU timeout. Scheduler release later returned ETIMEDOUT.

This means the failure is not explained by the exact production direct-loader byte stream, its LoadDev/profile operations, or the independently known-working M3 entry program. With corrected memory classes held fixed, both entry families fail identically.

Golden Ubuntu and the sacrificial slot were restored. The next search moves outside entry-program bytes into engine-side activation/firmware execution state or another prerequisite not exercised by E199 terminate-only.
