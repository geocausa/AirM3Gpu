# Current G15 Bring-up State

Research state: **2026-10-24**

Target: MacBook Air M3 J615 / T8122 / G15G C0. Exact reference: macOS 14.8.3 / 23J220.

> Canonical resume point: `docs/CANONICAL-CHECKPOINT.md`.

## Headline

**E1075 is the newest live advancement:** restoring the exact T8122 HwDataA `+0x418c..+0x41db` 20-word image, originally recovered in E056 from Apple's J615/C0 producer, changes the current J615 runtime from an unread first DeviceControl command and timeout into running firmware with `/dev/dri/renderD128` present.

E1075 changes one variable only and is preserved in `geocausa/linux` at `a4778ca2a3f4791245cf1f7f42f26ccf2b17f5f7` (`wip/g15-e1075-hwdata418c`). No user GPU workload was submitted in that boot.

This startup success is separate from the older useful-execution frontier. E972 now gives the exact first semantic Apple/Linux divergence: both open the same mode-1 ESL slot, then working Apple publishes token7 with the initial outer-ESL PC while Linux does not. E974 proves parser state `0x61` means ESL slot 1, not a hardware mode. E989–E1005 prove the missing token7 event is common to complete RLD/LoadDev and valid LoadShader state operations, so the old RLD-only theory is closed.

Current execution boundary:

`outer ESL slot-open (token5) -> [autonomous complete state-operation accept/commit/retire edge missing] -> no initial-PC token7 -> no slot-close token6 -> no next-opcode progress`

## Next bounded gate

**E1076:** boot the unchanged E1075 kernel with only `asahi.g15_debug` bit 47 enabled. The built-in self-test submits one empty Compute job and polls at most two seconds. It is staged/documented but was not run because the remote reboot action was blocked. Boot configuration was restored to Golden and no `next_entry` remains.

If the one-job self-test passes, move to the smallest userspace compute/readback. Only after that should render/readback and a triangle be attempted.

## Closed / do not restart broadly

Do not return to generic Queue/WorkQueue, RunCompute layout, UAT/PTE, PagePool/FList/UMA, PMGR power, parser-state IDs, profiler-control, or blind MMIO sweeps without contradictory evidence. E979/E1003/E1014/E1016/E1018–E1021 found no surviving software producer that can simply be toggled to cross token5→token7.
