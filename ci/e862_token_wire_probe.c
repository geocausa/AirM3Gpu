#include <stdio.h>
#include <stdlib.h>
int main(void) {
    const char *cmd =
      "set -eu; ROOT=/Applications/Xcode_16.3.app/Contents/Applications/Instruments.app/Contents/PlugIns/GPUPlugin.xrplugin; "
      "echo '=== GPUPlugin files ==='; find \"$ROOT\" -type f | sed -n '1,260p'; "
      "echo '=== trace templates ==='; find /Applications/Xcode_16.3.app/Contents -type f -name '*.tracetemplate' -print | sed -n '1,260p'; "
      "echo '=== ShaderProfiler-bearing GPUPlugin files ==='; "
      "find \"$ROOT\" -type f -print0 | while IFS= read -r -d '' f; do "
      "  if strings -a \"$f\" 2>/dev/null | grep -Eiq 'shaderprofiler|shader profiler'; then "
      "    echo \"--- FILE $f ---\"; file \"$f\"; "
      "    strings -a \"$f\" 2>/dev/null | grep -Ei 'shaderprofiler|APSConfig|configureAPS|setAPSCounterConfig|CliqueTraceLevel|EmitPos|EmitThreadControlFlow|CountPeriod|PulsePeriod|SystemTimePeriod|KickAndStateTracing|ESLInstTracing|TileTracing|SyncDrain' | head -160 || true; "
      "  fi; done; "
      "echo '=== DVT policy call strings ==='; "
      "D=/Applications/Xcode_16.3.app/Contents/SharedFrameworks/DVTInstrumentsFoundation.framework/Versions/A/DVTInstrumentsFoundation; "
      "if [ -f \"$D\" ]; then strings -a \"$D\" | grep -E 'configureAPS:|setAPSCounterConfig:|APSConfig|ShaderProfiler|CliqueTraceLevel|EmitPos|EmitThreadControlFlow' | sort -u; fi";
    int rc = system(cmd);
    printf("SCAN_RC=%d\n", rc);
    return rc == 0 ? 0 : 1;
}
