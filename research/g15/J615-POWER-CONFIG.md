# J615 G15G Power Configuration

The live J615 performance table contains one zero/default state followed by 13 active states. The active states currently reconstructed and validated are:

| Frequency | Core voltage | Maximum power |
|---:|---:|---:|
| 338 MHz | 650 mV | 3,516 mW |
| 618 MHz | 675 mV | 5,774 mW |
| 796 MHz | 720 mV | 8,103 mW |
| 836 MHz | 755 mV | 9,376 mW |
| 928 MHz | 755 mV | 10,194 mW |
| 952 MHz | 805 mV | 12,037 mW |
| 1056 MHz | 805 mV | 13,102 mW |
| 1053 MHz | 850 mV | 14,799 mW |
| 1170 MHz | 850 mV | 16,149 mW |
| 1152 MHz | 890 mV | 17,713 mW |
| 1278 MHz | 890 mV | 19,322 mW |
| 1204 MHz | 915 mV | 19,586 mW |
| 1338 MHz | 915 mV | 21,405 mW |

Additional validated values:

- core leakage coefficient `1644`
- SRAM leakage coefficient `60`
- minimum SRAM voltage `790 mV`
- G15G SRAM scaling constant `1.02`
- reference/base clock `24 MHz`

The deliberately interleaved frequency/voltage ordering is preserved; the Linux device-tree OPP order must match the runtime pstate index order rather than sorting by frequency.
