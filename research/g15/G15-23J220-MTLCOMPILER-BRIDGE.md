# G15 23J220 MTLCompiler bridge

E280 recovered the exact 23J220 `MTLCompiler` service-side call graph far enough to define the replay boundary without guessing.

## Exact service ABI

The 23J220 compiler service exposes this logical flow:

1. create a codegen-service object;
2. register a compiler plugin path plus an opaque configuration blob and receive a plugin index;
3. submit `(plugin_index, request_type, request_bytes, request_size, completion_block)`;
4. the service dispatches the request on a guarded split stack and forwards it to the selected compiler plugin.

The plugin interface loads the trusted compiler framework/bundle and resolves the driver's compiler lifecycle plus BuildRequest entry points. The same opaque configuration blob supplied at plugin registration is passed directly to the driver's compiler-create function.

The preferred normal path is the driver's BuildRequest-with-options entry point, with a plain BuildRequest fallback. A separate serialized-bitcode entry point is used only when that mode is requested and supported.

## Live cross-check on newer macOS

A forced monolithic current-macOS compile reaches the same service boundary twice for a minimal Compute pipeline:

- a small source/library request of type `0x0d`;
- a backend executable request of type `1`.

Static 23J220 request dispatch agrees with those request classes. This validates the outer request ABI, but current-macOS compiler output is not exact-target evidence.

## Alternate-cache caveat

Setting an alternate dyld shared-cache directory is insufficient proof that an old compiler stack is executing. Every claimed exact-target run must be verified by image UUID.

A private old-cache mapping also exposes a real loader ABI boundary: current dyld expects a newer libdyld helper interface than the 23J220 libdyld provides. Therefore simply forcing the old shared cache under the current dyld is not a valid exact execution route.

## E280 execution target

The remaining oracle path is now narrow:

1. execute the captured backend request through the exact 23J220 codegen service and exact 23J220 AGX compiler plugin;
2. capture the exact serialized compiler reply;
3. feed that reply into the already recovered exact G15 direct-ESL/state-loader path;
4. byte/semantic-diff the generated entry against E274/E279.

No Linux GPU launch is justified until this produces a concrete execution-facing delta.
