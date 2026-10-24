#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdint.h>
#include <stdio.h>
int main(void){
 const char *P="/Applications/Xcode.app/Contents/PlugIns/GPUDebugger.ideplugin/Contents/Frameworks/GTShaderProfiler.framework/Versions/A/GTShaderProfiler";
 void *h=dlopen(P,RTLD_NOW|RTLD_LOCAL); if(!h){fprintf(stderr,"dlopen:%s\n",dlerror());return 2;}
 void *s=dlsym(h,"agxps_aps_parser_parse"); if(!s)return 3; Dl_info d={0}; if(!dladdr(s,&d))return 4;
 uintptr_t b=(uintptr_t)d.dli_fbase; uint16_t *t=(uint16_t*)(b+0xbe0a48); uintptr_t anchor=b+0x829edc;
 printf("base=0x%llx table=0x%llx anchor=0x%llx\n",(unsigned long long)b,(unsigned long long)((uintptr_t)t),(unsigned long long)anchor);
 for(int i=0;i<17;i++) printf("type=%d entry=0x%04x target_off=0x%llx target=0x%llx\n",i,t[i],(unsigned long long)(0x829edc+((uintptr_t)t[i]<<2)),(unsigned long long)(anchor+((uintptr_t)t[i]<<2)));
 return 0;
}
