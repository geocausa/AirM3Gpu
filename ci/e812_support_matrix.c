#include <dlfcn.h>
#include <stdint.h>
#include <stdio.h>
typedef int (*supported_t)(uint32_t,uint32_t,uint32_t);
typedef void*(*gc_t)(uint32_t,uint32_t,uint32_t,uint32_t);
typedef void(*gd_t)(void*);
int main(void){
 const char*P="/Applications/Xcode.app/Contents/PlugIns/GPUDebugger.ideplugin/Contents/Frameworks/GTShaderProfiler.framework/Versions/A/GTShaderProfiler";
 void*h=dlopen(P,RTLD_NOW|RTLD_LOCAL); if(!h){fprintf(stderr,"dlopen:%s\n",dlerror());return 2;}
 supported_t sup=(supported_t)dlsym(h,"agxps_aps_gpu_is_supported"); gc_t gc=(gc_t)dlsym(h,"agxps_gpu_create"); gd_t gd=(gd_t)dlsym(h,"agxps_gpu_destroy");
 printf("symbols supported=%p gpu_create=%p gpu_destroy=%p\n",(void*)sup,(void*)gc,(void*)gd);
 if(!sup||!gc||!gd)return 3;
 for(uint32_t gen=13;gen<=16;gen++) for(uint32_t var=0;var<=8;var++) for(uint32_t rev=0;rev<=8;rev++){
   int s=sup(gen,var,rev); if(s){void*g=gc(gen,var,rev,0);printf("supported gen=%u var=%u rev=%u gpu=%p\n",gen,var,rev,g);if(g)gd(g);}
 }
 printf("target supported=%d\n",sup(15,4,5));
 void*g=gc(15,4,5,0); printf("target gpu_create=%p\n",g); if(g)gd(g);
 return 0;
}
