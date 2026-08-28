# Curated G15 Research Notes

This directory contains original, curated reverse-engineering summaries and clean-room generators used by the T8122/G15 bring-up.

Raw Apple binaries, kernelcaches, firmware extracts, raw decompiler output, and Ghidra project databases are deliberately excluded. Symbol names, offsets, derived constants, and independently reconstructed layouts are recorded where needed to make the source work reproducible.

Current high-level state: `../../docs/CURRENT-STATE.md`.

Key current boundary notes:

- `G15-23J220-COMPUTE-ABI.md` — E066 exact 23J220 cross-check of the 0x880 Compute/CLE command, 0x18 accelerator-ring entry, Compute pipe ID 2, and ReleaseResource opcode 0x11.
- `G15-COMPUTE-SKU-STREAM.md` — E062 exact Compute SKU packet grammar, fixed WFI dword, timestamp record geometry, aligned stream sizes, and J615 dynamic register-ID closure.
- `G15-COMPUTE-CONTROL-STREAM.md` — E063 macOS-oracle proof of the `0x1a420` raw CDM stream edge and exact Gen4 patch/reset token/address-record grammar.
- `G15-COMPUTE-LAUNCH-BOUNDARY.md` — E061 proof that normal type-3 RunCompute is inherently hardware-facing; exact RTKit stream/UMA handoff and remaining first-command prerequisites.
- `G15-QUEUE-REGISTRATION-LIFECYCLE.md` — E056-E060 DPE correction, scheduler acceptance, stamp-state binding, pipe retirement, and native G15 ReleaseResource closure.
- `G15-PIPE-SUBMISSION-BOUNDARY.md` — historical transport/TX/doorbell proof chain that led to the now-closed registration boundary.

Earlier startup notes remain useful as the proof chain for the now-closed InitData/RTKit/`MSG_INIT` stages.
