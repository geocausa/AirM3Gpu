#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
typedef struct{unsigned char b[0x68];} desc_t;
typedef void*(*gc_t)(uint32_t,uint32_t,uint32_t,uint32_t); typedef void(*vp_t)(void*); typedef desc_t(*dc_t)(void); typedef void*(*pc_t)(desc_t*);
int main(void){
 const char*P="/Applications/Xcode.app/Contents/PlugIns/GPUDebugger.ideplugin/Contents/Frameworks/GTShaderProfiler.framework/Versions/A/GTShaderProfiler";
 void*h=dlopen(P,RTLD_NOW|RTLD_LOCAL); if(!h){fprintf(stderr,"dlopen:%s\n",dlerror());return 2;}
 void *wrap=dlsym(h,"agxps_aps_parser_parse"); Dl_info wi={0}; dladdr(wrap,&wi); uintptr_t base=(uintptr_t)wi.dli_fbase;
 gc_t gc=(gc_t)dlsym(h,"agxps_gpu_create"); vp_t gd=(vp_t)dlsym(h,"agxps_gpu_destroy"),pd=(vp_t)dlsym(h,"agxps_aps_parser_destroy"); dc_t dc=(dc_t)dlsym(h,"agxps_aps_descriptor_create"); pc_t pc=(pc_t)dlsym(h,"agxps_aps_parser_create"); if(!gc||!gd||!pd||!dc||!pc||!wrap)return 3;
 void*g=gc(15,4,5,0); if(!g)return 4; desc_t d=dc(); memcpy(d.b,&g,8); *(uint32_t*)(d.b+8)=0x400; *(uint32_t*)(d.b+12)=0x40; *(uint32_t*)(d.b+16)=0x1000; *(uint64_t*)(d.b+24)=0x1000; void*p=pc(&d); if(!p)return 5;
 printf("base=%p wrapper=%p wrapper_off=0x%llx parser=%p\n",(void*)base,wrap,(unsigned long long)((uintptr_t)wrap-base),p);
 uintptr_t *obj=(uintptr_t*)p; for(int i=0;i<10;i++) printf("obj[%d]=0x%016llx\n",i,(unsigned long long)obj[i]);
 uintptr_t *vt=(uintptr_t*)obj[0]; printf("vtable=%p vtable_off=0x%llx\n",(void*)vt,(unsigned long long)((uintptr_t)vt-base));
 for(int i=-4;i<24;i++){uintptr_t q=vt[i]; Dl_info di={0}; const char*n="?"; unsigned long long off=0;if(q&&dladdr((void*)q,&di)){n=di.dli_sname?di.dli_sname:"?";off=(unsigned long long)(q-(uintptr_t)di.dli_fbase);}printf("vt[%2d]=0x%016llx off=0x%llx sym=%s\n",i,(unsigned long long)q,off,n);}
 pd(p);gd(g);return 0;
}
