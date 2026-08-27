# Curated G15 Research Notes

This directory contains original, curated reverse-engineering summaries and clean-room generators used by the T8122/G15 bring-up.

Raw Apple binaries, kernelcaches, firmware extracts, raw decompiler output, and Ghidra project databases are deliberately excluded. Symbol names, offsets, derived constants, and independently reconstructed layouts are recorded where needed to make the source work reproducible.

Current high-level state: `../../docs/CURRENT-STATE.md`.

Key current boundary notes:

- `G15-QUEUE-REGISTRATION-LIFECYCLE.md` — E056-E060 DPE correction, scheduler acceptance, stamp-state binding, pipe retirement, and native G15 ReleaseResource closure.
- `G15-PIPE-SUBMISSION-BOUNDARY.md` — historical transport/TX/doorbell proof chain that led to the now-closed registration boundary.

Earlier startup notes remain useful as the proof chain for the now-closed InitData/RTKit/`MSG_INIT` stages.
