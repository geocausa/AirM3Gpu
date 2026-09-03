# New-chat handoff — J615 / G15 Compute bring-up

Generated: 2026-09-02T22:04:28+01:00

This file is the compact continuation point if a long ChatGPT thread loses context.

## Machine / access

- Physical target: MacBook Air M3 / T8122 / J615 / G15G C0.
- Ubuntu PiMaster endpoint: `client_macbookm3ubuntu`.
- macOS PiMaster endpoint: `client_macbook_air_m3`.
- macOS can therefore be driven directly after a one-shot boot; do not assume a reboot script/automation is required for control after macOS comes up.
- Current boot: `7.1.6-ubuntu-m3-usbpd-gc5037a961e4d` (Golden Ubuntu).
- GRUB env: `next_entry= `.

## Repository roles

- Canonical project/research repository: `geocausa/AirM3Gpu`.
- Linux implementation fork: `geocausa/linux`.
- Private/raw working corpus: `/home/macmac/m3-gpu-lab` — do not clean or publish wholesale.

Current AirM3Gpu HEAD: `c24c535564fbcba61e32e582a5d407618343b8d0`.
Current active Linux worktree: `/home/macmac/src/linux-m3-gpu-e265`.
Current kernel branch: `wip/g15-e278-cdm-shared-rw`.
Current kernel HEAD: `f8306c6f90b0fcc561298488f6185a2055496a0b`.

## Proven execution boundary

- **E199 LIVE PASS**: exact-target terminate-only J615 Compute completed through scheduler, engine retirement, WorkQueue callback and selected channel completion. Generic RunCompute transport/completion is therefore proven.
- **E274** (kernel `bcc062a1c864`) is the preferred real-launch baseline: command reaches scheduler acceptance and then reproducibly times out waiting for engine completion after ~6 s. It is diagnostically better than the later reset-class experiments.
- **E275 / E276 / E278** are rejected unchanged: separating CDM storage and the manual `0x40` launch-control discriminator regressed to immediate-reset class.
- Keep exact E263 terminate pointer at the final terminate dword (`root+0x2c`).
- Keep production direct-launch dword3 `0x40000000`; E276 rejected `0x40` for the corrected envelope.

## Static closures that must not be reopened casually

- RTKit appends the required G15 Compute RegisterArray tail itself (E261).
- Range-5 executable CPU→GPU visibility does not justify ad-hoc cache maintenance (E262).
- Individual executable heap alignment is 0x40; current placement is sufficient (E264).
- Compiler-only spill/IPR launch metadata is zero for the bounded hand-written shader (E266).
- Raw +0x1ba/+0x1bb and 0x1a440 ordinary direct-path selectors are already closed (E267/E268).
- Exact direct-launch packet grammar was reclosed in E270.
- Preemption/DataBuffer backing belongs to command/DataBuffer storage family; do not move 0x1a510 backing into range-5 code (E277).

## E279 conclusion

E279 is a static PASS. The fixed full entry sequence from Alyssa's successful M3 bring-up and pac85 is a valid hand-written G15 execution oracle, but it is **not** proven to be the byte-universal exact 23J220 production `ComputeProgramVariant` entry.

Exact 23J220 `setupDirectESL()` builds a generated ESL/state-loader program from compiler reply/resource state. It can emit immediate, absolute, gather/user/indirect-user/SCS loads, finish rounds, append LoadShader, profile-control state and conditional LDIMM/branch state. Production entry bytes must therefore be recovered from an exact compiler/driver oracle rather than assumed from the manual fixed epilog.

## Current frontier — E280

Directory: `/home/macmac/m3-gpu-lab/experiments/E280-g15-exact-compiler-entry-oracle`.

Goal: obtain an exact 23J220 minimal direct-Compute compiler/deserialized reply plus the production driver-generated ESL entry program, then mechanically diff it against E274.

Already present in E280:
- exact 23J220 compiler-service/lifecycle analysis;
- exact 23J220 `MTLCompiler` codegen-service/plugin bridge and BuildRequest forwarding ABI;
- exact profile/state-loader decompilation;
- exact LDIMM, B/BL, SETPROFILECTL and LoadShader emitter reconstruction;
- live newer-macOS capture of the source/library (`0x0d`) and backend executable (`1`) request classes for a minimal Compute pipeline;
- proof that alternate-cache selection alone is **not** exact-target evidence, plus a traced current-dyld/23J220-libdyld helper-ABI incompatibility for private old-cache mapping.

Next required outputs:
1. replay the captured backend request through the exact 23J220 service + exact AGX compiler plugin;
2. exact 23J220 minimal compiler reply / ShaderInfo;
3. exact generated ESL entry bytes + LoadShader mode;
4. exact body bytes/metadata relevant to launch;
5. semantic/byte diff vs E274.

**Do not issue another Linux GPU real-launch command until E280 produces a concrete execution-facing delta.**

## macOS note

Installed macOS is newer 25F84, while the exact target ABI is retained 23J220 / macOS 14.8.3 artifacts. Use current macOS for dynamic cross-checks only unless the observation is version-independent. Because PiMaster is available in macOS, future one-shot boots can be driven interactively from ChatGPT on both sides.

## Safety / recovery

- Golden is the persistent/default boot.
- Candidate kernel slot is sacrificial.
- Never leave a one-shot candidate armed after recovery.
- Preserve dirty historical worktrees and the raw lab corpus; they contain unique evidence.
- Push a kernel branch before any risky live candidate; checkpoint sanitized conclusions to AirM3Gpu when they become stable.
