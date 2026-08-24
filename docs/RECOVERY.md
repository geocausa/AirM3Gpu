# Local Machine Recovery Contract

This file documents the tested safety contract used during bring-up. Binary rollback files themselves are intentionally not stored in AirM3Gpu.

## Golden state

Persistent/default kernel:

`7.1.6-ubuntu-m3-usbpd-gc5037a961e4d`

Golden kernel image SHA-256:

`24cc320029c67b8996bd0c12fdffe46e51d823dbda24c753e4dc634cc0d54315`

Original known-good M3 m1n1 `boot.bin` SHA-256:

`8c538fc2c161d47cb2b275b650afd0c6b5dfd74fe1bd0c9343cf1c40d8602c9a`

GRUB persistent selection:

`GRUB_DEFAULT=0`

Experimental kernels are entered with a one-shot `grub-reboot`; `next_entry` must be empty again after the boot.

## Recovery procedure

If an experimental stage needs to be abandoned:

1. Restore a locally retained `boot.bin` whose SHA-256 is the golden value above to `/boot/efi/m1n1/boot.bin` using an atomic replacement.
2. Confirm the read-back SHA-256.
3. Clear any stale one-shot GRUB `next_entry`.
4. Confirm `/boot/vmlinuz` / the default GRUB entry still points at the golden kernel.
5. Reboot normally; do not arm the experimental entry.
6. Verify after boot:
   - `uname -r` is the golden release;
   - m1n1 `boot.bin` has the golden SHA-256;
   - `next_entry` is empty;
   - no experimental T8122 GPU platform node is active on the golden path;
   - no `/dev/dri/renderD*` node was created by the experimental work.

Do not overwrite the last known-good rollback copy when staging a new experimental boot artifact.
