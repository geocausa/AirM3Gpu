# J615 q22 firmware cursor consumption (E164)

E164 closes the consumer-side half of the active q22 mapping-ring contract without enabling firmware QueueInfo/context publication, lazy CL-channel construction, submission, or RunCompute.

The only host write to q22 `read_idx` in the current tree is the CPU-only preflight reset before persistent q22 activation. Runtime `publish_pages()` reads `read_idx` but advances only `write_idx`. E164 adds temporary diagnostic cursor reporting around those already-existing map/unmap batches.

Live passive Queue resource lifetimes filled the active ring from `read=0, write=0` to `read=0, write=190`; the firmware read cursor remained zero across multi-second waits. One additional Queue lifecycle crossed Apple's exact occupancy-0xc0 pressure threshold. The existing native q22 pressure path then ran at write 192/193, and before the first subsequent unmap the shared state had advanced to `read=194, write=194`. Thus J615 firmware consumed all 194 entries pending at the drain point. Four later unmaps left the bounded experiment at `194:198`.

Twenty-five passive Queue lifecycles completed in total. G15 SUBMIT stayed exact `ENODEV`, QueueInfo/context publication markers remained empty, strict fault evidence remained empty, and the candidate stayed healthy beyond three minutes before returning to the golden kernel with sacrificial boot artifacts restored exactly.

Linux checkpoint: `688651b7bfe16f552a8b2fc71e5b295323b08686`, tree `62bd6c7b5fb7bacf76a5d6e59b1e156a57eb3d80`.
