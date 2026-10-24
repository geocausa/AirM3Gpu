# Recent experiment index

Current authority: [`CANONICAL-CHECKPOINT.md`](CANONICAL-CHECKPOINT.md). This index covers the work **after** the former E717 handoff; the earlier ledger is preserved unchanged in [`HISTORICAL-EXPERIMENTS-THROUGH-E717.md`](HISTORICAL-EXPERIMENTS-THROUGH-E717.md).

| Range / experiment | Evidence class | Material result | Decision |
|---|---|---|---|
| E740–E764 | Linux diagnostic / bounded observer | RDE observer integrated into selected Compute lifetime; kick-boundary/late windows; sampler SKU and split source ownership | Observation only; no instruction-0 success. |
| E776–E795 | Linux diagnostic | CSInvocation correlation -> APS USC topology/source/selector/counter-control -> raw ring/control capture | APS becomes the preferred observation plane; command transport stays closed. |
| E808 | Linux diagnostic | APS transition status exposed | Observer state directly inspectable. |
| E817–E823 | Linux diagnostic | Source cacheability, lifecycle timeline, STOP timing and post-STOP gates | Narrows observer timing/ownership. |
| E825 | Linux diagnostic | Initial bounded power/APS KTrace window | **Do not use missing KTrace records as a negative.** |
| E826 | Corrective kernel change | Restores proven 2 ms config-refresh settle after ForceConfigSnapshot | Invalidates the tempting E825 negative. |
| E827–E828 | Linux diagnostic | Producer-side power-event tracing / class 1 added alongside validated classes | Latest kernel observer direction; E828 head `3070021adff4108337d19c53070c1f95359b86f5`. |
| E848 / E850 | Offline Xcode RE | G15 v4r5 parser/decoder plus independent Xcode 15.4 v4r4 token jump table | Cross-version parser/token continuity; no GPU access. |
| E862–E872 | Offline parser/token replay | Token-wire fields/classes and controlled parser state transitions recovered | Semantic vocabulary for captured pages. |
| E877–E900 | Current-Xcode policy + page replay | Production profiler policy/masks reconstructed; Linux APS pages replayed | Oracle evidence only until exact-target bridge. |
| E907 | Working-Apple page | Known-positive Apple APS page preserved and token wire decoded | Positive comparison point. |
| E950 | Linux captured pages | Full RegisterArray APS pages preserved/replayed | Concrete full-register case. |
| E955 | Linux captured pages | Statics RLD pages preserved/replayed | Concrete loader/state case. |
| E972 | Offline exact-helper replay | Same-program Apple E932 and Linux E934 pages replay through recovered Xcode 16.3 decoder | Harness/inputs preserved; durable stdout/conclusion still missing. |

**Current boundary:** pre-instruction-0 autonomous interpreter admission.

**Current task:** recover E972 output -> identify earliest semantic Apple/Linux difference -> back-translate to exact 23J220 -> only then consider one live discriminator.
