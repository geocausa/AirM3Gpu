# G15 QueueInfo +0x10 / timestamp-queue state — E125

Exact macOS 14.8.3 / 23J220 reconstruction corrects an inherited G15 QueueInfo semantic. Command queue `+0x828` is `AGXTimeStampQueue`; `chooseCLWorkQueue()` loads its selected GPUVA at object `+0x28` and passes that value as the fifth CL-channel constructor argument. `AGXChannel::init()` stores it at channel `+0xa8`, and `resetChannelState()` converts/publishes it at selected QueueInfo `+0x10`. Therefore G15 QueueInfo `+0x10` is the selected timestamp-queue state FWVA, not the ordinary queue-wide `NotifierList`.

Firmware initializes `_AGFITimeStampQueue` at firmware `+0xb60` with eGartRange 7 and element size `0x18`. The exact J615 resource-stack backing is one `0x4000` page containing `0x2aa` complete states with `0x10` trailing bytes. Range 7 gives outer option `0x700000007`, compact UAT `0x007`, and exact leaf `0x00c0000000000447`.

`AGXTimeStampQueue::resetTimeStampQueueState()` clears all 0x18 bytes, writes the selected state's own converted FWVA at `+0x08`, writes `(mode == 2)` at `+0x10`, and leaves `+0x14` zero. The global resource stack chooses/releases a global index; no CL-global-index-zero assumption is made.

Linux E125 renames only the private selected-channel-state input to `timestamp_queue_state_fwva`, adds an unreachable exact range-7 timestamp backing/reset model, and records that the inherited raw `notifier_list` name is not G15 semantic authority. The live WorkQueue notifier allocation/constructor remains deliberately unchanged and G15 execution remains fail-closed.
