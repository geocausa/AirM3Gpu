# G15 stock-empty Compute SKU source map and serializer

E101/E102 close the exact macOS 14.8.3 / 23J220 inactive stock-empty Compute SKU stream while keeping it unreachable from Linux submission.

The exact stream is `0x2b8` bytes before reporting/alignment and `0x2c0` bytes in the command: a `0x1bc` type-0xb setup packet, 0x3c-byte start timestamp, WFI dword `1`, 0x3c-byte end timestamp, 0x7c-byte type-0xc retirement packet and finish dword `0x40000002`.

E101 re-audits every stock-empty source. Important corrections include: the descriptor `+0x460` boolean is setup-payload `+0x3c`, setup `+0x40` is `command_fwva+0x76c` (JobParameters2), and timestamp `+0x34` is the command UUID. The stock-empty optional PerfCtr/counter regions are zero. The type-0xc backpointer is setup payload `+0x158` (`stream_fwva+0x15c`), and encoder bookkeeping is exact `0xfffffdc8` (`-0x238`). J615 CL region addresses use exact stride `0xf400` plus one `0x800` MGPU span.

Linux commit `897d3ae4189603b58438131724e8d481f03b6917` adds a byte-exact serializer with only runtime-owned pointer/state inputs. It owns no GPU memory, has no call sites, and does not replace the existing RunCompute `+0x760` producer. The serializer therefore cannot enable G15 execution by itself.

Validation: tree `2bc3de6ea13b5115594305bb597f10df92a735bc`; module SHA-256 `b6373f5b22796f11994d2292663bff0464f945488c09edccb666bdce81829778`; exact 24-warning baseline; strict checkpatch 0/0/0; independent layout/source audit PASS with synthetic-reference SHA-256 `8c5624d0a35013f39033c178ce8ffc218a2391ad7ef4028a8a0c43e0f85135c3`.

No module was installed and no RunCompute or other GPU command was issued.
