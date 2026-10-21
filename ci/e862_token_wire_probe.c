#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXSYM 20000
struct sym { unsigned long long a; char line[1024]; int hit; };
static int interesting(const char *s){
 const char *pats[]={
  "-[DTGPUCounterProfile_GPURawCountersAPS _validateAndConfigureRawCounters]",
  "-[DTGPUCounterProfile_GPURawCountersAPS setAPSCounterConfig:]",
  "-[DTGPUCounterProfile_GPURawCountersAPS start:vendorFeatures:]",
  "-[DTGPUDataSource enableShaderProfiler:]",
  "-[DTGPUDataSource enableCliquePositionTracing:]",
  "-[DTGPUDataSource run]",
  "-[DTGPUService enableShaderProfiler]",
  "-[DTGPUService configureAPS:]",
  "-[DTGPUService setAPSCounterConfig:]",
  NULL};
 for(int i=0;pats[i];i++) if(strstr(s,pats[i])) return 1;
 return 0;
}
int main(void){
 const char *F="/Applications/Xcode_16.3.app/Contents/SharedFrameworks/DVTInstrumentsFoundation.framework/Versions/A/DVTInstrumentsFoundation";
 char cmd[4096]; snprintf(cmd,sizeof(cmd),"nm -arch arm64 -m '%s'",F); FILE *fp=popen(cmd,"r"); if(!fp){perror("nm");return 2;}
 struct sym *S=calloc(MAXSYM,sizeof(*S)); int n=0; char line[4096];
 while(fgets(line,sizeof(line),fp)&&n<MAXSYM){ unsigned long long a=0; if(sscanf(line,"%llx",&a)==1 && strstr(line,"(__TEXT,__text)")){ S[n].a=a; strncpy(S[n].line,line,sizeof(S[n].line)-1); S[n].hit=interesting(line); if(S[n].hit) printf("TARGET %016llx %s",a,line); n++; }} pclose(fp);
 for(int i=0;i<n;i++) if(S[i].hit){ unsigned long long end=S[i].a+0x3000; for(int j=0;j<n;j++) if(S[j].a>S[i].a && S[j].a<end) end=S[j].a; printf("RANGE %016llx %016llx %s",S[i].a,end,S[i].line); }
 printf("=== LITERAL CENSUS ===\n");
 snprintf(cmd,sizeof(cmd),"strings -a '%s' | egrep -i 'GRC_SHADER_PROFILER_DATA|GBL_USC_PROFILE_DATA|ShaderProfiler|CliquePosition|APSConfig' | head -300",F); system(cmd);
 printf("=== TARGET DISASSEMBLY ===\n");
 snprintf(cmd,sizeof(cmd),"otool -arch arm64 -tvV '%s'",F); fp=popen(cmd,"r"); if(!fp){perror("otool");return 3;}
 while(fgets(line,sizeof(line),fp)){
   unsigned long long a=0; if(sscanf(line,"%llx",&a)!=1) continue;
   for(int i=0;i<n;i++) if(S[i].hit){ unsigned long long end=S[i].a+0x3000; for(int j=0;j<n;j++) if(S[j].a>S[i].a && S[j].a<end) end=S[j].a; if(a>=S[i].a && a<end){ printf("FUNC %s",S[i].line); printf("%s",line); break; }}
 }
 pclose(fp); free(S); return 0;
}
