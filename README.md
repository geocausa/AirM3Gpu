# AirM3Gpu

Experimental clean-room bring-up and reverse-engineering work for Apple M3 / T8122 J615 G15G C0 on Linux.

This repository is the canonical public research checkpoint. Compilable Linux implementation history lives in `geocausa/linux`; raw proprietary and machine-local evidence remains private.

## Current state — 2026-10-25

The physical MacBook Air M3 has crossed real Compute, graphics execution, native Apple DCP/KMS display takeover, simultaneous GPU+display operation, direct GPU-produced DMA-BUF presentation, and compositor-mediated native Wayland presentation.

- **E1075** restores the exact T8122 HwDataA `+0x418c..+0x41db` block and reaches `firmware-running` with `/dev/dri/renderD128`.
- **E1076** proves correct Vulkan Compute/readback: `788 runs in 1.0 s`, `0 wrong results`.
- **E1077** proves correct Vulkan graphics/readback: `1174 runs in 1.0 s`, `0 wrong pixels`, with non-zero TA and fragment timing.
- **E1078** carries exact GPU triangle pixels to the panel through a verified CPU-copy bridge and restores the framebuffer byte-for-byte; useful, but not native scanout.
- **E1086** localizes the old simpledrm/Wayland presentation wall. GPU execution remains correct while Mutter reports `Invalid sequence for VSYNC frame info`.
- **E1091** crosses native J615 display ownership: PMP/DCP/PIODMA come up, `apple-drm` registers, `eDP-1` is connected at 2880x1800@60, and `fb0=appledrmfb`.
- **E1092** proves native display + qualified G15G runtime coexist on one boot while Vulkan Render completes `988 runs / 0 wrong pixels`.
- **E1093** removes the CPU-copy bridge: a Honeykrisp-produced DMA-BUF is PRIME-imported into `apple-drm` and made the native DCP active framebuffer, then the original framebuffer is restored.
- **E1094** proves the next layer: Honeykrisp/G15G buffers enter native Wayland/Mutter and receive real `wl_buffer.release` and `wp_presentation_feedback.presented` events while Apple DRM/DCP advances. A 10-frame FIFO run completes; a 60-frame mailbox run completes; long FIFO runs expose a pacing/liveness defect rather than a basic GPU or native-presentation failure.

### E1094 audit corrections

Independent GPT-6.1 Sol/xhigh and GPT-6 Astra reviews re-read the raw E1094 traces and corrected the initial narrative:

- the Mutter `Invalid sequence for VSYNC frame info` warning **still occurs** in E1094, including a successful short FIFO run;
- the attempted `fifo_latest_ready` comparison was **invalid** because the environment selector was rejected;
- mailbox is not a one-variable FIFO control: the inspected Wayland WSI defaults FIFO to 3 images while true mailbox requires 4, unless the actual created counts prove otherwise;
- all-images-busy/release starvation is a downstream localization, not yet the root cause;
- the strongest current hypothesis is a timed/FIFO transaction-liveness interaction: after prior work drains, a target only about one refresh ahead can be followed by release stalls of roughly 0.69 s, 1.00 s and >=2.73 s;
- raw KMS sequence `0` remains a real metadata defect, but is not proven sufficient to cause the stalls;
- Mutter itself is still software-compositing on this path. E1094 proves GPU-client presentation through the compositor, not a fully GPU-accelerated GNOME desktop.

The current technical frontier is therefore **FIFO/timed Wayland transaction liveness, exact buffer/dependency ownership, and KMS/frame-clock synchronization**, followed by compositor acceleration, endurance, performance, power and suspend/resume.

The preferred next discriminator is an offline-qualified transparent Wayland proxy and one-session **A/B/C/A** comparison: FIFO passthrough -> drop only `wp_commit_timer_v1.set_timestamp` -> passthrough + Mesa `nowlts` -> passthrough again. Actual image count/format/modifier/generation, releases, feedback, API state, journal cursor and DCP state must be captured for every arm.

Historical E972/E974/E989-E1005 token5->token7 failures remain valid for the older failing lineage, but they are not the repaired-lineage wall.

**Operational note:** after E1094 the host is still on the experimental E1094 kernel at this checkpoint; exact Golden recovery remains mandatory before the next live GPU experiment. The prior E1093 Golden recovery remains a historical verified recovery event, not the current live state.

Start with `docs/CANONICAL-CHECKPOINT.md`, then `docs/E1094-AUDITED-PRESENTATION.md`, `docs/NEW-CHAT-HANDOFF.md` and `docs/RECENT-EXPERIMENTS.md`.
