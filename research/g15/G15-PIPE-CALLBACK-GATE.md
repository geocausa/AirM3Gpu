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

The callback dispatch table binds arguments 0, 1 and 2 to firmware wake/dispatch classes. These arguments are **not PipeType**; `FUN_6a0c` subsequently scans all priorities and all V/F/C rings.

The bootstrap value `4` is scheduler-admissible because `4 & 0x78 == 0`; values carrying bits from `0x78` represent transition/busy forms rather than a simple zero/nonzero gate.

## The `cf58` exception

`FUN_fffffc000000cf58()` scans sixteen firmware work-state slots. It returns zero only when every slot is state 0 or 5, and nonzero when at least one slot is in another state.

For the first empty Compute queue, the slots are expected to be idle. Thus the arg-1/arg-2 exception is not a generic wake bypass: if the internal power byte is in a blocked transition form, an otherwise-idle first submission can be prevented from entering `FUN_6a0c` entirely.

## KTrace provenance

The callback's trace guards are distinct: entry/exit class `0x100/0x101` is guarded by trace-mask **bit 1**, while blocked-path `0x207` is guarded by **bit 2**. Firmware refreshes the mask from q21 `+0x00`, the Linux `G15SharedStatus.host_flags` word.

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

## E044 live calibration and guard correction

E044 added the known bit-2/class-2 `0x213` record to the info-level selector. Its diagnostic module SHA-256 was:

`804eb61042d9949d166c1979a175fde34af4f804e0c19e3d45135c197478d949`

The one-shot boot was healthy and the signed empty Compute probe again produced `WriteIndex 0 -> 1`, tag `0x0f/state=1`, no Read/CFI retirement, and a bounded fail-closed timeout. The calibrated stream contained 607 selected `0x213` records and zero `0x207` records.

An exact assembly re-check then caught an instrumentation mistake in the initial interpretation: callback-entry event `0x100` is guarded by internal trace-mask **bit 1**, not bit 2. E043/E044 set only q21 `host_flags` bit 2. Therefore zero observed `0x100` records in E044 is expected regardless of whether `g15_pipe_work_callback` ran.

E044 proves that q21 bit 2 reaches firmware, the KTrace ring is live, and the packed ID decoder is valid for the class-2 records. It does **not** prove callback absence. Zero `0x207` means only that no visible bit-2 blocked-path record was produced; this is compatible with either (a) the callback not being invoked, or (b) the callback being invoked and passing its internal power gate.

The correct next discriminator is E045: enable only q21 trace bits **1 and 2** (`host_flags=0x6`) after the existing pre-RTKit exact validators, keep `0x100`, `0x207`, and `0x213` selected at info level, and run one signed empty Compute publication. Then:

- `0x100` + matching `0x207`: callback arrives and the internal power gate blocks scheduler entry;
- `0x100` without `0x207`: callback arrives and passes the outer gate; the next boundary is after the gate;
- calibrated `0x213` with no `0x100`: first-work path stalls before `g15_pipe_work_callback`.

No PMGR or internal power-state poke is justified before this corrected trace experiment.


## E045 live closure

E045 enabled only q21 trace bits 1+2 (`host_flags=0x6`) after all pre-RTKit exact validators and before `MSG_INIT`. Diagnostic module SHA-256:

`e4a119856859dc00edef086d4dfc82e6f62ede7dd2ae0f33d7df32d750bf9c11`

The one-shot boot remained healthy and one signed empty Compute/priority-2 publication reproduced the known transport timeout. The selected trace capture contained:

- `0x100`: 1 record, `args[0]=1`
- `0x207`: 0 records
- `0x213`: 121 records

`0x100` is the shared `g15_pipe_work_callback(param_1)` entry marker. Static reconstruction of `FUN_6a0c(param_1)` establishes that this argument is a dispatch/wake class, not PipeType: once entered, the scheduler walks all four priorities and all three vertex/fragment/compute rings. Therefore callback arg 1 is not evidence of Fragment misrouting.

The missing `0x207` is decisive in combination with callback entry: the callback is invoked and does not take its outer `DAT_fffffc000010e528 & 0x78` / `FUN_cf58()` blocked path. It proceeds through the power-kick helper to `FUN_fffffc0000006a0c`.

E045 moves the failure past RTBuddy's outer callback power gate. Firmware already provides passive bit-1 scheduler traces suitable for the next discriminator: `0x112` channel-index snapshots, `0x111` accepted RunWorkQueue records, and `0x128` bounded scan/exhaustion markers.

No queue ABI, PMGR, power-state, or work-doorbell semantic change is justified before those existing traces are observed.

## Rejected inferences

Two attractive but incorrect interpretations were explicitly rejected during this pass:

- `FUN_fffffc00000231bc` is a power-side helper, not the QueueInfo consumer.
- the earlier assumption that callback argument 2 denotes Compute is false; callback arguments are wake/dispatch classes.

A later exact assembly pass also corrects an earlier xref note: `FUN_3d330` **does** directly call `FUN_18864` at `0xfffffc000003d458`. `FUN_18864` remains a startup power-data initializer, but the call is part of the cold-power transaction and must not be discarded from the path.

## E046/E047 control — boundary moves into `FUN_3d330`

E046 surfaced `0x100/0x101/0x111/0x112/0x128` while trace bits 1+2 were enabled. It observed one `0x100` and none of the later selected records, but class-2 `0x213` traffic could pressure the 512-entry KTrace ring.

E047 repeated the experiment with q21 `host_flags=0x2`, enabling bit 1 only. Diagnostic module SHA-256:

`a46184ba4274770ff0e51b05619ecbcab15611294121b3a4a0ca436ee9c9d9a2`

The one-shot candidate remained healthy. One signed empty Compute/priority-2 publication again advanced host WriteIndex to 1 and timed out with Read/CFI at zero. With class-2 traffic removed, the trace counts were:

- `0x100` callback entry: 1 (`args[0]=1`)
- `0x101` callback exit: 0
- `0x111` accepted RunWorkQueue: 0
- `0x112` scheduler/index snapshot: 0
- `0x128` bounded scan: 0
- `0x12e` reset marker: 0

The same counts remained unchanged more than one minute later. Exact callback control flow calls `FUN_3d330()` immediately after the gate and before `FUN_6a0c()`. Therefore the current non-returning boundary is the synchronous **`FUN_3d330()` cold power/setup transaction**, before scheduler entry.

Stats tag `0x0f/state=1` is generated by a separate pstate/statistics helper and proves a software power transition occurs during the wedge, but it is not enough to identify the blocking sub-call. The next discriminator is passive host-visible q21/q4 state correlation at EP20 receive times.
