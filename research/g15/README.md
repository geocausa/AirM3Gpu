# Curated G15 Research Notes

This directory contains original, curated reverse-engineering summaries and clean-room generators used by the T8122/G15 bring-up.

Raw Apple binaries, kernelcaches, firmware extracts, raw decompiler output, and Ghidra project databases are deliberately excluded. Symbol names, offsets, derived constants, and independently reconstructed layouts are recorded where needed to make the source work reproducible.

Current high-level state: `../../docs/CURRENT-STATE.md`.

Key current boundary note:

- `G15-PIPE-SUBMISSION-BOUNDARY.md` — live empty-QueueInfo transport boundary, exact G15 TX/doorbell facts, E035 wake-note negative result, and E036 submission-time power closure.

Earlier startup notes remain useful as the proof chain for the now-closed InitData/RTKit/`MSG_INIT` stages.
