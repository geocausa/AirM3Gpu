# Public m1n1 AGX semantic cross-check — E084

E084 is a **concept-only** cross-check against current public Asahi `m1n1`. It does not import an older firmware layout into the J615/G15 driver. Exact macOS 14.8.3 / 23J220 evidence remains authoritative for every G15 field, offset, size, opcode, mapping class and lifetime.

## Public snapshot

The reproducible source snapshot used here is AsahiLinux/m1n1 `main` commit `d7df51a586a82f8fed9344cb1646ac5a8aee32ed` (2026-08-28). Its AGX generation vocabulary contains `G13`, `G14` and `G14X`; no public `G15`, T8122/J615 or M3 AGX marker was found. The only remotely named GPU WIP branch found, `lina/gpu-wip` at `11163540ee025d996f4adf45a40b91d6290ec605`, is older and likewise supplied no G15 marker. This note therefore uses public structures only as a semantic dictionary.

## Compute start/finalize and SKU lifecycle

Public m1n1 models Compute with separate Start and Finalize firmware commands. Across those records recur a command-queue backpointer, context identity, tracking UUID, counters/statistics, event/stamp state and final retirement/restart state. Newer public `G14X` definitions also move the Compute work item toward a RegisterArray-driven interface instead of the older embedded `ComputeInfo` form.

That is useful context for G15 because exact 23J220 has independently established a RegisterArray and an execution stream at RunCompute `+0x760`. It is **not** evidence that G15 retains the old Start/Finalize binary layout. The exact G15 stream remains authoritative.

Public m1n1 also documents the common firmware-side SKU sequence as `Start -> Timestamp -> Wait for interrupt -> Timestamp -> Finish -> End`. Exact 23J220 independently gives G15 `type-b -> timestamp -> Compute WFI -> timestamp -> type-c -> finish`. The structural resemblance makes **Start/setup-class** and **Finalize/retirement-class** the highest-value hypotheses for future decoding of G15 type-b/type-c payload semantics. No old opcode, size or field position is transferred.

## Stamps and events

The public older-generation `JobMeta` family has separate host/FW stamp pointers, a stamp value, slot, event-control index, UUID and queue-command ordering state. `EventControl` separately owns event counts, submission state, a VM slot, per-engine stamp arrays and linked-list membership. Public trace names include `StampUpdateCL`, `Postproc`, `EvtComplete` and `EvtDequeued`.

This strongly corroborates the **concept** already independently visible in exact G15 `G15JobMeta`: host stamp, firmware stamp, stamp value, slot, event-control index, UUID and a queue-local event sequence. It directs the next completion RE toward post-processing/dequeue and event-list lifetime. It does not justify renaming G15 `event_seq` to an older `queue_cmd_count` without a 23J220 producer proof.

## Buffer and page-pool state

Public `BufferManagerInfo` exposes long-standing Apple GPU concepts including a persistent manager identity, page-list address/size, page count, block/secondary-list state, page cursors, block size and a grow counter. A public grow event also carries VM identity, buffer-manager identity and a sequencing counter.

G15's exact UMA/FList ABI is clearly redesigned, but this comparison avoids treating its Page Pool List, Backup Page List, Page-Pool State, HardwareBuffer ID and async-grow tokens as unrelated mystery blobs. Conversely, the old TVB field names are **not** copied into the 0x70 G15 state. In particular, exact G15 state `+0x20/+0x24/+0x28` remains conservatively cursor/state terminology until 23J220 proves direction and ownership.

## Context identity

Public m1n1 work/Start/Finalize records carry a command `context_id`, while `EventControl` has a separate `vm_slot`. That supports keeping two conceptual domains separate in G15: the exact managed `(context_id, generation)` used for execution and notifier/event VM bookkeeping. No public generation-byte analogue is assumed.

## Timestamps, counters and statistics

Public AGX structures contain several independent timing/counting domains simultaneously: job start/end timestamp storage, explicit Timestamp SKU commands, Start/work counters, queue submission timing, KTrace timestamps, buffer-manager counters and statistics pointers. This is a useful warning against naming an unknown G15 qword merely because it changes monotonically.

For G15, field classification should continue to start with writer, reset point, pairing, width and completion/recovery use. Exact 23J220 already distinguishes SKU timestamps, command timestamp pointers, context-store request/completion timestamps, HWMetrics and queue/event sequencing.

A useful negative result is that current public m1n1 still leaves Compute statistics (`stats_cp`) opaque. There is no mature public semantic map to borrow for G15 Compute stats.

## Queue state

Public `CommandQueuePointers` separates GPU done/read cursors, CPU write cursor and ring size. Public `CommandQueueInfo` adds multiple GPU read pointers, event ID, priority, UUID, busy/has-commands/inflight state and a GPU context. Queue submission messages carry queue type/address, head, event number, new-queue state and a timestamp.

This is strong concept-level support for the exact G15 QueueInfo/RingState reconstruction already in Linux. It also makes queue sequencing/scheduler/busy/inflight state the preferred hypothesis family for the still-generic 23J220 command snapshots copied from AGXCommandQueue. Those fields should be resolved by exact mutations around submit/register/retire, not by matching old offsets.

## Resulting priority

The cross-check changes no G15 ABI. The next work remains:

1. finish the exact range-8 special-aperture q22 map/unmap encoding;
2. complete the exact FList persistent-resource owner;
3. close stamp/notifier post-processing, completion, dequeue and recovery ownership;
4. decode G15 type-b/type-c SKU payloads using Start/Finalize **concepts** as hypotheses only;
5. classify remaining queue snapshots/counters from exact writers and lifetime transitions.

No live RunCompute is authorized by E084.
