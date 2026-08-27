# G15 Pipe Callback Gate

Research state: 2026-08-27

Target: J615 / T8122 G15G C0, RTKit-2419.140.12.release.

This note continues the transport closure in `G15-PIPE-SUBMISSION-BOUNDARY.md`. The signed empty Compute publication still advances the host TX WriteIndex without firmware retiring the entry. The new result is that the first-work boundary has been reduced to a specific RTBuddy firmware callback and its internal runtime-power gate.

## Stats tag 0x0f closure

A passive Linux decoder, validated against the reconstructed Apple producer layout, establishes G15 statistics tag `0x0f` as software-state telemetry:

- record `+0x04`: unaligned little-endian 64-bit timestamp
- record `+0x0c`: 32-bit software state

During the signed empty Compute publication firmware reports `state = 1` while the TX pipe remains `Read=0, CFI=0, Write=1` and eventually times out. The same state value was seen across earlier E032-E038 captures.

Therefore tag `0x0f/state=1` is not a QueueInfo-consumption acknowledgement and does not prove that the pipe callback reached the scheduler.

## Internal power-state byte

A complete xref pass over the G15 firmware reduces `DAT_fffffc000010e528` to 39 references in six functions. It is a compact runtime-power state byte, not q21's public `power_state` field.

The relevant pipe callback is `g15_pipe_work_callback` at `0xfffffc0000042ca8`. Its reconstructed decision is:

1. optionally emit KTrace `0x100` with the callback argument;
2. test `DAT_fffffc000010e528 & 0x78`;
3. if the mask is clear, call the power-kick helper and `FUN_fffffc0000006a0c(arg)`;
4. if the mask is set, arguments 1 or 2 may continue only when `FUN_fffffc000000cf58()` reports existing non-idle work;
5. otherwise skip the scheduler and optionally emit KTrace `0x207`.

The callback dispatch table binds arguments 0, 1 and 2. Compute maps to argument `2`.

The bootstrap value `4` is scheduler-admissible because `4 & 0x78 == 0`; values carrying bits from `0x78` represent transition/busy forms rather than a simple zero/nonzero gate.

## The `cf58` exception

`FUN_fffffc000000cf58()` scans sixteen firmware work-state slots. It returns zero only when every slot is state 0 or 5, and nonzero when at least one slot is in another state.

For the first empty Compute queue, the slots are expected to be idle. Thus the Compute arg-2 exception is not a generic wake bypass: if the internal power byte is in a blocked transition form, an otherwise-idle first submission can be prevented from entering `FUN_6a0c` entirely.

## KTrace provenance

The callback's `0x100` and `0x207` emissions are guarded by trace-mask bit 2. Firmware refreshes that mask from q21 `+0x00`, the Linux `G15SharedStatus.host_flags` word.

The trace emitter writes directly to the RuntimePointers KTrace state/ring at `+0x1d8/+0x1e0`. Linux already allocates, publishes, and polls the same 0x38-byte records.

The packed trace ID lives at record `+0x2c`. Under Linux's `RawKTraceMsg` layout:

- `0x100` -> `code=0x00, channel=0x01`
- `0x207` -> `code=0x07, channel=0x02`
- class-2 calibration event `0x213` -> `code=0x13, channel=0x02`

## E043 live result

E043 changed only instrumentation:

- q21 `host_flags` stayed zero through every pre-RTKit byte-exact validator;
- immediately before the production G15 `MSG_INIT`, bit 2 was set;
- Linux surfaced only selected KTrace callback IDs at info level.

Candidate module SHA-256:

`7a03f841f67d4ccfd7943453968fcb093c29f5c79ec0328c36f06190315f61c1`

The candidate booted normally, registered DRM and exposed `renderD128`. The signed root-level empty Compute probe then reproduced the established transport boundary:

- q4 command-submission gate `0 -> 1`
- Compute/2 host pipe `(0,0,0,0) -> (0,0,1,1)`
- tag `0x0f`, state `1`
- bounded timeout with final pipe `(0,0,1,1)`
- ReleaseResource fail-stopped with `ENODEV`
- QueueInfo backing retained
- no RTKit crash, GPU firmware crash, DART/IOMMU fault, panic or oops

Neither selected `0x100` nor `0x207` was observed. That negative result alone does not prove the callback was skipped because the enabled KTrace class had not yet been independently calibrated live.

## E044 discriminator

E044 adds only the known class-2 `0x213` record to the info-level selector. Its built diagnostic module SHA-256 is:

`804eb61042d9949d166c1979a175fde34af4f804e0c19e3d45135c197478d949`

Interpretation of the next one-shot signed publication is deliberately narrow:

- `0x213` present, no `0x100`: KTrace class is live, but the Compute doorbell is not reaching `g15_pipe_work_callback`.
- `0x100` plus `0x207`: callback is reached, but the internal runtime-power gate blocks scheduler entry.
- `0x100` without `0x207`: callback gate passes; the remaining stall is inside `FUN_6a0c` or below.
- no `0x213`: trace-mask timing/transport must be calibrated before drawing a callback conclusion.

No direct PMGR write, real command-buffer submission, QueueInfo ABI change or guessed power-state poke is justified by this checkpoint.

## Rejected inferences

Two attractive but incorrect interpretations were explicitly rejected during this pass:

- `FUN_fffffc00000231bc` is a power-side helper, not the QueueInfo consumer.
- a decompiler-implied relation from `FUN_3d330` to `FUN_18864` does not survive assembly/xref checking; `FUN_18864` is a startup power-data initializer.

The remaining live question is no longer a broad "GPU power" hypothesis. It is the exact control-flow location of the first Compute doorbell relative to `g15_pipe_work_callback` and `FUN_6a0c`.
