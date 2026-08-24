# G15 Compute command ABI (J615 / RTKit-2419.140.12)

## Proven size and generation

Apple host `AGXCLChannelSKU::submitBuffer()` writes the G15 firmware command through
`+0x878`; firmware consumes command type 3 with an exact active span of `0x880`.
The current pre-G15 Linux generated body was only `0x31c`.

G15 is command-side closer to G14X, but this must not be generalized to InitData:

- Linux G14X/V13.5 RunCompute: `0x87c`
- Apple G15 RunCompute: `0x880`
- G15 `AGXCLChannelG15::generateRegisterList()` operates on a register array at
  command `+0x20`, with its address/count/length metadata at `+0x720/+0x728`.

## Exact structural anchors

- `+0x000`: command type = 3
- `+0x004`: host sequence/counter qword (unaligned)
- `+0x010`: VM slot
- `+0x020`: G15 register array begins
- `+0x720`: register-array self address
- `+0x728`: register count/length metadata
- `+0x730..+0x75f`: active G15 pre-microsequence region; host writes fields at
  `+0x740`, `+0x748`, and `+0x750`; semantics not yet closed
- `+0x760`: microsequence / SKU stream pointer
- `+0x768`: microsequence / SKU stream size
- `+0x76c`: Compute JobParameters2 starts
- `+0x7c4/+0x7c8`: final two JobParameters2 dwords; G15 register-list encoder
  stores a qword at `+0x7c4`
- `+0x7cc..+0x7e3`: G15 encoder metadata, exactly `0x18` bytes
- `+0x7e4`: JobMeta, same offset as G14X
- `+0x7fc`: JobMeta stamp slot; firmware command-type-3 consumer reads this
- `+0x810`: command time
- `+0x818`: kernel timestamp pointers
- `+0x828`: user timestamp pointers
- `+0x838..+0x85f`: active G15-specific tail. Host has unaligned writes at
  `+0x83e`, `+0x847`, `+0x84f`, `+0x857`, plus bytes at `+0x846/+0x85f`.
  Firmware directly consumes the pointer at `+0x83e`. This region remains opaque
  in Linux while G15 runtime submission is fail-closed.
- `+0x860`: CDM context-store request timestamp (u64)
- `+0x870`: CDM context-store completion timestamp (u64)
- `+0x878`: u32 command flag/state. Generic host submit zeros it and
  `AGXCLChannelG15::encodeCLCommandSKUStream()` explicitly forms
  `command_fwva + 0x878` for its encoded stream.
- `+0x87c..+0x87f`: trailing zero/pad in the compile-only Linux shell
- exact total: `0x880`

Firmware independently proves `+0x860/+0x870`: it subtracts completion minus
request and can panic with the literal diagnostic
`CDM context store took too long ... req time ... complete time`.

## WFI hybrid detail

`AGXCLChannelG15::encodeComputeWFI()` appends exactly one dword whose value is 1.
Linux firmware microsequence opcodes define `WaitForIdle = 1` and
`WaitForIdle2 = 2`. Therefore G15 retains the old WFI opcode even though its
command body uses the newer register-list generation. Do not globally make G15
inherit G14X command/microsequence rules.

## Linux status

The compile-only G15 raw shell is sized and offset-asserted at `0x880`, with
register array / microsequence / JobParameters2 / JobMeta / timestamp and
context-store anchors placed exactly. Active regions whose host semantics are
not yet closed remain opaque and zero-filled. Runtime G15 submission remains
blocked; this is not sufficient to enable `start_op()`.
