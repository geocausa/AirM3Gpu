# G15 STOP-body execution discriminator

Research state: E321/E322, 2026-09-29.

This note records the execution conclusion only. Raw kernel journals and other
machine-local evidence remain in the private lab.

## Candidate

E321 starts from the fully corrected E319 mapping baseline and changes only the
selected shader body from the exact 58-byte 23J220 production body to the
independently validated four-byte G15 shader STOP:

`0e 00 00 00`

The host-side completion validator is adjusted to require the preinitialized
result sentinel to remain unchanged. No other GPU-visible state changes.

Implementation branch:

`wip/g15-e321-stop-body-on-e319`

Implementation commit:

`bc47537e50f9`

## Live result

E322 issued exactly one protected normal-UAPI Compute launch with that STOP-only
body. Compute/2 progressed to `(1,1,1,1)`, and the first RunWorkQueue was
accepted by the scheduler. The GPU timeout path still fired and scheduler
release later returned ETIMEDOUT.

Therefore production-body memory operations, stores, and body control flow are
not required to reproduce the real-shader non-completion. Even a valid
STOP-only selected shader fails to retire.

The remaining boundary is earlier than body semantics: LoadShader/program
activation, executable-program fetch/mapping, or other engine-side execution
state.

The one-shot recovery returned to Golden automatically, and the sacrificial
candidate slot was restored to its exact pre-live hashes.
