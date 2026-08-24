# G15 root q4 / q21 reconstruction

Scope: exact AGXG15G host driver paired with `RTKit-2419.140.12.release`.
This note records only fields proven from the Apple host or direct firmware users.
Runtime T8122 enablement remains intentionally disabled.

## Root pointers

The G15 root is 24 qwords / 0xc0 bytes. Apple writes:

- q4 (+0x20): firmware-translated GPU side of the allocation whose host CPU member is `AGXFirmware+0x2b0` (`param_1[0x56]`) and GPU member is `+0x2b8` (`param_1[0x57]`). Exact allocation size is 0xe00.
- q21 (+0xa8): firmware-translated GPU side of the 0x20-byte allocation whose CPU member is `AGXFirmware+0x548` (`param_1[0xa9]`) and GPU member is `+0x550` (`param_1[0xaa]`).
- q22 (+0xb0): 0xc3d0-byte object, CPU/GPU host members +0x620/+0x630.
- q23 (+0xb8): 0x238-byte object, CPU/GPU host members +0x628/+0x638.

## q21: exact 0x20-byte shared status block

Host writes:

| Offset | Width | Host evidence | Firmware evidence / meaning |
|---:|---:|---|---|
| 0x00 | u32 | `AGXArmFirmware::initFirmwareSharedData` copies host object +0x60c | Firmware copies it to `DAT_...e89c`; this is a host-supplied flags word, not a halt counter. |
| 0x04 | u32 | `AGXFirmware::initFirmwareData` writes 1 | Firmware `FUN_...3c5dc` tests 0, sets 1, then emits the one-time `GFX ... FW ...` banner. One-time/banner guard. |
| 0x0c | u32 | no host nonzero write found | Firmware `FUN_...6a0c` sets 1 while handling a critical/busy section then clears it to 0. |
| 0x14 | u32 | zero after allocation | Firmware boot `FUN_...3dc6c` writes 0 before main initialization and 1 after initialization, with barriers. Strong firmware-ready/boot-stage indicator. |
| 0x18 | u32 | zero after allocation | Firmware power-state paths update it to the current power/state index. Not a legacy halt counter. |

All these offsets are naturally u32-aligned. The remaining q21 bytes are currently unresolved/zero.

**Consequence:** the legacy Asahi `FwStatus` layout must not be overlaid onto q21. In particular, old `halt_count/halted/resume` offsets have no demonstrated G15 equivalence.

## q4: exact 0xe00 compact configuration block

Firmware aliases root q4 as `DAT_fffffc000010e260` and copies/uses many fields during boot. q4 is therefore an active compact configuration object, not the legacy giant Asahi Globals object.

### Direct host writes proved so far

`AGXFirmware::initFirmwareData`:

- +0x000 = 0, or 7 when accelerator config word +0x650 has bit 28 set.
- +0x004 = 0.
- +0x00c = 0.
- +0x014 = 0.
- +0x018 = 0.
- +0x038 = 0x78.
- +0x044 = 0.
- +0x01c = accelerator +0x2970 (dynamic).
- +0x028 = accelerator +0x660; exact J615 boot value is **0**. Base `AGXAccelerator::configureDevice()` zeroes the qword at accelerator +0x65c..+0x663, and later G15/G15G configuration only updates the packed +0x650..+0x656 feature word before `initPowerAndPerformanceData()` copies +0x660 into q4. Reproduction: `tools/g15_q4_028.py` / `g15-q4-028.out`.
- +0x078 = power-interface 0 / `gpu-max-power`; exact J615 value is **4070**. `configurePowerAndPerformanceController()` seeds accelerator +0x2288 with `0xfe6`; `setupConfig()` optionally overrides it from `gpu-max-power`, but exact J615 omits that property. `initPowerAndPerformanceData()` then calls `updatePowerInterfaceTarget(0, &fw+0x83c)`, whose index-0 arm stores the value directly at q4+0x078.
- +0x07c/+0x080 = normalized power-interface targets 1 and 2; exact J615 values are **100/100**. `setupConfig()` enables both with target `0x10000`; firmware +0x824 is max pstate index `gpu-num-perf-states-1 = 1`, HwDataB +0xb58 receives that same index, and `gpu-pwr-min-duty-cycle=30` cannot lower the full-scale normalized result of 100. Reproduction: `tools/g15_q4_power_interfaces.py` / `g15-q4-power-interfaces.out`.
- +0x05a = `((accelerator byte +0x9d08) & 3) == 0`; exact G15/J615 value is **1**. The G15 constructor zeroes the byte, `AGXAccelerator::start()` normalizes it to `(old & 0x30) | 0x44`, and later writers only alter bits 3 or 5, so bits 0..1 are mechanically zero when q4 is built.
- +0x02c = accelerator config word +0x650 bit 0. `AGXAccelerator::configureDevice()` explicitly ORs bit 0 before the smart-idle query and subsequent setup masks preserve it, so exact J615 value is **1**.
- +0x030 = accelerator config word +0x650 bit 1, the Smart Idle Off enable. This is independently confirmed by `AGXArmFirmware::setSmartIdleOffEnable(bool)`, which updates both accelerator bit 1 and q4 +0x030. The AGXG15G Info.plist has exactly three `gpu,t8122` personalities (`AGXAcceleratorG15G`, `_A0`, `_B0`), and all three resolve `halIsSmartidleOffEnabled()` to a literal return **1**. Therefore exact J615 boot q4 +0x030 is **1**. Reproduction: `tools/g15_q4_feature_bits.py` / `g15-q4-feature-bits.out`.

`AGXArmFirmware::initFirmwareData`:

- +0x008 = 0.
- +0x010 = 0.
- +0x034 = 0 in the property-enabled branch; zero allocation already supplies the other branch.
- +0x03c = 0.
- +0x040 = 0.
- +0x048 = 0.
- +0x04c = 0.
- +0x050/+0x052/+0x054 = three u16 values copied from accelerator +0x6c4/+0x6c6/+0x6c8.
- +0x056 = 0 (unaligned u32).
- +0x05e = inherited HwDataB value at +0xa6c after defaulting zero to 1.
- +0x7a8/+0x7ac = accelerator +0x19e0/+0x19e4.
- +0x7b0..+0x7c8 = accelerator +0x19e8..+0x1a00 in 4-byte steps.
- +0x7cc = float conversion of accelerator +0x1a04.
- +0x9ac = 0.
- +0x9b8 = 0.
- +0xde5 = 0 (unaligned).

`AGXArmFirmware::setupConfig`:

- +0x97c = accelerator +0x1e48 (u64).
- +0x984 = accelerator +0x1e50 (u64).
- +0x98c = accelerator +0x1e58 (u64).
- +0x994 = accelerator +0x1e60 (u32).

The source values above are themselves fixed by `AGXAccelerator::configureDevice` before the G15 subclass runs. Exact kernelcache constants are:

- accelerator +0x1e48 / q4 +0x97c = `0x0000000a00000028`.
- accelerator +0x1e50 / q4 +0x984 = `0x00000001000000fa`.
- accelerator +0x1e58 / q4 +0x98c = `0x0000006400000001`.
- accelerator +0x1e60 / q4 +0x994 = `1`.

`AGXAcceleratorG15::configureDevice` also proves the three 16-bit source values copied to q4+0x50/+0x52/+0x54: it writes accelerator +0x6c4 as `0x0028ffff` and +0x6c8 as `0xffff`, therefore the q4 values are exactly `0xffff`, `0x0028`, `0xffff` respectively.

There is also an Apple virtual setup callback invoked with accelerator +0x2974 after q4+0x1c is populated. Its exact q4 target fields remain to be reconstructed, so those fields stay opaque.

### Direct firmware-read offsets observed

Static firmware decompilation of all q4-alias users directly references at least:

`+0x000, +0x004, +0x008, +0x00c, +0x010, +0x014/+0x015, +0x018, +0x01c, +0x020, +0x024, +0x028, +0x02c, +0x030, +0x038, +0x044, +0x048, +0x04c, +0x052, +0x056, +0x05a, +0x05e, +0x062, +0x070, +0x074, +0x078, +0x080, +0x08c, +0x090, +0x094, +0x098, +0x09c, +0x0a4, +0x26f, +0x7a8..+0x7cc, +0x7d4/+0x7d8/+0x7dc, +0x96c/+0x974, +0x97c/+0x980/+0x984, +0x9a0..+0x9b8, +0x9dd, +0xde9/+0xded/+0xdf1/+0xdf5/+0xdf9`.

The firmware also conditionally copies a 0x200-byte table beginning at q4+0x9e5 when the q4+0x9dd value is nonzero.

### Exact J615 Apple DeviceTree cross-check

The project contains the exact Apple J615 ADT from build 23J220 at `reference/apple-j615-23J220/DeviceTree.j615ap.raw`. Parsing `/arm-io/sgx` with m1n1 confirms that all ten optional `gpu-idleoff-*` properties used to populate accelerator +0x19e0..+0x1a04 are absent on this machine. The decoded node is saved as `j615-apple-adt-sgx-properties.txt`.

The property-to-q4 mapping reconstructed from `AGXAccelerator::configureDevice` is:

- q4+0x7a8: `gpu-idleoff-standby-timer`
- q4+0x7ac: `gpu-idleoff-prob-init-val`
- q4+0x7b0: `gpu-idleoff-fn-hit`
- q4+0x7b4: `gpu-idleoff-fi-hit`
- q4+0x7b8: `gpu-idleoff-fn-miss`
- q4+0x7bc: `gpu-idleoff-fi-miss`
- q4+0x7c0: `gpu-idleoff-nei-hit`
- q4+0x7c4: `gpu-idleoff-min_conf`
- q4+0x7c8: `gpu-idleoff-high-conf`
- q4+0x7cc: float-converted `gpu-idleoff-reset-iter`

The earlier assumption that property absence implied zeros was incorrect. `AGXAccelerator::configureDevice()` first invokes virtual slot `+0xa08`, which resolves on the G15 vtable to `AGXAccelerator::configurePowerAndPerformanceController()`. That routine establishes Smart Idle defaults before any `gpu-idleoff-*` property probes. The base function then conditionally overwrites those defaults only when properties exist; `AGXAcceleratorG15::configureDevice()` calls the base implementation first. Because exact J615 omits all ten overrides, the defaults survive unchanged and `AGXArmFirmware::initFirmwareData()` copies them into q4.

Exact J615 q4 values are therefore:

- `+0x7a8` standby timer: `700` us
- `+0x7ac` probability init: `1.0`
- `+0x7b0` FN hit: `0.8`
- `+0x7b4` FI hit: `0.2`
- `+0x7b8` FN miss: `0.9`
- `+0x7bc` FI miss: `0.1`
- `+0x7c0` NEI hit: `0.25`
- `+0x7c4` minimum confidence: `0.7`
- `+0x7c8` high confidence: `0.9`
- `+0x7cc` reset iterations: `6`

The float defaults come directly from the constant vectors loaded by `configurePowerAndPerformanceController()` (`0xfffffe00071053c0` followed by `0xfffffe00071053b0`); the timer and reset count are immediate constants `700` and `6`. Reproduction: `tools/g15_q4_smart_idle_defaults.py` / `g15-q4-smart-idle-defaults.out`.

## Immediate implementation rule

- q21 can be represented as an exact typed 0x20-byte shared status object now; all known concurrency fields are aligned.
- q4 can be represented as an exact-size typed/packed skeleton, but dynamic accelerator-derived fields must remain zero/unpopulated until Linux-side sources are proven.
- Do not restore legacy `Globals` or `FwStatus` into the G15 root simply to satisfy existing driver code. Runtime paths that still depend on those legacy semantics must remain fail-closed.

## G15 q6..q20 UAT block

A structural cross-check closes another part of the 24-qword root. In the legacy Asahi root, the byte range starting at offset 0x30 is exactly:

- 4-byte UAT header (`page_size`, `page_bits`, `num_levels`),
- three 0x20-byte `UatLevelInfo` records,
- 0x14 bytes of zero padding.

Total: `4 + 3*0x20 + 0x14 = 0x78` bytes, exactly q6..q20 (15 qwords). G15's Apple host copies exactly q6..q18 (13 qwords / 0x68 bytes) from the accelerator firmware-mapper record at `*(accelerator+0x9b68) + 0x18`; q19 and q20 receive no host writes, matching the final 0x10 bytes of the legacy zero pad. This is a byte-for-byte structural correspondence, not merely the same total size.

The exact J615 Apple ADT `/defaults` node contains `uat-vaddr-size = 43`. Apple includes the TTBR0/TTBR1 selector bit in that number, so each TTBR has a 42-bit input range. This matches m1n1's G15 UAT split (`L0_OFF=42`) and means the shift-36 root level contains `2^(42-36) = 64` entries. The lower levels remain shift 25 / 2048 entries and shift 14 / 2048 entries.

The compile-only G15 root now models q6..q20 as an exact 0x78-byte typed UAT block and populates it with the established UAT descriptor builder using the 64-entry G15 root. Runtime G15 dispatch remains disabled.

## Exact J615 q4 +0x05e value

The previously dynamic-looking q4 `+0x05e` chain is now closed for the exact production J615 ADT.

Apple's static AGX constructor initializes `_gSharedProbeConfig+0x0a` to `2`. `AGXFamilyAccelerator::probe()` queries the provider property `model-slow`; when the property is absent it explicitly writes `2`, and when present it writes the boolean property value. The same probe then sets `_gSharedProbeConfig+0x12 bit4` exactly when `config+0x0a == 1`.

The exact J615 23J220 DeviceTree contains no `model-slow` property. Therefore the production J615 path has shared-probe bit 4 clear. `AGXFirmware::initFirmwareData()` consequently writes **1** to HwDataB `+0xa6c` (the alternative debug/model-slow value is 200). `AGXArmFirmware::initFirmwareData()` reads that word, defaults zero to one, and stores it unaligned at q4 `+0x05e`. Thus q4 `+0x05e = 1` exactly on this J615 path.

The compile-only G15 model now asserts that generated HwDataB `power_sample_period` lands at `+0xa6c`, initializes it to 1 for G15, asserts q4 `unk_05e` at `+0x05e`, and initializes q4 `+0x05e` to 1. Older generations retain their existing dynamic power-sample-period initializer. Reproduction: `tools/g15_q4_05e_recon.py` / `g15-q4-05e-recon.out`.

## Exact q4 +0x01c / +0x024 G15 zeros and FRG timeout identity

The q4 field at `+0x024` is now semantically identified. In `AGXFirmware::initFirmwareData`, accelerator `+0x2974` is passed to firmware-object virtual slot `+0x4a0`. The active ChinookV9 vtable resolves that slot exactly to `AGXArmFirmware::setFRGTaskTimeout(unsigned int)`, whose complete body loads q4 (`AGXArmFirmware+0x2b0`) and stores the argument at q4 `+0x24`. Thus q4 `+0x024` is the FRG task-timeout field, not q4 `+0x062` as an earlier tentative association suggested.

The source values are also exact zeros on G15. `AGXAccelerator::start()` clears the full 8-byte accelerator range `+0x2970..+0x2977`, then enters the only population path only when combined accelerator config bit 34 is set. Base `AGXAccelerator::configureDevice()` unconditionally clears bit 34 with mask `0xfffffffbffffffff`. `AGXAcceleratorG15::configureDevice()` calls the base function first; its later high-word OR constant `0x0000001020030940` leaves bit 34 clear, and its final high-word transformation only clears/reinserts bit 35. A whole-image direct-reference scan finds no other write to accelerator `+0x2970/+0x2974`.

Therefore, on the exact G15 path:

- accelerator `+0x2970 = 0`;
- accelerator `+0x2974 = 0`;
- q4 `+0x01c = 0`;
- q4 `+0x024 = 0` (`FRG task timeout`).

The compile-only model now explicitly initializes both zeros, names `+0x024` as `frg_task_timeout_024`, and asserts both byte offsets. Reproduction: `tools/g15_q4_timeout_recon.py` / `g15-q4-timeout-recon.out`.

## 2026-08-21: q4 control/performance setter map

Direct ChinookV9 vtable resolution now gives host-side semantics for additional q4 fields without assuming their boot-time values:

- +0x03c: `setCPMSWindowSize()` / `getCPMSWindowSize()`; Apple init explicitly clears it.
- +0x040: `setCPMSTFCASize()` / `getCPMSTFCASize()`; Apple init explicitly clears it.
- +0x048/+0x04c: the two arguments written by `setKickChannelQos()`; Apple init explicitly clears both.
- +0x070: `setCommandSubmissionEnabled()`.
- +0x090: `setPerformanceControllerTarget()`.
- +0x094: `setPerformanceControllerTransferOutput()`.
- +0x098: `setPerformanceBoostModeMinUtil()` value; +0x0a4 is its valid/enable byte.
- +0x09c: `setPerformanceBoostModeCEStep()`.
- +0x0a0: `setPerformanceControllerResetIters()`.

The exact J615 ADT exposes target utilization 85, boost minimum utilization 90, and boost CE step 50; `gpu-perf-reset-iters` is absent and Apple's setup fallback is 6. These property values are **not yet baked into the Linux q4 constructor**, because the exact pre-boot callback/application ordering has not yet been mechanically closed. The source only types the ABI fields and retains the existing safe zero base where values are not yet proven to be present before firmware boot.

## 2026-08-21: q4 engagement and keepalive defaults

The q4 words immediately after the Smart Idle block are now mechanically closed for exact J615. `AGXArmFirmware::enableUTEngagement(bool)` is a direct store to q4 `+0x7d0`, and `enableCLVREngagement(bool)` is a direct store to q4 `+0x7d4`. `AGXArmFirmware::initPowerAndPerformanceData()` seeds both fields with `1`, so the initial firmware-visible state enables both engagement paths. This also corrects the previous structural placeholder that treated `+0x7d0` as padding.

The same initializer copies accelerator `+0x1e90` to q4 `+0x7d8` and accelerator `+0x1e8c` to q4 `+0x7dc`. Base `AGXAccelerator::configureDevice()` establishes both accelerator fields at `100` before looking for optional overrides named `gpu-keepalive-perf-mode-threshold` and `gpu-keepalive-off-mode-threshold`. Exact J615 `/arm-io/sgx` contains neither property, therefore both values remain `100`.

Exact J615 q4 values:

- `+0x7d0` UT engagement enable = `1`
- `+0x7d4` CLVR engagement enable = `1`
- `+0x7d8` GPU keepalive performance-mode threshold = `100`
- `+0x7dc` GPU keepalive off-mode threshold = `100`

Reproduction: `tools/g15_q4_engagement_keepalive.py` / `g15-q4-engagement-keepalive.out`.

## 2026-08-21: q4 GVDM timer identity

`AGXArmFirmware::setGVDMTimerInterval(unsigned int)` is a direct store of its argument to q4 `+0x9ac`. `AGXArmFirmware::initFirmwareData()` explicitly clears that same word with `str wzr`, proving the initial G15 value is exactly zero. The compile-only q4 model now names the word `gvdm_timer_interval_9ac` rather than treating it as an anonymous zero. Reproduction: `tools/g15_q4_gvdm_timer.py` / `g15-q4-gvdm-timer.out`.

## 2026-08-21: exact q4 idle-off / early-wake delays

The initialization ordering for q4 `+0x9a0/+0x9a4/+0x9a8` is now mechanically closed.

`AGXFirmware::init(AGXAccelerator*)` invokes virtual slot `+0x150` before returning. On the active `AGXArmFirmwareChinookV9` vtable this is `AGXArmFirmware::setupConfig()`. `AGXArmFirmware::init()` calls that base initializer, so `setupConfig()` has completed before firmware-data initialization.

`setupConfig()` establishes these fallback/default values and clears the associated override storage/flags before probing optional properties:

- firmware `+0x1814`: GPU idle-off delay = **2 ms**;
- firmware `+0x1820`: Fender idle-off delay = **40 ms**;
- firmware `+0x182c`: firmware early-wake timeout = **5 ms**.

The exact J615 `/arm-io/sgx` node omits `gpu-idle-off-delay-ms`, `gpu-fender-idle-off-delay-ms`, and `gpu-fw-early-wake-timeout-ms`, so the defaults survive unchanged.

`AGXFirmware::initFirmwareData()` then tail-calls virtual slot `+0x170`, which resolves on ChinookV9 to `AGXArmFirmware::initPowerAndPerformanceData()`. That routine selects the default or override source from `+0x1814/+0x1818`, `+0x1820/+0x1824`, and `+0x182c/+0x1830` using flags `+0x181c/+0x1828/+0x1834`, then writes the selected values directly to q4.

Therefore the exact J615 initial q4 values are:

- q4 `+0x9a0` GPU idle-off delay = **2 ms**;
- q4 `+0x9a4` Fender idle-off delay = **40 ms**;
- q4 `+0x9a8` firmware early-wake timeout = **5 ms**.

The compile-only G15 model now names and initializes all three fields. Reproduction: `tools/g15_q4_idle_delays.py` / `g15-q4-idle-delays.out`.

## 2026-08-21: q4 progress-check interval record

The packed q4 source at `+0x97c..+0x994` is now split into its seven exact 32-bit words. `AGXAccelerator::configureDevice()` seeds accelerator `+0x1e48..+0x1e60` from two static constants plus a final immediate `1`; the exact word sequence is `[40, 10, 250, 1, 1, 100, 1]`. `AGXArmFirmware::setupConfig()` copies those 28 bytes directly to q4 `+0x97c..+0x994`.

Three words have direct setter identities:

- q4 `+0x97c` is `setProgressCheckInterval3D()` and starts at **40**;
- q4 `+0x980` is `setProgressCheckIntervalTA()` and starts at **10**;
- q4 `+0x984` is `setProgressCheckIntervalCL()` and starts at **250**.

The remaining exact words are retained as offset-oriented fields until their semantics are proven: `+0x988 = 1`, `+0x98c = 1`, `+0x990 = 100`, `+0x994 = 1`. The compile-only model no longer hides the first three intervals inside packed qwords. Reproduction: `tools/g15_q4_progress_intervals.py` / `g15-q4-progress-intervals.out`.

## 2026-08-21: exact initial q4 performance-controller bootstrap

The apparent J615 ADT performance values are **not** the initial q4 `+0x090..+0x0a4` image. Apple keeps a separate runtime backing block in the firmware object. `AGXArmFirmware::setupConfig()` explicitly clears firmware fields `+0x17a0`, `+0x17a4`, `+0x17a8`, `+0x17ac`, and `+0x17b0`; it also clears the `+0x17b4` valid byte through the alias `x23 = firmware + 0x156c`, `[x23 + 0x248] = firmware + 0x17b4`.

At entry to `AGXArmFirmware::initPowerAndPerformanceData()`, Apple loads 16 bytes from firmware `+0x17a0`, another 16 bytes from `+0x17b0`, and copies them directly to q4 `+0x090` and `+0x0a0` respectively. A scan of both the Arm and base `initPowerAndPerformanceData()` routines finds no dispatch through the performance setter/update slots during this bootstrap copy.

Therefore the exact initial q4 fields are:

- `+0x090` performance-controller target = **0**;
- `+0x094` transfer output = **0**;
- `+0x098` boost minimum utilization = **0**;
- `+0x09c` boost CE step = **0**;
- `+0x0a0` reset iterations = **0**;
- `+0x0a4` boost-min-util valid byte = **0**.

The exact J615 ADT still supplies target utilization 85, boost minimum utilization 90, and boost CE step 50, while reset-iters falls back to 6. Those values populate setup/config state and can later be applied through the dedicated setter/update path; they must not be mistaken for the q4 boot image. The compile-only builder now initializes the six boot fields explicitly to zero. Reproduction: `tools/g15_q4_perf_bootstrap.py` / `g15-q4-perf-bootstrap.out`.

## 2026-08-21: q4 display-power timestamp/interval

The two q4 words at `+0x96c/+0x974` are runtime display-power-management inputs, not boot-time ADT constants.

The host path is now mechanically closed:

- `IOGPUDeviceUserClient::s_set_display_params_for_gpu()` consumes two scalar user-client inputs.
- `AGXDeviceUserClient::set_display_params_for_gpu(unsigned long long, unsigned long long)` queues `AGXAccelerator::updateDisplayPMParams(unsigned long long*, unsigned long long*)` onto the accelerator workloop.
- `AGXAccelerator::updateDisplayPMParams()` dereferences both arguments and dispatches firmware vptr slot `+0x920` (the `__ZTV` data reference is at symbol-base `+0x930` because the live Itanium vptr starts at `__ZTV+0x10`).
- On every active Arm/Chinook vtable that slot resolves to `AGXArmFirmware::updateDisplayPowerManagementParams(unsigned long long, unsigned long long)`.
- That function stores the first input unchanged to q4 `+0x96c`; it clamps the second input to `100000..400000` and stores it at q4 `+0x974`.

Firmware imports q4 `+0x96c/+0x974` into `DAT_fffffc000010e968/e970`. A zero timestamp makes firmware retain its live schedule timestamp, while a zero interval makes it retain its live interval. When the interval override is nonzero it replaces the current interval and is used throughout the same timestamp arithmetic. Firmware converts this time domain with `/24000/1000`, confirming the 24 MHz clock. Therefore Apple's clamp corresponds exactly to:

- `100000` ticks = `4.166666... ms` = `240 Hz`;
- `400000` ticks = `16.666666... ms` = `60 Hz`.

The source now names these fields `display_pm_timestamp_96c` and `display_pm_interval_974`. Both remain explicitly zero in the compile-only initial q4 image because no display user-client update has occurred at boot. This preserves firmware's own fallback behavior instead of inventing a display timing value.

Reproduction: `tools/g15_q4_display_pm.py` / `g15-q4-display-pm.out`; host data-reference scan in `g15-display-pm-refs.log` and vtable snapshot in `g15-vt930.out`.

## 2026-08-21: q4 initial GPU perf-state cap

q4 `+0x08c` is the GPU performance-state cap in Apple's fixed-point `pstate * 100` representation, not a literal percent.

The exact J615 boot value is mechanically closed:

- `AGXAccelerator::start()` looks up the literal property `gpu-num-perf-states` and writes it as word 0 of the temporary GPU `PerfStateInfo`.
- Exact J615 `/arm-io/sgx` has `gpu-num-perf-states = 2`.
- The G15 constructor initializes accelerator bytes `+0x4e0..+0x4e7` from constant `00 00 00 00 01 00 00 00`, hence the MGPU selector byte `+0x4e1` is zero. Whole-AGX instruction scanning finds only reads of `+0x4e1` in this path.
- Therefore `AGXAccelerator::start()` takes its single-GPU branch and copies the exact 0x448-byte temporary `PerfStateInfo` to accelerator `+0xa1e0`; its first word remains `2`.
- `AGXAccelerator::getPerfStateCap(domain 0)` returns `*(u32 *)(accelerator+0xa1e0) - 1`, hence `1`.
- `AGXFirmware::setupConfig()` stores that result at firmware-object `+0x828`.
- `AGXArmFirmware::initPowerAndPerformanceData()` multiplies `+0x828` by 100 and writes q4 `+0x08c`.

Thus the exact initial J615 value is `100`. Runtime `AGXArmFirmware::updatePerfStateCap(OSObject*)` uses the same representation: it validates a pstate index, stores the raw index to firmware `+0x828`, multiplies by 100, and updates q4 `+0x08c`.

The source field is now named `perf_state_cap_x100_08c` and initialized to `U32(100)`.

Reproduction: `tools/g15_q4_perf_state_cap.py` / `g15-q4-perf-state-cap.out`.


### Keepalive override tail

The previously opaque q4 tail words now have exact host-side semantics from the Arm firmware getter/setter implementations:

- `+0xde9`: GPU keepalive mode override (`getGPUKeepAliveModeOverride()` / `setGPUKeepAliveOverride()`).
- `+0xded`: GFXC keepalive override (`setGFXCKeepAliveOverride()`).
- `+0xdf1`: GPU keepalive performance-mode threshold override. The getter falls back to the base threshold at `+0x7d8` when this override is zero.
- `+0xdf5`: GPU keepalive off-mode threshold override. The getter falls back to the base threshold at `+0x7dc` when this override is zero.

The compile-only q4 object remains zero-initialized for these runtime override fields; this milestone names the ABI and fallback behavior without inventing a boot-time override. Reproduction: `tools/g15_q4_keepalive_overrides.py` / `g15-q4-keepalive-overrides.out`.


### Soft-fault settings tail

q4 `+0xdf9` is the packed soft-fault settings word. `AGXArmFirmware::initFirmwareData()` invokes virtual slot `+0x710` with `true`; on ChinookV9 that is `initSoftFaultSettings(true)`, which dispatches to `updateSoftFaultSettings(true)` at slot `+0x718`. The updater sets bit 0 from the boolean, sets bit 1 only when disabled, clears bit 2, and finally masks to the low three bits. Therefore the exact J615 boot value is **1** (soft faults enabled). Firmware imports this word into `DAT_fffffc000010e920`. Reproduction: `tools/g15_q4_softfault.py` / `g15-q4-softfault.out`.
