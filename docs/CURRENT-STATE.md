# Current G15 Bring-up State

Checkpoint date: 2026-08-24

## Hardware identity

Target: J615 MacBook Air M3 / T8122.

Runtime identity registers:

- `ID_VERSION = 0x07022000`
- `ID_COUNTS_1 = 0x0011010a`
- `ID_COUNTS_2 = 0x00040404`
- active core mask: `0x3ff`
- topology: 1 MGPU/cluster, 10 cores, 10 fragment units, 4 GPs

## Closed runtime gates

The following were independently exercised on real hardware and then torn down cleanly:

1. Device discovery and G15G C0 identity validation.
2. GFX power-domain activation and ASC start.
3. 42-bit G15 UAT handoff, shared bank-1 root setup, TTB bootstrap, and cleanup.
4. J615 `PwrConfig` parsing and exact validation.
5. Complete G15 InitData construction and CPU-side validation, followed by destruction.
6. Complete pre-RTKit GpuManager/channel/backing graph validation.
7. RTKit management protocol v12 handshake and endpoint-map discovery.
8. EP1 firmware-preallocated crashlog physical backing and successful RTKit acceptance.
9. RTKit application endpoint EP20/EP21 startup with no InitData or application traffic.

After each preflight, ASC is stopped and the driver deliberately fails closed rather than registering DRM.

## Power model closure

J615 has 14 performance-state records: one zero/default state followed by 13 active states.

Machine-exact/derived values currently used:

- core leakage coefficient: `1644`
- SRAM leakage coefficient: `60`
- minimum SRAM voltage: `790000 uV`
- `sram_k = 1.02`
- base clock: `24000000 Hz`
- maximum reconstructed power: `21405 mW`
- Smart Idle standby timer: `700 us`
- maximum fragment units: `10`
- maximum GPs: `4`

See `research/g15/J615-POWER-CONFIG.md` for the active OPP table.

## G15 InitData closure

The G15 HwDataA legacy Shared1/2/3 region is replaced by a generation-specific deterministic pre-tail spanning `0x3a9c..0x421b`.

Important proven boundaries:

- pre-tail begins: `HwDataA + 0x3a9c`
- float `5.0`: `+0x3aa4`
- C0 DPE/PPT image begins: `+0x3aa8`
- DPE/PPT image length: `0x5dc`
- SoCHot block begins: `+0x4188`
- SoCHot sensor mask at `+0x4198`: `0x4248`
- SoCHot scalar at `+0x41a0`: `125`
- existing typed G15 tail begins: `+0x421c`
- complete HwDataA size remains `0x4360`

The clean-room DPE/PPT encoder is included at `research/g15/build-g15g-c0-dpe-ppt.py`.

## RTKit boundary

Management negotiation succeeds with protocol version 12. System endpoints are allowed to negotiate; application endpoints remain blocked.

EP20 (firmware) and EP21 (doorbell) are discovered and can now be started independently. q21 remains untouched. No InitData address is sent to firmware and no `MSG_INIT` occurs.

### EP1 crashlog backing — closed

System endpoint EP1 sends:

- raw request `0x1041000192c000`
- buffer size `0x4000`
- supplied address `0x1000192c000`

The iBoot-populated live ADT places the address inside firmware carveout `region-id-25` (`0x10001888000..0x10001f73fff`). Read-only AGX UAT walks show that neither the full address nor the low-40 `0x192c000` form has a TTBR0 mapping, including after RTKit management boot. A bounded `memremap(WB)` succeeds and reads the firmware fill pattern `0xefefefefefefefef`.

The G15 RTKit implementation now models this as firmware-preallocated physical ordinary memory, distinct from host-allocated GEM/UAT buffers. Live validation completes with zero GPU `failed buffer request` messages. See `research/g15/G15-RTKIT-CRASHLOG.md`.

### Current boundary

EP20 (firmware) and EP21 (doorbell) are now started in the tested preflight using RTKit `STARTEP` management messages only. No unknown app message, crash, q21 mutation, or buffer failure is observed. The next stage is the first firmware-visible InitData handoff and `MSG_INIT` startup-read closure.

## Explicitly not enabled

- no application RTKit endpoint start
- no `MSG_INIT`
- no DRM registration/render node
- no queue submission
- no general G15 render support claim

The next live stage must remain behind a separately proven mapping boundary.
