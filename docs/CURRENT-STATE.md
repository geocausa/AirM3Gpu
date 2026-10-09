# Current G15 Bring-up State

Research state: **2026-10-09**

Target: MacBook Air M3 J615 / T8122, GPU G15G C0, exact macOS reference build 23J220 (14.8.3 ABI).


## 2026-10-09 update — E560

The real-Compute failure boundary is now **after CDM engine start/root fetch but before the first ESL-entry fetch**. E552 changed only hardware-visible RegisterArray `0x1a420` to an unmapped tagged address and produced an address-exact CDM control-stream read fault, proving the engine starts and fetches the published root. The paired ESL-entry sentinel did not get fetched, so the surviving differential is Launch-specific rather than generic root translation/visibility.

E542 independently showed an ESL STOP/no-LoadShader Launch still stalls, placing useful shader execution downstream of the earliest blocker. E555 closed known public/successful G15 Launch packet-byte discrepancies, and E556 dynamically validated the exact-target-form G15 Stream-Link grammar on working Apple hardware. E558 therefore deprioritizes broad CDM-root visibility/token-decode hypotheses.

Concrete non-coherent DMA-publication bugs have also been repaired in the program heaps, RunCompute/RegisterArray image and SKU backing. E560 is the newest built candidate and repairs the same ownership defect for the exact 0x14a0 Compute preemption/DataBuffer backing without changing addresses, PTEs, RegisterArray values, Launch bytes, programs or scheduler state. It is statically gated and **not yet live-executed**. E557, a five-entry RegisterArray-prefix diagnostic, is also built and remains the next discriminator if E560 is negative.

See `research/g15/G15-E509-E560-CDM-LAUNCH-FRONTIER.md`.

**Current next gate:** protected live E560 first; if unchanged, protected live E557. Use one-shot macOS whenever a precise successful-path oracle can collapse the remaining Launch-specific state space, then back-translate to 23J220 before changing Linux.

## 2026-10-07 update — E508

The execution frontier has moved materially beyond the September E274 timeout. The accumulated G15 module now reaches a real Compute launch through scheduler admission, firmware case `0x0b`, hardware-slot activation, G15 register/dependency publication, start-timestamp handling and the WFI wait. Firmware-owned fault state remains clear, channel state remains `wptr=1/doneptr=0`, and the actual shader result remains untouched (`0xffffffff`): useful USC body execution never starts.

A successful current-macOS Metal trace was back-translated to exact retained firmware and proves Linux is already past Apple's Compute KickStart-equivalent event. E483/E484 separately rejected missing post-map translation invalidation.

E505/E507 exposed a real current-Apple first-Compute spill/first-use allocation pattern, but E506/E508 back-translated it against exact 23J220 and rejected any synthetic target UMA/FList growth for the selected minimal diagnostic. Exact 23J220 raw Compute `+0x138/+0x140` remain the authoritative min/ideal request pair and are zero for this command; current 25G241 spill descriptor values must not be transplanted.

See `research/g15/G15-E428-E508-PREBODY-EXECUTION-FRONTIER.md`.

**Current next gate:** exact 23J220 pre-body program/USC execution activation or another non-UMA first-use global/context resource. Use current macOS only to ask a precise discriminating question, then back-translate to 23J220 before another Linux live candidate.

## 2026-10-02 update — exact compiler oracle survives 25G241

The machine-local E280 evidence was re-audited after the macOS host update to 26.7.1 / 25G241. The local lab had progressed farther than this summary recorded: a target-14-converted 4112-byte backend request already produced a successful exact-23J220 2208-byte compile reply. The current-dyld compatibility bridge was rebased for 25G241 and the same exact 23J220 backend path again completed with status 0.

Two fresh runs and the preserved September run differ only in the same two three-byte tail fields (six bytes total), while size, visible strings and object structure remain stable. Back-to-back runs vary at those same fields, so they are treated as per-run metadata rather than an execution-semantic regression.

This restores current macOS as a practical host for the preserved exact-23J220 compiler oracle. Current-system compiler output is still not exact-target evidence by itself; target claims continue to require the retained 23J220 components and provenance checks.

The compiler-reply acquisition half of E280 is therefore closed. The remaining production-entry boundary is the exact state-loader byte grammar described in `G15-23J220-PRODUCTION-STATE-LOADER.md`.

## 2026-09-29 update — E349

A protected terminate-only bisection isolated the first later regression to E298's direct-CDM mapping change. E297 completes; E298 times out. Allocation separation, low-vs-high range-5 VA placement and the uncached memory attribute have all been cleared. E348 further isolates the live failure to the AP field: the same low-VA uncached read-only CDM mapping completes with AP=2 and times out with AP=0.

E349 re-audited the exact 23J220 low-level UAT path and reconfirmed that Apple really does encode the matching bank-0 leaf with AP=0 (`0x0080000000000008` protection bits). The earlier reduction had an imprecise vtable-slot description, but the raw leaf arithmetic was correct. The contradiction therefore moves to G15's surrounding UAT permission mode/context: current Linux execution behaves like the older AP permission matrix even though Apple G15 uses a non-legacy interpretation. See `research/g15/G15-E329-E349-CDM-PTE-BISECTION.md`.

**Current next gate:** statically close the G15 UAT-mode/context activation contract—firmware mode consumer, GPTBAT/context setup, and any separate protected/runtime programming—before changing production PTE policy or issuing another GPU command.

## 2026-09-30 update — E350

The AP contradiction is closed. Production G15 SecureGart selects IOUnifiedAddressTranslator and the protected UAT-PPL mapper; the direct AGX encoder used by the earlier static reduction is the fallback backend. The exact PPL encoder maps pool-0x16 compact option 0x108 to protection `0x0080000000000088` (AP=2, uncached, GPU-access, PXN=UXN=0), exactly matching the E348 live-completing control. PPL also proves 0x108 and 0x308 are not PTE-equivalent, so the remaining range-5 resources must be reclassified individually rather than by one global 'uncached' constant. See `research/g15/G15-E350-UAT-PPL-PRODUCTION-PTE.md`.

**Current next gate:** re-reduce each execution-facing resource through the production PPL table, starting with pool 0x16 and pool 5, before another shader launch.

## 2026-09-30 update — E351

The production PPL matrix is now closed. Ordinary IOGPU backing descriptors have direction 3, resolving the last low-control ambiguity: pool 0x16 CDM and CodeHeap body/helper use compact 0x108 (`0x0080000000000088`), while pool 5 ESL, pool 0x0a Statics, pool 3 UserBuffer arguments, and the Shared application output use compact 0x308 (`0x00c0000000000088`). See `research/g15/G15-E351-PPL-RESOURCE-MATRIX.md`.

**Current next gate:** build a two-class range-5 candidate and audit every allocation before any further live shader launch.

## Current headline

The project has crossed the generic Compute execution boundary. A terminate-only J615 Compute command has completed normally on real hardware, proving the fundamental queue, RunCompute, firmware, event/stamp and WorkQueue completion path. The remaining blocker is specific to **real launch / state-loader / shader execution**, not generic G15 Compute transport.

The current safe boot is the persistent Golden Linux kernel. The sacrificial candidate slot has been restored and GRUB has no one-shot candidate armed.

## Strongest live results

### E199 — known-good Compute completion

A single exact-target terminate-only Compute command completed end-to-end:

- Compute/2 publication and firmware retirement reached `(1,1,1,1)`;
- scheduler acceptance succeeded;
- the WorkQueue callback fired with no error;
- no GPU/DART/RTKit fault occurred during execution.

This proves the generic J615 RunCompute/completion machinery is viable.

A separate post-idle q22 teardown issue exists and is treated independently from the execution result.

### E274 — historical real-launch diagnostic baseline

The corrected bounded real-launch path reaches:

- normal-UAPI acceptance;
- Compute/2 retirement observation `(1,1,1,1)`;
- RunWorkQueue scheduler acceptance;
- then a repeatable approximately six-second engine-completion timeout.

No explicit persisted GPU/DART/RTKit fault accompanied that timeout. E274 was the preferred baseline for that phase, but E487/E503 now provide a later and much narrower post-KickStart/pre-body boundary.

### E275 / E276 / E278 — rejected regressions

These experiments changed the failure class from the useful E274 timeout to an essentially immediate-reset class:

- E275: split CDM allocation from shader storage;
- E276: manual/bring-up launch dword 3 `0x40` instead of production `0x40000000`;
- E278: split CDM with ordinary GPU-shared-RW storage.

They should not be used as the forward live baseline.

## Closed static boundaries after E260

- Exact late engine/resource state for the ordinary non-ray path is zero/default, including dynamic `0x107a0 = 0x00ff0000`.
- Exact RTKit firmware-appended Compute RegisterArray tail is recovered; Linux must not manually duplicate it.
- CPU→GPU visibility/cache-maintenance was rejected as the missing range-5 code issue.
- Exact CDM terminate pointer is the address of the final terminate dword (`root + 0x2c` for the 0x30-byte stream).
- Exact Apple heap executable suballocation granularity does not require 0x1000/0x4000 entry/body separation.
- Direct-launch compiler spill/IPR metadata contributions are correctly zero for the hand-written diagnostic.
- Raw selector state feeding `0x1a440` is correct for an ordinary Compute encoder.
- `0x1a510` and the four preemption/state tail addresses belong to the command/DataBuffer allocation family; moving them to range-5 executable storage is not justified.
- Exact production direct-launch dword 3 remains `0x40000000`; the manual `0x40` value is not a replacement for the production contract.

## Historical static frontier — E280–E283

E279 is statically closed, and the exact-compiler-result portion of E280 is now closed as well.

Exact 23J220 `ProgramVariantESLState::setupDirectESL()` constructs a generated state-load program from multiple possible load forms (immediate, absolute, gather/user/indirect/SCS), finishes pending rounds, explicitly calls `appendLdshdr()`, appends USC profile-control state-loader instructions, and then `ESLStateLoadEncoderGen2::finish()` emits LoadShader plus conditional additional state/branch instructions.

A provenance-clean exact 23J220 backend run produced the target G15 Compute executable. E281–E283 established that the matching workload has exactly two direct-loader records—BufferBindings as a two-word UserBuffer load and Statics as an independent two-word absolute load—and that reproducing the final register values and two-load topology still retains the E274 six-second engine-completion timeout. See `research/g15/G15-23J220-PRODUCTION-STATE-LOADER.md`.

This E280–E283 frontier was subsequently advanced by later loader/PTE/firmware work. The authoritative next task is the 2026-10-09 E560 gate above.

## Repository checkpoint

Kernel implementation history through E278 is preserved in `geocausa/linux` on dedicated branches. The canonical project/research checkpoint is `geocausa/AirM3Gpu`. See `docs/REPOSITORIES.md`.

## Safety / methodology

- Keep the persistent Golden kernel untouched.
- Push source candidates before risky live tests.
- One bounded GPU command per candidate boot unless a prior result explicitly proves reuse safe.
- Do not delete dirty historical worktrees merely to make the directory tree look cleaner.
- Do not publish proprietary/raw evidence to the public project repository.
