// E868 hosted static-table recovery for first-v4r5 token11/token5 handlers. No GPU access.
#include <dlfcn.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
int main(void){
 const char *P="/Applications/Xcode.app/Contents/PlugIns/GPUDebugger.ideplugin/Contents/Frameworks/GTShaderProfiler.framework/Versions/A/GTShaderProfiler";
 void*h=dlopen(P,RTLD_NOW|RTLD_LOCAL); if(!h){fprintf(stderr,"dlopen:%s\n",dlerror());return 2;}
 void*parse=dlsym(h,"agxps_aps_parser_parse"); Dl_info di={0}; if(!parse||!dladdr(parse,&di))return 3;
 uintptr_t base=(uintptr_t)di.dli_fbase;
 const uint16_t *t16=(const uint16_t*)(base+0xbe1020ULL);
 const uint32_t *t32=(const uint32_t*)(base+0xbe0c7cULL);
 printf("BASE=%p token11_table16=%p token11_table32=%p\n",(void*)base,(void*)t16,(void*)t32);
 for(unsigned i=0;i<8;i++) printf("TOKEN11_MAP subtype=%u halfword=0x%04x word=0x%08x\n",i,t16[i],t32[i]);
 // Exact E866 token fields after decoder-object -> parser-stack mapping.
 // token struct begins at sp+0xd8; handler sp+e8/eC/f0 are token +0x10/+0x14/+0x18.
 printf("E866_TOKEN11 decoded_subtype=5 mapped_halfword=0x%04x mapped_word=0x%08x payload_f18=1\n",t16[5],t32[5]);
 printf("E866_TOKEN5 mode=1 raw_index=1 transformed_index=0x61 payload64=0\n");
 dlclose(h); return 0;
}
