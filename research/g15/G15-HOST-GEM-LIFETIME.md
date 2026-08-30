# J615 host GEM lifetime (E161)

E161 permits only host GEM creation and DRM CPU mmap-offset lifetime on top of the E160 private VM boundary. Normal and VM-private GEM creation allocate DRM shmem/handle state; the VM-private form borrows the VM reservation object but receives no GPU VA or PTE. `GEM_MMAP_OFFSET` is DRM-core CPU mapping metadata.

VM_BIND, special GPU-visible object binding, Queue create/destroy and submit remain behind the unconditional G15 mutation gate. Consequently neither `Uat::bind()`, per-VM GPU mapping, shared bank-1 mapping nor q22 publication is reachable from the newly allowed paths.

The one-shot candidate created a valid private VM, created both public and VM-private 16-KiB GEMs, obtained mmap offsets and CPU-mapped/wrote/read them successfully. Queue creation while those objects were alive returned exact `ENODEV`; both handles closed and VM destroy succeeded. The complete sequence passed 16 additional iterations and remained fault-free beyond two minutes with no bind-slot or post-bootstrap q22 runtime publication.

Linux checkpoint `34848b4519467fa079402e117f97fe653e32b833`, tree `6817992ccda823dfd3f41b5083288f23d50546c8`. No RunCompute/custom GPU command was issued.
