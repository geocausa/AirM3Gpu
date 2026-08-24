# G15 RTKit application endpoint start

Checkpoint: 2026-08-24, J615 / T8122 G15G C0.

## Result

RTKit application endpoints EP20 (firmware) and EP21 (doorbell) can be started as an independent bring-up stage before InitData handoff.

Linux `apple_rtkit_start_ep()` sends only the RTKit management `STARTEP` operation. It does not send an application payload or an InitData address.

m1n1 independently uses the same protocol ordering for AGX: start ASC, start EP20/EP21, then initialize channels/build InitData, and only afterward send InitData and device-control initialization.

## Tested preflight

The G15 endpoint-start preflight:

1. completes all previously validated host-side gates;
2. completes RTKit protocol-v12 management boot;
3. verifies EP20 and EP21 were advertised;
4. starts EP20;
5. starts EP21;
6. verifies q21 remains at bootstrap state and no firmware crash occurred;
7. destroys RTKit and stops ASC;
8. returns before DRM registration.

It deliberately sends no:

- InitData address
- `MSG_INIT`
- device-control `Initialize`
- EP21 device-control doorbell
- GPU work

Kernel commit:

`1b57b289af96973badfbb8489ef379a1b3a96f07`

## Live PASS

Artifact set:

- kernel release: `7.1.6-m3-g15-eot-gb3d38d16c076-dirty`
- kernel Image SHA-256: `9f40eba872327e595577b7b74a12b3e2593620e3999967f6bff2ff28be1e2ed2`
- `asahi.ko` SHA-256: `2441b74274ecc0a401bc503a142d16f838fecad942d8a0863202e05dcef62b79`
- m1n1 `boot.bin` SHA-256: `a81993b122b7f990bc99ccab42decab7ef92adb94741dadf0ee79d24fdda3e0d`

Observed postconditions:

- EP20 + EP21 endpoint-start PASS
- zero GPU `failed buffer request`
- zero GPU `Unknown message`
- q21 untouched
- no GPU crash/fault
- no render node
- persistent GRUB default remains golden

## Safety boundary

This closes endpoint activation only. The next firmware-visible boundary is the InitData address / `MSG_INIT` handoff, which remains blocked pending a separate startup-read audit.
