# G15 Queue Registration Lifecycle — J615 / T8122

Research checkpoint: 2026-08-27

This note records the first clean end-to-end G15 scheduler registration and resource-retirement lifecycle reached on J615. It contains derived facts only. No Apple firmware, kernelcache, built module, or raw proprietary capture is included.

## Safety boundary

The live probe still does **not** expose a RunVertex, RunFragment, or RunCompute command and does not publish a shader command stream. E059/E060 make exactly one firmware `Barrier` / command type 4 record visible so the scheduler can establish its stamp bookkeeping for a newly registered QueueInfo. The barrier waits on an already-satisfied stamp value and is used only to exercise scheduler/DAG bookkeeping.

Normal render/compute workload execution remains disabled.

## DPE leakage-update ABI closes the pre-scheduler blocker

E056 corrected a major host-layout mistake. The 0x54-byte HwDataA block at `+0x4188..+0x41db` is not the sparse SoCHot structure previously assigned there. On G15G, accelerator vtable slot `+0xcb8` is `populateDPELeakageUpdateConfig`. Apple copies that image into HwDataA and mirrors the same state into q23 `+0x158..+0x1a8`.

For J615/C0 the mechanically reconstructed values include:

- HwDataA `+0x41d8 = 1`;
- q23 `+0x1a8 = 1`;
- repeated `0x30d4` and `0x0bb8` DPE constants in the surrounding 0x54-byte image.

Firmware power-dispatch case `0x14` uses `+0x41d8` when arming its DPE timer. With the old zeroed Linux tail, this path could retrigger at extremely high rate. Restoring the exact image collapses that pathological dispatch stream and lets the first work callback return from its cold-power setup.

## E057 — scheduler entry is reached

With the DPE image corrected, the bounded first Compute publication crosses the long-standing scheduler boundary:

- callback `0x100` is observed;
- q21 `busy` changes `0 -> 1`;
- scheduler snapshot `0x112` records appear;
- one `0x111` record accepts the published Compute/2 QueueInfo;
- firmware advances the pipe's CFI/write-shadow state.

The first new crash is then deterministic: `FUN_1fec0(pipe=2, slot=0x80)` dereferences a null per-stamp state pointer. Apple's `IOGPUChannel::stampIndex` starts at sentinel `0x80`, but real submission setup allocates a valid stamp index before scheduler-visible publication.

## E058 — a valid number is not enough

Replacing the sentinel with EventManager slot 0 avoids the invalid index but still crashes at the same null-state dereference. Static reconstruction shows why: firmware has an internal per-slot pointer table, and simply choosing a valid slot number does not bind the slot to QueueInfo backing.

`FUN_1e910` returns immediately for the old `wptr=0` empty registration, so no command parser runs to establish that mapping.

## E059 — barrier-only stamp binding

RTKit's G15 barrier helper `FUN_1f268` supplies the missing registration edge. When a type-4 Barrier is parsed it binds:

`per_stamp_state[runwork_stamp_slot] = QueueInfo + 0x18`

QueueInfo `+0x18` is the already allocated Linux `gpu_buf`; no new firmware allocation is required.

E059 therefore publishes one byte-exact Barrier at QueueInfo ring entry 0 and sends RunWorkQueue with `wptr=1`, EventManager slot 0, `wait_value=0`, and the normal next self-stamp. The live result is the first complete scheduler registration:

- `0x111` accepts Compute/2 QueueInfo;
- callback exit `0x101` appears;
- the pipe reaches `(Read, CFI, Write, Shadow) = (1,1,1,1)`;
- QueueInfo registration returns success;
- firmware initializes the context/resource bytes to `(02,00,00,0f)`.

This closes the E057/E058 null stamp-state crash without executing GPU work.

## E060 — native ReleaseResource closes cleanly

E059 exposed one independent control-plane bug during immediate teardown. Linux's G15 `ReleaseResource` payload shape matched Apple, but the versioned Rust enum retained a legacy V13.3 `Unk0d` placeholder. That shifted the G15 variant's discriminant from Apple's opcode `0x11` to `0x12`.

The E059 crash registers prove the misdecode mechanically. Firmware case `0x12` interpreted the packed context bytes as a pointer and faulted at `0xfffffc0000007aec`.

Apple's exact G15 `submitReleaseResource()` record is:

- `+0x00`: opcode `0x11`;
- `+0x04`: zero;
- `+0x08..+0x0b`: context identity bytes;
- `+0x0c`: unaligned firmware context/resource pointer;
- remainder zero.

E060 removes the legacy `Unk0d` slot from G15 enum numbering only and adds a fail-closed pre-send assertion that the generated tag is exactly `0x11`. Non-G15 numbering is unchanged.

The one-shot E060 run then completes the full bounded lifecycle:

- Barrier-only QueueInfo registration succeeds;
- callback and scheduler enter and return;
- pipe retirement reaches `(1,1,1,1)`;
- `ReleaseResource` tag check reports `0x11`;
- native `ReleaseResource` returns success;
- the temporary command-submission gate returns `1 -> 0`;
- software-state telemetry later returns from state 1 to state 0;
- no RTKit crash, GPU firmware crash, DART/IOMMU fault, kernel oops, or panic occurs.

E060 diagnostic module SHA-256:

`a516262736853a707acf16f7639d9f6a7da40d671aa61e5713bcc00cd2c7c421`

The diagnostic module and instrumentation are not part of the clean public Linux patch series.

## Current boundary

The pre-scheduler power/DPE blocker, QueueInfo scheduler registration, stamp-state binding, pipe retirement, and native context/resource release are now closed for the bounded registration probe.

The next step is static reconstruction of the smallest real G15 scheduler command contract. Live testing should remain below shader/render execution until every command-side pointer, stamp, dependency, and completion prerequisite is mechanically accounted for.
