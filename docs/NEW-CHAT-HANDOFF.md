# New-chat handoff — J615 / G15 Compute bring-up

Generated: 2026-10-07T19:10+01:00

This is the public/sanitized continuation point. The private lab has the full evidence corpus and a more detailed local handoff at `/home/macmac/m3-gpu-lab/HANDOFF-20261007-E508.md`.

## Target and authority

- Physical target: MacBook Air M3 / T8122 / J615 / G15G C0.
- Persistent/default OS: Golden Ubuntu kernel `7.1.6-ubuntu-m3-usbpd-gc5037a961e4d`.
- Exact ABI authority: macOS 14.8.3 / build 23J220 plus matching G15G C0 firmware.
- Current macOS 26.7.1 / 25G241: dynamic structural oracle only; do not transplant private constants.

## Machine control

Use HostFabric/Fabric as the primary execution path. Linux target is `macmac`; macOS oracle is `Mac` on the same physical machine. One-shot macOS boots are allowed when they answer a precise discriminator, but persistent boot must remain Ubuntu. Do not use unrelated Surface machines for this project.

## Current safe state

Golden Linux is running, GPU is unbound, the experimental Asahi module is not loaded, `/dev/dri/renderD128` is absent and no one-shot GRUB entry is armed.

The frozen Golden source and rollback baseline must not be modified. Historical experimental worktrees and private evidence are preserved rather than cleaned.

## Current execution frontier

The project is past generic Compute transport and past the Apple-equivalent KickStart boundary.

For the stuck real Compute launch:

- normal queue/VM control plane succeeds;
- scheduler transport accepts/retires the command;
- firmware enters Compute case `0x0b` and activates the hardware slot;
- expected G15 register/dependency state is published;
- firmware reaches WFI;
- no classified MMU/page fault is recorded;
- channel remains `wptr=1/doneptr=0`;
- actual shader result remains at its initial value, proving the body never reaches its first store;
- completion/release never arrives.

E503 maps Apple's successful Compute KickStart event to the exact retained firmware event at the beginning of case `0x0b`. Linux reaches beyond it. Therefore the missing prerequisite is downstream of scheduler/KickStart admission but upstream of useful USC body execution or hardware completion.

## Recent closures

- E483/E484: explicit post-bind ASID/TLB invalidation tested live and rejected.
- E487: decisive shader-result probe proves the body never executes.
- E505: current Apple stack performs a one-time first-real-Compute ~47 MiB Wire allocation; structural clue only.
- E506: exact 23J220 UMAPool sizing has no nonzero first-Compute floor for the selected minimal diagnostic.
- E507: current Apple spill sizing runs on every real Compute and emits repeatable descriptors; current-build private ABI only.
- E508: exact 23J220 back-translation keeps raw Compute min/ideal requests zero and rejects transplanting current descriptor values or synthesizing ~40 MiB UMA/FList backing.

Full sanitized summary: `research/g15/G15-E428-E508-PREBODY-EXECUTION-FRONTIER.md`.

## Do not reopen without new contradictory evidence

- direct `c040` scheduler-state reads;
- direct banked G15 `d8c0` fault reads;
- generic queue/WorkQueue scheduling failure;
- missing post-bind TLBI;
- classified MMU/page-fault cause for the stuck command;
- synthetic 40 MiB target UMA/FList growth;
- copying current macOS private spill constants into the 23J220 Linux path.

## Next task

Focus on **exact 23J220 pre-body program/USC execution activation or another non-UMA first-use global/context resource**.

Preferred workflow:

1. use the macOS oracle first when a precise successful-path question can separate candidates;
2. back-translate every current-OS observation to retained 23J220 authority;
3. make only one proven exact-target Linux change;
4. live-test on a fresh Golden boot using the established first-load/freeze/watchdog/evidence protocol;
5. return to Golden after each guarded candidate unless reuse is explicitly proven safe.

No new Linux live candidate is justified merely by the E505/E507 allocation values.
