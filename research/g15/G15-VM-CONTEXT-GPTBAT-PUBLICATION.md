# J615 VM-context GPTBAT publication (E165)

E165 re-live-proves the active user-context boundary on top of the current E164 ownership/q22 stack while deliberately stopping before QueueInfo, lazy CL channel, WorkQueue publication, command parsing, and RunCompute.

The stale E033 empty-Queue publication body is removed from the preflight. `preflight_vm_bind_only()` now does only `GpuManager::bind_vm()`, records the selected user slot, drops the temporary bind guard, and returns. G15 `Uat::bind()` writes the exact bank-0/shared-bank-1 GPTBAT pair and performs the host ASID TLB invalidation; it emits no RTKit/FWCTL/pipe/doorbell/GPU command traffic.

Ordinary G15 SUBMIT remains `ENODEV`. Only the explicit E165 lab signature (`flags=0x47313556`, `pad=0x45313635`) with all other submit fields zero may resolve an existing passive Queue and execute the bind-only preflight, and it still returns `ENODEV` outward afterward.

The one-shot candidate completed 18 full VM + Queue + signed bind + Queue destroy + VM destroy lifecycles, reporting GPTBAT slots 1 through 18. Ordinary zero SUBMIT was rejected every time. No empty QueueInfo, ReleaseResource, uncertain-publication, GPU/firmware/kernel fault, or command marker appeared. A final probe passed after more than three minutes, then the machine returned to the golden kernel and sacrificial boot artifacts were restored exactly.

Linux checkpoint: `f962a8a435d2673ca0e26daf92697dcd163c4e71`, tree `a60f278451922180d581ce9a99b9f136fc9ce05d`.
