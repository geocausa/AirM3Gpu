#!/usr/bin/env python3
"""Clean-room Apple G15G/C0 DPE/PPT + HwDataA pre-tail encoder.

Derived from the 25F84 AGXAcceleratorG15G host producer.  No captured
firmware data blob is embedded: the repeated banks and exceptional qwords are
emitted structurally.
"""
from __future__ import annotations
import argparse, hashlib, struct
from pathlib import Path

DPE_SIZE = 0x5DC
PRETAIL_SIZE = 0x780
DPE_OFFSET = 0x00C       # absolute HwDataA 0x3aa8 from pre-tail base 0x3a9c
SOCHOT_OFFSET = 0x6EC    # absolute HwDataA 0x4188

Q_BANK = 0x03FFFFFF03FFFFFF
Q_3FFFFF = 0x003FFFFF003FFFFF
Q_0F07 = 0x0F070F070F070F07


def put_qword_index(buf: bytearray, idx: int, value: int) -> None:
    off = idx * 4
    assert 0 <= off <= len(buf) - 8
    struct.pack_into('<Q', buf, off, value & 0xFFFFFFFFFFFFFFFF)


def build_dpe() -> bytes:
    b = bytearray(DPE_SIZE)

    for idx in (0x33, 0x35, 0x37, 0x39):
        put_qword_index(b, idx, 0xFFFFFFFFFFFFFFFF)
    put_qword_index(b, 0x3B, 0x0000000000080000)
    for idx in (0x3D, 0x3F):
        put_qword_index(b, idx, Q_3FFFFF)

    # 0x41/0x43 are explicit zero stores in Apple and remain allocation-zero.
    put_qword_index(b, 0x45, 0x003F000000000000)
    # 0x47/0x49/0x4b/0x4d are explicit zeros.
    put_qword_index(b, 0x4F, 0x2000000000000000)

    for idx in (0x51, 0x53, 0x55, 0x57):
        put_qword_index(b, idx, 0xFFFFFFFFFFFFFFFF)
    for idx in (0x59, 0x5B, 0x5D, 0x5F):
        put_qword_index(b, idx, Q_0F07)

    # Two 64-qword banks. Each qword address advances by 8 bytes because the
    # decompiler's destination type is u32 *, hence odd index increments of 2.
    for idx in range(0x61, 0xE0, 2):
        put_qword_index(b, idx, Q_BANK)

    # Special band interleaved between the two banks.
    put_qword_index(b, 0xE1, 0xA000000000000017)
    for idx in (0xE3, 0xE5, 0xE7, 0xE9):
        put_qword_index(b, idx, 0xFFFFFFFFFFFFFFFF)
    for idx in (0xEB, 0xED, 0xEF, 0xF1):
        put_qword_index(b, idx, Q_0F07)

    for idx in range(0xF3, 0x172, 2):
        put_qword_index(b, idx, Q_BANK)

    # Apple read/modify/write masks this qword. The destination starts zero on
    # J615, so its exact C0 bootstrap value is simply 0x00c00000.
    put_qword_index(b, 0x175, 0x0000000000C00000)

    # The C0 dynamic patch loop is dead on J615: accelerator+0x9d98 is zeroed
    # in the constructor and no producer changes it before this call.
    struct.pack_into('<I', b, 0, 0)
    return bytes(b)


def build_pretail(sochot_mask: int) -> bytes:
    if sochot_mask not in (0, 0x4248):
        raise ValueError('J615 SoCHot mask must be 0 or 0x4248')
    b = bytearray(PRETAIL_SIZE)

    # Absolute HwDataA +0x3aa4. G15G configureDevice seeds 5.0f and derived
    # init copies it here after zeroing through +0x3aa7.
    struct.pack_into('<I', b, 0x008, 0x40A00000)
    b[DPE_OFFSET:DPE_OFFSET + DPE_SIZE] = build_dpe()

    # Absolute +0x4188 SoCHot copy. All copied bytes are zero except the
    # discovered sensor mask at +0x4198 and the fixed 125 at +0x41a0.
    struct.pack_into('<Q', b, SOCHOT_OFFSET + 0x10, sochot_mask)
    struct.pack_into('<Q', b, SOCHOT_OFFSET + 0x18, 125)
    return bytes(b)


def sha(x: bytes) -> str:
    return hashlib.sha256(x).hexdigest()


def main() -> None:
    ap = argparse.ArgumentParser()
    ap.add_argument('--sochot-mask', type=lambda x: int(x, 0), default=0x4248)
    ap.add_argument('--dpe-out', type=Path)
    ap.add_argument('--pretail-out', type=Path)
    args = ap.parse_args()
    dpe = build_dpe(); pre = build_pretail(args.sochot_mask)
    if args.dpe_out: args.dpe_out.write_bytes(dpe)
    if args.pretail_out: args.pretail_out.write_bytes(pre)
    print(f'DPE size=0x{len(dpe):x} sha256={sha(dpe)}')
    print(f'PRETAIL mask=0x{args.sochot_mask:x} size=0x{len(pre):x} sha256={sha(pre)}')
    nz=[(i,v) for i,v in enumerate(pre) if v]
    print(f'PRETAIL nonzero_bytes={len(nz)} first=0x{nz[0][0]:x} last=0x{nz[-1][0]:x}')

if __name__ == '__main__': main()
