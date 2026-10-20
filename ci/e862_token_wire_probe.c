// E868 hosted symbol census for Xcode 16.3 APS parser/profile-data APIs. No GPU access.
#include <stdio.h>
#include <stdlib.h>
int main(void) {
 const char *cmd = "nm -gU /Applications/Xcode.app/Contents/PlugIns/GPUDebugger.ideplugin/Contents/Frameworks/GTShaderProfiler.framework/Versions/A/GTShaderProfiler | grep -E ' _agxps_aps_(profile_data|get|clique|parser|token|sample|work|esl)' | sort";
 int rc = system(cmd);
 printf("E868_NM_RC=%d\n", rc);
 return rc ? 1 : 0;
}
