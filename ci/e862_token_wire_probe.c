#include <stdio.h>
#include <stdlib.h>
int main(void) {
    const char *cmd =
      "set -eu; "
      "P=/Applications/Xcode_16.3.app/Contents/Applications/Instruments.app/Contents/PlugIns/GPUPlugin.xrplugin/Contents/MacOS/GPUPlugin; "
      "test -f \"$P\"; "
      "lipo \"$P\" -thin arm64 -output out/GPUPlugin-arm64; "
      "shasum -a 256 \"$P\" out/GPUPlugin-arm64; "
      "file \"$P\" out/GPUPlugin-arm64; "
      "echo '=== policy strings ==='; strings -a out/GPUPlugin-arm64 | grep -E 'setAPSCounterConfig:|enableShaderProfiler|CountPeriod|PulsePeriod|SystemTimePeriod|SyncDrainMode|APSConfig|ShaderProfiler' | sort -u";
    int rc = system(cmd);
    printf("EXTRACT_RC=%d\n", rc);
    return rc == 0 ? 0 : 1;
}
