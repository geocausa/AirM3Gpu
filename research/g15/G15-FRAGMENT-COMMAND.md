# G15 / RTKit-2419 Fragment (3D) command reconstruction

Exact host/FW target: AGXG15G + RTKit-2419.140.12.

Mechanically established geometry:

- total command size / ring stride: `0xc60`
- G15 command prefix/header is independently source-classified:
  - `+0x04` is the per-AGXCommandQueue cross-engine command sequence; normal Render consumes 3D `n` then TA `n+1`;
  - `+0x20` is the current AGXCommandBuffer state-entry `+0x398` qword;
  - `+0x28` is the exact 0x80-byte G15 parameter-buffer Info FWVA published by TA prepare;
  - `+0x30` is the selected 0x80-byte PM record FWVA; `+0x38` is the trailing PM-state FWVA;
  - `+0x0c` is the ContextIDManager allocation;
  - `+0x10..+0x1f` are unwritten;
  - `+0x40..+0x7f` is RTM-derived state with unwritten dwords at `+0x5c` and `+0x7c`.
- G15 register array begins at `+0x80`
- G15 register-list metadata: `+0x780/+0x788`
- G15 source-oriented `G15JobParameters3` begins at `+0x790` (legacy semantic JobParameters3 is not reused)
- host/register generator has active fields at `+0x9d8/+0x9e0`
- legacy/shared command state remains aligned through the middle body
- `+0xa48` is zero in the normal G15 path; `+0xa4c` is unwritten
- busy/overflow state: `+0xb60/+0xb64`
- encoder metadata begins `+0xb6c`
- command-local pointer/state at `+0xba0`
- `JobMeta` begins at `+0xba8`; G15 encoder reads its +0xc/+0x14/+0x1c/+0x24 members
- post-meta state: `+0xbd4`, `+0xbd8`
- `+0xbf0` is explicitly zero on G15
- `+0xbf8/+0xc00` are SegmentResourceList / BlockFence-time0 FWVAs
- `+0xc08/+0xc10` are AGXMTLCounterSampler firmware tokens
- G15 late-tail divergence begins at `+0xc18`
- host writes an unaligned FW-visible resource pointer at `+0xc1e`
- host writes further unaligned qwords at `+0xc27/+0xc2f/+0xc37`
- G15 encoder embeds `command_fwva + 0xc40` in the SKU stream
- `+0xc40` and `+0xc50` are initialized zero by host; firmware treats tail state as active
- exhaustive provenance checks found no proven RunFragment-local consumer for `+0xc48` or `+0xc58..+0xc5f`; apparent `c48/c58/c5c` hits resolve to unrelated firmware globals or a separate 0x18-stride scheduler array

Static command-local offsets used by `AGX3DChannelG15::encode3DCommandSKUStream` decode to `+0xa58`, `+0xb70`, `+0xb74`, `+0xba0`, and `+0xbd8`.

`AGX3DChannelG15::generateFrgBarrierRegister` returns zero. `encodePixelEndRenderWFI` emits literal opcode `1`, so G15 again combines a register-list command body with the older WaitForIdle opcode.

Kernel scaffolding keeps the exact register-array geometry but intentionally emits no G15 3D register entries yet. The late tail is modeled only where host/FW offsets are mechanically proven. Runtime submission remains fail-closed.
