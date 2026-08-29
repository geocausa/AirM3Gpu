# G15 Compute completion stamps and event-control ownership — E088

Exact target: macOS 14.8.3 / 23J220 AGXG15G plus matching KDK IOGPUFamily. Public m1n1 was used only to choose semantic questions; every relationship below was re-established independently on 23J220.

## RunCompute event-control pointer

`AGXCommandBuffer` maintains 36 rotating states. Its exact constructor allocates 0xc0-byte per-state control blocks and stores parallel CPU/GPU address arrays. The CPU pointer is used by `nextCommandBufferState()` to zero and repopulate event/submission state; the parallel GPU address is copied by `AGXCommandDescriptor::loadCommandBufferData()` to descriptor `+0x148`.

G15 `AGXCLChannelSKU::submitBuffer()` converts descriptor `+0x148` through the firmware GPUVA→FWVA hook and publishes the result at RunCompute `+0x14`. Therefore `+0x14` is the FWVA of this rotating **event-control block**. It is distinct from the generic base-descriptor submit-completion event.

The older public m1n1 `EventControl` concept is structurally consistent with this result, but no m1n1 offset or layout is used here.

## Stamp production

`AGXCommandQueue::processComputeSetup()` emits a normal IOGPU stamp command through the Compute channel. Exact IOGPU vtable resolution identifies `IOGPUChannel::writeStampCommand()` and `IOGPUChannel::setEventStamp()`. The channel stamp index is stored at channel `+0x18`; the G15 event-machine `writeStamp()` implementation emits the exact `{stamp_value, stamp_index}` record.

The same stamp index is copied to the CL descriptor and passed to `IOGPUCommandDescriptor::setSubmitEventStamp()`, which stamps the generic submit-completion event and attached fences. `addComputeToWorkqueue()` separately stamps queue-local/descriptor-local events used by command-queue bookkeeping. These event objects must not be collapsed into one field simply because they share a stamp index.

## Firmware notification to scheduler completion

The firmware event-ring stamp-update case contains four 32-bit bitmaps. Set bits call `IOGPUEventMachine::signalStamp()` using stamp-index bases 0x00, 0x20, 0x40 and 0x60. Once the ring is drained, any stamp activity causes fence notification, `IOGPU::signalStampsUpdated()`, and a global stamp test. `signalStampsUpdated()` directly kicks `IOGPUScheduler::kick_completed()`.

The firmware interrupt therefore wakes/kicks completion processing; it does **not** directly retire Compute resources.

## WorkQueue retirement gate

`IOGPUWorkQueue::pruneRingBuffer()` tests each submitted descriptor's generic submit event in ring order. Only a completed event allows the descriptor's `complete()` virtual to run, after which the ring slot is released and the read pointer advances.

For Compute, `AGXCLCommandDescriptor::complete()` first retires the managed context ID and the UMA/HardwareBuffer reference, resets their descriptor IDs to -1, then invokes generic `IOGPUCommandDescriptor::complete()` for retained prepared memory.

Combined with E087, the normal ordering is therefore:

`firmware stamp update → scheduler completion kick → submit-event test → AGX Compute resource retirement → generic retained-memory completion → WorkQueue slot retirement`.

This closes the normal success-path stamp/completion ownership. Error/recovery retirement remains a separate target; no live RunCompute is enabled by E088.
