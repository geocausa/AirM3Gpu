# J615 unbound private VM mapping (E162)

E162 advances one boundary beyond the host-only VM/GEM lifecycle by allowing ordinary `VM_BIND`/unbind against the private G15 bank-0 VM root. This path edits only the client's private TTBR0 page tables through `mmu::Vm::bind_object()` / `unmap_range()`.

Firmware context publication remains later and separate: `GpuManager::bind_vm()` is still reachable only from Queue-side code, and Queue creation remains gated. Special shared-object binding, accelerator-shared bank-1 mappings, q22 runtime publication, QueueInfo/CL-channel construction, and submission are therefore still unreachable.

The one-shot candidate live-tested a 16 KiB RW mapping at GPU VA `0x4000`, then proved Queue creation still returned `ENODEV`, unbound the mapping, closed the GEM, and destroyed the VM. The full cycle passed 16 more times and again after more than two and eight minutes. Firmware-visible bind/slot markers, q22 runtime publication, and strict GPU/firmware/kernel fault evidence remained empty.

Linux checkpoint: `87bcd27a516d0fc8d6b81e32c034c1fc2b2795d5`, tree `817a3c7734d7efb288343363c86f62835fcf70e1`.
