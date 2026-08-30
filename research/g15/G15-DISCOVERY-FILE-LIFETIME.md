# J615 discovery-only DRM File lifetime

E159 advances one boundary beyond E158: G15 DRM `File::open()` is permitted, but every ioctl that creates, destroys, maps, publishes, or submits client state returns `ENODEV` before its first mutation. `GET_PARAMS` and `GET_TIME` are the only allowed G15 ioctls. Queue/Compute/WorkQueue source is unchanged and the E156 lazy CL-channel ensure remains definition-only.

A one-shot `7.1.6-gc6fa9b794ba9` boot reached `T8122 G15 E159 persistent manager + DRM discovery PASS; client mutations blocked`. The dedicated probe opened `/dev/dri/renderD128`, successfully read `GET_PARAMS` (`chip=0x8122`, generation 15) and `GET_TIME`, then issued a deliberately invalid zeroed `VM_CREATE`. It returned `ENODEV (19)` instead of argument-validation `EINVAL`, proving the discovery mutation gate ran before VM allocation. The same probe passed again after more than two minutes.

No post-bootstrap q22 mapping/pressure publication occurred and the strict fault audit found no GPU/RTKit/MTR/DART/kernel fault. The historical E033 signed-submit branch is unreachable on G15 because `SUBMIT` now hits the unconditional discovery mutation gate first. The machine returned to the golden kernel and exact sacrificial module/initrd bytes were restored.

E159 therefore live-proves persistent bootstrap + DRM registration + File discovery while keeping VM/GEM/Queue/submission state nonexistent. The next safe boundary is a private VM lifecycle with Queue creation still independently gated. No RunCompute or custom GPU command was issued.
