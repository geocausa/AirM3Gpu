# J615 shared UMAPool retain/replacement lifecycle — E137

E137 closes the exact macOS 14.8 / 23J220 lifetime semantics behind the E135 shared-pool placement and E136 ownership split. It is static/read-only research only: no Linux source change, no module installation and no RunCompute/custom GPU command.

## Shared-container slots are weak pool references

Normal `AGXChannel::init()` takes `AGXUMASharedPoolContainer +0x40`, selects one of four pool slots at `+0x48/+0x50/+0x58/+0x60`, and examines the pointer while holding that lock.

For a non-null slot it calls `0xfffffe000aa15238(pool)`. Exact ARM64 proves that helper adds `0x0c` to the object pointer and tail-calls an atomic loop which:

- returns false without mutation if the 32-bit reference count is zero;
- otherwise CAS-increments the nonzero count;
- retries on a racing update and returns true only after successful promotion.

This is mechanically a nonzero-only atomic reference acquisition / try-retain operation. If promotion fails, channel setup creates a replacement pool and overwrites the selected slot while still under the container lock.

The exact `AGXUMAPool` vptr address point is `0xfffffe0007c3a930`; inherited vslots `+0x20/+0x28` resolve to the unconditional atomic reference increment/decrement helpers respectively.

## The shared pool owns the container reference

A newly created shared pool stores its `AGXUMASharedPoolContainer*` at pool `+0xe0`. After publishing the new pool into the selected weak slot, channel setup retains the container.

`AGXUMAPool::cleanup()` later releases the object at pool `+0xe0` and zeros that field. Conversely, `AGXUMASharedPoolContainer::free()` performs no traversal or release of the four pool slots; it immediately dispatches to superclass free.

The ownership direction is therefore pool -> container. The container slot does not keep the pool alive, which is why slot reuse requires try-retain under the lock.

## Channel-direct references are separate from the active-pool epoch

After channel `+0x188` receives its direct pool reference, channel setup takes accelerator lock `+0x400` and increments pool `+0x20`. On the `0 -> 1` transition it adds one extra pool retain and links pool `+0x10/+0x18` into the accelerator list rooted at `+0x410`.

`AGXAccelerator::removeUMAPool(pool)` performs the exact inverse: under accelerator `+0x400` it decrements pool `+0x20`, and on `1 -> 0` unlinks that intrusive node and releases the extra active-epoch reference. Channel teardown then separately releases its direct pool reference from channel `+0x188`.

Thus one shared active-epoch retain is held while at least one channel uses the pool, in addition to each channel's direct object reference.

## Last-reference finalization clears only the matching weak slot

For shared pools (`pool +0xa0 != 0`) with a container at `pool +0xe0`, `AGXUMAPool::finalize()` takes the container lock and accelerator UMAPool-list lock. It compares the finalizing pool against all four weak slots. If a slot still points to this exact pool, it max-folds pool accounting `+0x38/+0x40` into the matching container accounting pair and zeros only that pointer.

It then unlinks the pool from the separate accelerator UMAPool list protected by accelerator `+0x9530`, unlocks both locks, and continues superclass finalization. A zero-ref old pool may therefore be replaced safely: the old finalizer cannot erase a newer pool because it checks pointer identity before clearing the slot.

`AGXUMAPool::free()` dispatches vslot `+0x158`, exactly `AGXUMAPool::cleanup()`, before superclass free; cleanup releases the FList resources and the retained container reference.

## Linux consequence

The E136 shared Compute pool cannot safely become a simple strong `Option<G15SharedComputeUmaPoolOwner>` owned by a container. Apple's slot is weak, promotion is conditional on a nonzero object reference count, channel-direct references are distinct from the active-pool epoch retain, and finalization clears a slot only when pointer identity still matches.

The next Linux model must preserve those properties or implement an explicit equivalent state machine before the shared Compute pool/FList path can become live. E137 therefore intentionally makes no Linux source change. Live `submit_compute()` and RunCompute remain unchanged/fail-closed.
