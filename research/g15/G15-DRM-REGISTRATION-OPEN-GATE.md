# J615 DRM registration behind a hard File-open gate

E158 advances exactly one live boundary beyond E157. After the current mapped-resource G15 manager reaches RTKit `MSG_INIT`, q21 `firmware_ready == 1`, and E152 q22 activation, Linux now registers DRM. A G15-only `File::open()` gate immediately returns `ENODEV` before File ID/PID state is created, so VM, Queue, QueueInfo, q22 runtime mappings, the E156 lazy CL-channel ensure, submission, and GPU commands all remain unreachable.

A one-shot `7.1.6-gc6fa9b794ba9` boot reached `T8122 G15 E158 persistent manager + DRM registration PASS; all G15 DRM file opens blocked`. `/dev/dri/renderD128` appeared as expected. A root open of that node returned `ENODEV (19)` and there were zero successful `DRM device opened` markers.

The candidate remained up for more than two minutes with no panic, Oops, SError, DART/IOMMU/GPU fault, RTKit/firmware crash, MTR alarm, or GPU command execution. The machine then returned to the golden kernel and the sacrificial module/initrd were restored byte-for-byte from exact pre-E158 copies.

E158 therefore live-proves DRM registration on top of the current E147-E152 persistent resource graph without allowing a G15 client object to exist. The next safe boundary is File discovery/open only, with VM creation independently gated. E156's channel ensure remains zero-caller and RunCompute remains fail-closed.
