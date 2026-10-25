# E1094 audited native Wayland presentation checkpoint

Date: **2026-10-25**

Target: MacBook Air M3 J615 / T8122 / G15G C0

This document records the corrected public interpretation of E1094 after independent Sol/xhigh and Astra reviews of the raw retained traces. Where an early E1094 summary conflicts with this file, this file wins.

## What E1094 proves

E1094 runs the already-qualified G15G/Honeykrisp client path while native `apple-drm`/DCP owns the internal display. The evidence crosses the compositor boundary:

- Honeykrisp identifies `Apple M3 (G15G C0)` and real Render work completes.
- Wayland DMA-BUF `wl_buffer` objects are created and committed.
- Mutter releases those buffers.
- `wp_presentation_feedback.presented(...)` events are observed.
- Native DCP swap state advances during the bounded client runs.
- A 10-frame FIFO run completes with client return code 0.
- A 60-frame mailbox run completes with client return code 0.
- No fatal GPU/DART/RTKit fault accompanies the qualification runs.

This establishes the path:

`G15G/Honeykrisp client -> DMA-BUF -> Wayland -> Mutter -> native Apple DRM/DCP -> panel presentation machinery`

It does **not** establish that Mutter itself is rendered by G15G. The compositor is still software-rendering in this qualification path.

## Corrected mode results

| Case | Result | Safe interpretation |
|---|---|---|
| FIFO, 10 requested frames | completes (`rc=0`) | repeated FIFO presentation is possible for a short bounded run |
| FIFO, 60 / 10 s | timeout | long-run pacing/liveness issue exposed |
| FIFO, 60 / 30 s | timeout; DCP remains healthy/progresses | not a basic GPU/native-display failure |
| Mailbox, 10 | completes | mailbox path is viable |
| Mailbox, 60 / 30 s | completes (`rc=0`) | strong repeated-presentation positive control |
| `fifo_latest_ready` env attempt | **invalid** | Mesa rejected the selector; do not use this as evidence |

Mailbox is not yet a one-variable control for FIFO. In the inspected Mesa Wayland WSI, FIFO normally uses three images while true mailbox requires four. Actual created counts were not retained in the original mode comparison and must be measured next time.

## Corrections from raw-evidence audit

### The old VSYNC warning did not disappear

Mutter still reports:

`Invalid sequence for VSYNC frame info`

in E1094, including the successful short FIFO case. The initial E1094 runner's journal collection used an unsupported timestamp form, so the empty grep was not a valid negative observation.

Therefore the warning is neither sufficient for total presentation failure nor proven to be the cause of the long stalls.

### The long wait is localized more sharply

The traced client ultimately blocks in Wayland-side polling while waiting for progress. Buffer release wakes it. At long-gap boundaries, all current swapchain images can become unavailable.

Independent reconstruction of the retained trace found that the next queued timed attachment is only about one nominal refresh beyond the previous presentation, yet the client then waits approximately:

- `0.69 s`
- `1.00 s`
- `>=2.73 s` (timeout-censored)

This argues against a simple explanation in which Mesa intentionally scheduled the next frame seconds into the future. The leading liveness question is why a modestly future/due timed/FIFO transaction or one of its dependencies is not reconsidered promptly.

### Image starvation is a symptom, not the root cause

Three images busy can result from several mechanically different conditions:

- a legitimate unsignaled producer dependency;
- delayed readiness notification after a fence is already signaled;
- active software composition/consumer ownership;
- a timed/FIFO predecessor that is not progressing;
- a pending KMS completion/frame-clock dependency;
- stale generation/reference bookkeeping;
- an actual release/flush/client-bookkeeping bug.

The next experiment must identify the oldest actual owner/dependency rather than simply counting busy images again.

### KMS sequence zero is real but not yet causal

The native display path exposes invalid/zero pageflip sequence metadata and Mutter warns about it. However, the inspected Mesa prediction path consumes presentation time/refresh rather than that sequence number, and successful FIFO10 still shows the warning.

Treat these as separate questions:

1. raw sequence correctness;
2. presentation timestamp correctness/clock domain;
3. KMS completion-event delivery/liveness.

Do not fabricate an incrementing sequence as a fix or discriminator.

## Leading working model

A model consistent with the qualified evidence is:

1. FIFO work initially progresses.
2. delayed presentation feedback causes ordinary predictor correction and future timed commits.
3. older work drains; the next target is only about one refresh ahead.
4. a timed/FIFO transaction or another dependency on its critical path is not advanced/reconsidered promptly.
5. the three-image FIFO ring fills and the client sleeps waiting for release.
6. later compositor/fence/KMS activity restarts progress; releases arrive in a burst and frames resume.
7. the cycle can repeat.

This is a **working hypothesis**, not a proven Mutter bug. A real reservation fence, software-composition interval or KMS completion can create the same client-side symptom.

## Preferred next causal split

Before another live boot, build and qualify a transparent Wayland proxy that:

- forwards bytes and `SCM_RIGHTS` FDs unchanged;
- preserves object creation/destruction and ID reuse;
- has a pure passthrough mode;
- can selectively omit only `wp_commit_timer_v1.set_timestamp` for the test client/surface.

Then, after exact Golden recovery and requalification, run in one compositor session:

1. **A:** FIFO60 through proxy passthrough;
2. **B:** same, dropping only commit timestamps;
3. **C:** passthrough plus Mesa `MESA_VK_WSI_DEBUG=nowlts`;
4. **A:** passthrough again.

Use 30-second bounds plus bounded termination grace. Interpretation is valid only if both A runs reproduce the long-gap signature and actual swapchain properties match.

`nowlts` is intentionally the C arm rather than the clean B arm because inspected Mesa source shows it changes more than timestamps: timer binding, extra/dummy FIFO commits, feedback requests and some event dispatch behavior.

Every arm must retain actual requested/effective mode, image count, format/colorspace/modifier, swapchain generation, API/image state, attachment/release/present/discard events, valid journal cursor, and DCP state.

## Evidence provenance

Private E1094 initial `RESULT.md` SHA-256: `a61a762a673600944bf0b49dd3a0cdef798c3efe3ec05d244e32d23e4f8001cf`.

Independent Sol/xhigh audit SHA-256: `7d16c74678af90ae885126b729b7081493f1663206a10246c3f9e463d4e720a0`.

Astra focused review SHA-256: `3cdf801d75fda1cfbf78371f47bec5a5d5e94975e08dcc214249cc5e89a61245`.

Astra/xhigh report SHA-256: `c5253946dc22ea228a4ec99e34ed73400f562fa245ca06ac0882246a8e67da02`.

Astra/xhigh decision tree SHA-256: `dbfd94e90b3514439214f135cc4bdd0c1aba20f6001669329e78c676e063404b`.

Astra/xhigh next-experiment plan SHA-256: `62550ac0ecd7a8d9e69ba3734fafeafc6b6dbb5c5ffd6e922ab8aa52981676f3`.

## Operational state

E1094 evidence is durable. At this checkpoint `macmac` is still running experimental kernel `7.1.12-e1091-t8122-pmp-reordered`. Exact Golden recovery remains the first operational action before any new live GPU experiment.
