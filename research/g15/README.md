# Curated G15 Research Notes

This directory contains original, curated reverse-engineering summaries and clean-room generators used by the T8122/G15 bring-up.

Raw Apple binaries, kernelcaches, firmware extracts, raw decompiler output, and Ghidra project databases are deliberately excluded. Symbol names, offsets, derived constants, and independently reconstructed layouts are recorded where needed to make the source work reproducible.

Current high-level state: `../../docs/CURRENT-STATE.md`.

Key current boundary notes:

- `G15-COMPUTE-ERROR-RECOVERY.md` — E090 exact recovery ingress, destructive forced-stamp convergence, and separate context/FList/request/channel reset ownership.
- `G15-COMPUTE-EVENT-CONTROL.md` — E089 exact G15 RunCompute `+0x14` event-control semantics and fail-closed removal of the legacy queue-wide notifier.
- `G15-COMPUTE-COMPLETION-STAMPS.md` — E088 exact RunCompute event-control FWVA and normal stamp→scheduler→descriptor-retirement chain.
- `G15-FLIST-PREPARE-COMPLETE.md` — E087 exact FList HardwareBuffer prepare/complete residency lifetime and separate PTE teardown boundary.
- `G15-FLIST-PERSISTENT-OWNER.md` — E086 unreachable compile-only owner for all four exact persistent FList backings.
- `G15-Q22-RANGE8-SPECIAL.md` — E085 exact range-8 q22 special-aperture flags (`3/2`) with range-7 `1/0` preserved, compile-only.
- `G15-PUBLIC-M1N1-SEMANTIC-CROSSCHECK.md` — E084 current-public m1n1 concept cross-check; semantic clues only, no old G15 offsets imported.
- `G15-J615-UMA-POOL-GEOMETRY.md` — E083 exact J615 M/B defaults and resolved 4-MiB/32-KiB FList list geometry, compile-only.
- `G15-FLIST-RANGE5-LIST-PTE.md` — E082 exact FList Page/Backup List range-5 compact `0x300` class, corrected caller option, and compile-only semantic PTE constant.
- `G15-FLIST-RESOURCE-PLAN.md` — E081 side-effect-free FList resource geometry/HardwareBuffer ownership plan with no GPU allocator.
- `G15-HARDWARE-BUFFER-ID-OWNER.md` — E080 synchronized manager wrapper plus FList sticky owner, compile-only and uninstantiated.
- `G15-HARDWARE-BUFFER-ID-STATE.md` — E079 exact 0x100-entry sticky/refcount/bitmap/free-stack HardwareBuffer-ID state machine, compile-only and uninstantiated.
- `G15-23J220-FLIST-RESOURCE-LIFETIME.md` — E077/E078 exact FList four-resource constructor, symbolic list-size formulas, distinct `0x...044b` FW-Uncached range-7 leaf class, and compile-only Linux mapping ownership.
- `G15-RANGE8-DEDICATED-ALLOCATOR.md` — E076 compile-only split of range-7/range-8 bank-1 VA ownership and hard-wired range-8 protection class.
- `G15-23J220-RANGE8-LEAF-PTE.md` — E075 exact range-8 Page-Pool-State leaf class, one-shot PTE PASS, and clean eight-parent teardown.
- `G15-23J220-RANGE8-PAGE-POOL-STATE.md` — E074 exact-target range-8 Page-Pool-State aperture plus one-shot shared-parent ownership PASS and the remaining leaf/PTE boundary.
- `G15-23J220-CONTEXT-ID-GENERATION.md` — E073 exact-target 64-ID context/generation lifecycle, 256-ID HardwareBuffer lifetime, and Linux `+0x85f` implementation.
- `G15-23J220-COMPUTE-UMA-TAIL.md` — E072 exact-target stock-empty UMA value closure plus Page-Pool State and HWMetrics pointer producers.
- `G15-23J220-COMPUTE-SKU-SOURCES.md` — E070/E071 exact-target SKU packet/source map, inactive PerfCtr closure, UMA prepared-state handoff, timestamp geometry, and the Linux +0x760/+0x846 corrections.
- `G15-EMPTY-COMPUTE-CONTAINER.md` — E069 exact-23J220 proof that the stock empty Compute descriptor reaches normal `submitBuffer()`/firmware submission rather than being host-elided.
- `G15-EMPTY-COMPUTE-REGISTERARRAY.md` — E068 stock empty-Compute raw oracle, final descriptor-source closure, and exact 20-entry J615/G15G list.
- `G15-23J220-COMPUTE-REGISTERARRAY.md` — E067 exact-target G15 Compute RegisterArray order/source/synthesis validation and the remaining Linux-producer boundary.
- `G15-23J220-COMPUTE-ABI.md` — E066 exact 23J220 cross-check of the 0x880 Compute/CLE command, 0x18 accelerator-ring entry, Compute pipe ID 2, and ReleaseResource opcode 0x11.
- `G15-COMPUTE-SKU-STREAM.md` — E062 exact Compute SKU packet grammar, fixed WFI dword, timestamp record geometry, aligned stream sizes, and J615 dynamic register-ID closure.
- `G15-COMPUTE-CONTROL-STREAM.md` — E063 macOS-oracle proof of the `0x1a420` raw CDM stream edge and exact Gen4 patch/reset token/address-record grammar.
- `G15-COMPUTE-LAUNCH-BOUNDARY.md` — E061 proof that normal type-3 RunCompute is inherently hardware-facing; exact RTKit stream/UMA handoff and remaining first-command prerequisites.
- `G15-QUEUE-REGISTRATION-LIFECYCLE.md` — E056-E060 DPE correction, scheduler acceptance, stamp-state binding, pipe retirement, and native G15 ReleaseResource closure.
- `G15-PIPE-SUBMISSION-BOUNDARY.md` — historical transport/TX/doorbell proof chain that led to the now-closed registration boundary.

Earlier startup notes remain useful as the proof chain for the now-closed InitData/RTKit/`MSG_INIT` stages.
