# G15 E509–E560: CDM root fetch proven; surviving blocker is Launch-specific

Checkpoint: 2026-10-09

Target: MacBook Air M3 / J615 / T8122 / G15G C0. Exact implementation authority remains macOS 14.8.3 build 23J220 and its matching G15 firmware. Current macOS is used only as a dynamic structural oracle and every conclusion must be back-translated before becoming target ABI.

## Executive result

The Linux failure boundary is now substantially narrower than the E508 post-KickStart/pre-body checkpoint.

The durable path is:

1. queue, VM, WorkQueue and scheduler admission succeed;
2. exact RTKit enters Compute case `0x0b`, activates the hardware slot, publishes the expected register/dependency state and reaches WFI;
3. terminate-only Compute still proves the generic completion machinery;
4. the real Launch does not retire and the useful shader body still never reaches its first observable store;
5. a tagged replacement of hardware-visible RegisterArray `0x1a420` produces an address-exact `gCDM_CS` read fault, proving the CDM engine starts and performs the first fetch from the published control-stream root;
6. a tagged replacement of only the Launch packet's ESL-entry address is never fetched;
7. therefore the surviving real-Launch failure lies between **successful CDM root fetch** and **first ESL-entry fetch**.

In shorthand:

`Queue -> scheduler -> CDM engine -> root fetch [PASS] -> Launch decode/activation [FAIL WINDOW] -> ESL entry -> LoadShader -> USC body`

## Key closures after E508

### DMA-publication defects were real, but not yet sufficient

A sequence of correctness fixes closed concrete non-coherent streaming-DMA ownership bugs in the execution-facing program heaps, RunCompute/RegisterArray publication and SKU backing. Those fixes are required independently of the final root cause, but the corrected real Launch still did not retire.

The newest candidate, E560, closes the same ownership defect for the exact G15 Compute preemption/DataBuffer backing: after allocation it explicitly reacquires CPU ownership, reconstructs the exact zero/default state, then publishes the allocation back to the device before its five addresses are exposed through RegisterArray/JobParameters2. E560 is built and statically gated but has not yet been live-executed.

### STOP/LoadShader/body are downstream of the earliest blocker

A Launch whose ESL immediately STOPs still stalls, so useful body semantics and LoadShader execution cannot be the earliest missing prerequisite. Apple-side tagged-address oracles independently prove that a successful M3 path consumes the ESL program address and downstream LoadShader program address, while the failing Linux path does not reach the corresponding tagged ESL fetch.

### E552 — CDM root fetch proven directly

Changing only the hardware-visible `0x1a420` root pointer to an unmapped tagged address causes the GPU to fault on a read at exactly that address, classified to the CDM control-stream requestor. This proves the Linux path reaches CDM engine start and attempts the first root-stream fetch.

### E555/E556 — packet-byte and Stream-Link grammar closure

Known public/successful G15 Launch packet fields were rechecked against the selected Linux packet and no surviving packet-byte discrepancy was found. The only historical control-word alternative had already been tested and rejected under the corrected Linux envelope.

Separately, a current-macOS dynamic oracle on the same M3 confirmed that the exact 23J220-form G15 Stream Link encoding is fetched, decoded and followed by working hardware: replacing the first live control-stream token with a tagged Stream Link caused a read fault at the encoded target. This validates the retained exact-target Stream-Link grammar without transplanting current-build private constants.

### E558 — broad root visibility is no longer credible

The combination of:

- direct `0x1a420` tagged-root fetch,
- an earlier terminate-only stream that fetches/decodes from the same production control-stream class,
- corrected CDM/program DMA publication,
- exact stream-end pointer closure,
- and the successful Apple Stream-Link oracle

means a generic "CDM cannot see/decode the root" theory is contradicted by evidence. The differential is Launch-specific.

## E557 and E560: next bounded Linux discriminators

Two candidates are ready:

- **E560 first**: preserve the exact production command and fix only the preemption/DataBuffer DMA ownership defect. Progress would identify that publication bug as the missing Launch prerequisite; an unchanged stall closes it as insufficient.
- **E557 second if E560 is negative**: keep the exact Launch and corrected modern envelope but reduce only the host-supplied RegisterArray prefix to the five entries independently sufficient in the successful M3 bring-up path. This is a diagnostic reduction, not a production ABI proposal.

Both retain the exact Golden kernel ABI and the protected first-load/freeze/watchdog/recovery discipline.

## Current engineering frontier

Do not reopen generic scheduler admission, queue transport, broad root translation, post-bind TLBI, synthetic UMA growth, old shader-body variants or previously rejected Launch control-word substitutions without contradictory evidence.

The next live question is now extremely narrow: does correcting the final per-command preemption/DataBuffer publication defect allow the already-started CDM engine to cross the Launch transition and fetch the ESL entry? If not, use E557 to test whether an interaction with the additional production RegisterArray prefix is the last concrete differential.

Current macOS remains available for one-shot dynamic oracles whenever a precise successful-path question can distinguish two surviving hypotheses; every observation must still be mapped back to retained 23J220 authority before modifying Linux.
