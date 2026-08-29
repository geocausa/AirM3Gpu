# G15 Compute error/recovery retirement — E090

Exact-target static closure for macOS 14.8.3 / 23J220. No Linux RunCompute, module install, or custom GPU command was issued.

## Recovery ingress

Firmware recovery is stamp/descriptor aware. A firmware event-ring recovery case calls `IOGPUScheduler::signalHardwareError()` with restart request 1 and the firmware error code (or `0x80` fallback). `AGXArmFirmware::processFirmwareInitiatedRecovery()` resolves firmware-provided `{stamp index, stamp value}` back to the owning WorkQueue and exact in-flight command descriptor. It records recovery/error state but does not free the descriptor, context, or UMA resources in place.

## Active AGX restart path

`AGXWorkQueue::restart()` overrides the generic IOGPU WorkQueue restart and delegates under the AGX command gate to `AGXAccelerator::restartWorkQueue()`. The generic IOGPU `nopCommand()` restart loop therefore exists but is not the active G15 recovery proof by itself.

The exact AGX restart state has a control byte at `+0x08`. Observed values `2` and `4` bypass the later global forced-stamp path; E090 deliberately does not invent enum names for those values.

On the destructive branch, `AGXAccelerator::restartWorkQueue()` calls `IOGPUEventMachine::forceCompleteAllStamps()`. Exact IOGPUFamily shows `forceCompleteStamp()` writes the target stamp value, calls `signalStamp()`, and invokes `IOGPU::signalStampsUpdated()`. This converges onto the normal E088 scheduler/prune path rather than introducing a second descriptor-completion ABI:

`forced stamp -> signalStamp -> signalStampsUpdated -> submit-event test/prune -> AGXCLCommandDescriptor::complete() -> command ContextID + UMA/HardwareBuffer retirement -> generic retained-memory completion`

The force-complete call itself does not synchronously call the descriptor `complete()` method; it advances event state so ordinary scheduler/prune retirement can run.

## Separate recovery drains

Destructive recovery also sanitizes reusable state outside base descriptor completion. It drains cached context/GART bindings, increments their generation bytes, unregisters/returns context IDs, resets UMA FLists, and resets busy CL/3D channel state.

`AGXUMAFList::resetPagePoolState()` explicitly drains outstanding async-grow/request ownership. Each pending request releases its HardwareBuffer reference and request-local context ID before destruction. The function also checks the FList's sticky HardwareBuffer ID; if the manager mapping still names this FList with refcount zero, that binding is returned to the manager free state and FList `+0x10` becomes `-1`. The FList prepared flag is cleared as well.

Thus recovery must not be modeled as blindly dropping FList mappings or IDs on every GPU error. A future Linux G15 implementation needs explicit restart mode handling, ordinary descriptor retirement for forced stamps, and separate idempotent cleanup for cached context state, async-grow/request references, sticky HardwareBuffer ownership, page-pool state, and channel state.

## Boundary

Normal completion and destructive recovery ownership are now statically closed. Remaining first-RunCompute prerequisites include the exact 36-state event-control owner at `+0x14`, first-activation FList population/HardwareBuffer ownership, HWMetrics, and the exact G15 SKU execution-stream producer. Runtime G15 submission remains fail-closed.
