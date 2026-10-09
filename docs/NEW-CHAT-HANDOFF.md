# New-chat handoff — J615 / G15 Compute bring-up

Generated: 2026-10-09

This is the public/sanitized continuation point. The private lab on the M3 contains the full evidence corpus and detailed machine-local experiment records.

## Target and authority

- Physical target: MacBook Air M3 / T8122 / J615 / G15G C0.
- Persistent/default OS: Golden Ubuntu kernel `7.1.6-ubuntu-m3-usbpd-gc5037a961e4d`.
- Exact ABI authority: macOS 14.8.3 / build 23J220 plus matching G15G C0 firmware.
- Current macOS 26.7.1 / 25G241: dynamic structural oracle only; do not transplant private constants.

## Machine control

Use HostFabric/Fabric or PiMaster on the M3. Linux target is `macmac`; macOS is the oracle side of the same physical machine. One-shot macOS boots are explicitly part of the workflow whenever they answer a precise discriminator; persistent boot must remain Ubuntu. Do not involve unrelated Surface machines in this project.

## Current safe state

Golden Linux is the persistent baseline. Experimental candidates are sacrificial and must not modify the frozen Golden kernel/source baseline. Historical worktrees and evidence are preserved rather than cleaned. Before each live candidate, require the established clean-boot / no-prior-Asahi / empty-one-shot / freeze / watchdog gate.

## Current execution frontier

The failure is no longer merely "post-KickStart / pre-body". It is localized to a Launch-specific transition:

`Queue -> scheduler -> CDM engine -> root fetch [PASS] -> Launch decode/activation [FAIL WINDOW] -> ESL entry -> LoadShader -> USC body`

Established facts:

- queue/VM control plane and WorkQueue scheduler admission succeed;
- exact firmware enters Compute case `0x0b`, activates the slot, appends the expected register state and reaches WFI;
- terminate-only Compute completes, so generic completion machinery is viable;
- the real shader body never reaches its first observable store;
- E552 proves the hardware consumes RegisterArray `0x1a420` and attempts the first CDM-root read;
- the paired ESL-entry sentinel is never fetched on the failing Linux Launch;
- ESL STOP still stalls, so LoadShader/body semantics are not the earliest blocker;
- known public/successful G15 Launch packet bytes are closed;
- working Apple M3 hardware independently follows the exact-target-form Stream-Link grammar.

Therefore do not treat generic CDM-root visibility/translation as the primary hypothesis.

## Recent DMA correctness fixes

Concrete non-coherent streaming-DMA ownership defects were found and corrected in:

- production program/data heaps;
- RunCompute/RegisterArray publication;
- selected SKU stream backing.

E560 extends that correction to the existing exact 0x14a0 Compute preemption/DataBuffer allocation: take CPU ownership after allocation, rebuild zero/default state, then return it to device ownership before RegisterArray/JobParameters2 publish its five addresses. No GPU-facing value is intentionally changed.

## Ready candidates

### E560 — run first

Branch: `wip/g15-e560-preempt-dma`.

Purpose: test whether the remaining unsynchronized preemption/DataBuffer publication is the missing Launch prerequisite. The exact-Golden module is built and statically gated; no live result exists yet.

Interpretation:

- progress/retirement: the DMA ownership defect was causally blocking Launch activation;
- unchanged stall: retain the fix as correctness work, but it is insufficient; proceed to E557.

### E557 — run second if E560 is negative

Branch: `wip/g15-e557-minimal-register-prefix`.

Purpose: reduce only the host-supplied G15 RegisterArray prefix to the five entries independently sufficient on the successful M3 bring-up path while leaving the exact Launch, mappings, DMA fixes, firmware tail and scheduler envelope unchanged.

This is a diagnostic reduction, not a production ABI proposal.

## Do not reopen without contradictory evidence

- generic Queue/WorkQueue scheduling failure;
- broad CDM-root translation/fetch failure;
- missing post-bind TLBI;
- synthetic target UMA/FList growth for the selected diagnostic;
- historical entry/body byte variants as the earliest cause;
- LoadShader/body semantics before first ESL fetch;
- already-rejected Launch dword/control-word substitutions;
- current macOS private constants as target ABI.

## Preferred workflow

1. protected live E560 on a fresh Golden boot;
2. if negative, clean recovery then protected live E557;
3. after either result, update the boundary before creating another candidate;
4. use one-shot macOS freely for precise successful-path oracle questions that can discriminate the surviving Launch-only hypotheses;
5. back-translate every macOS observation to retained 23J220 authority before changing Linux;
6. keep the persistent Golden baseline untouched and preserve evidence from every live cycle.

Full sanitized summary: `research/g15/G15-E509-E560-CDM-LAUNCH-FRONTIER.md`.
