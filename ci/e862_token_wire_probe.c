#include <stdio.h>
#include <stdlib.h>
int main(void) {
 const char *cmd =
  "set -eu; X=/Applications/Xcode_16.3.app; "
  "echo '=== GPU templates detailed scan ==='; "
  "find \"$X/Contents/Applications/Instruments.app/Contents/Packages/GPU.instrdst\" -type f -name '*.tracetemplate' -print0 | while IFS= read -r -d '' f; do "
  " echo \"--- TEMPLATE $f ---\"; file \"$f\"; "
  " plutil -p \"$f\" 2>/dev/null | grep -Ei -C 5 'ShaderProfiler|APSConfig|configureAPS|CliqueTraceLevel|EmitPos|EmitThreadControlFlow|CountPeriod|PulsePeriod|SystemTimePeriod|SubSampling|KickAndStateTracing|ESLInstTracing|shaderprofilerinternal' || true; "
  " strings -a \"$f\" | grep -Ei -C 4 'ShaderProfiler|APSConfig|configureAPS|CliqueTraceLevel|EmitPos|EmitThreadControlFlow|CountPeriod|PulsePeriod|SystemTimePeriod|SubSampling|KickAndStateTracing|ESLInstTracing|shaderprofilerinternal' || true; "
  " done; "
  "echo '=== all Xcode tracetemplates matching profiling policy ==='; "
  "find \"$X/Contents\" -type f -name '*.tracetemplate' -print0 | while IFS= read -r -d '' f; do if strings -a \"$f\" 2>/dev/null | grep -Eiq 'ShaderProfiler|shaderprofilerinternal|APSConfig|CliqueTraceLevel'; then echo \"--- MATCH $f ---\"; strings -a \"$f\" | grep -Ei -C 5 'ShaderProfiler|shaderprofilerinternal|APSConfig|CliqueTraceLevel|EmitPos|EmitThreadControlFlow|CountPeriod|PulsePeriod|SystemTimePeriod|SubSampling|KickAndStateTracing|ESLInstTracing' | head -300; fi; done";
 int rc=system(cmd); printf("TEMPLATE_SCAN_RC=%d\n",rc); return rc==0?0:1;
}
