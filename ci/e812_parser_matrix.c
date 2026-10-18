#include <dlfcn.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
typedef struct{unsigned char b[0x68];} desc_t;
typedef void*(*gc_t)(uint32_t,uint32_t,uint32_t,uint32_t);
typedef void(*vp_t)(void*);
typedef int(*valid_t)(void*);
typedef uint32_t(*get_t)(void*);
typedef desc_t(*dc_t)(void);
typedef void*(*pc_t)(desc_t*);
int main(void){
 const char*P="/Applications/Xcode.app/Contents/PlugIns/GPUDebugger.ideplugin/Contents/Frameworks/GTShaderProfiler.framework/Versions/A/GTShaderProfiler";
 void*h=dlopen(P,RTLD_NOW|RTLD_LOCAL); if(!h){fprintf(stderr,"dlopen:%s\n",dlerror());return 2;}
 gc_t gc=(gc_t)dlsym(h,"agxps_gpu_create"); vp_t gd=(vp_t)dlsym(h,"agxps_gpu_destroy"),pd=(vp_t)dlsym(h,"agxps_aps_parser_destroy"); valid_t gv=(valid_t)dlsym(h,"agxps_gpu_is_valid"); get_t gg=(get_t)dlsym(h,"agxps_gpu_get_gen"),gvar=(get_t)dlsym(h,"agxps_gpu_get_variant"),gr=(get_t)dlsym(h,"agxps_gpu_get_rev"); dc_t dc=(dc_t)dlsym(h,"agxps_aps_descriptor_create"); pc_t pc=(pc_t)dlsym(h,"agxps_aps_parser_create");
 if(!gc||!gd||!pd||!gv||!gg||!gvar||!gr||!dc||!pc){fprintf(stderr,"missing symbol\n");return 3;}
 unsigned ok=0;
 for(uint32_t gen=13;gen<=16;gen++) for(uint32_t var=0;var<=8;var++) for(uint32_t rev=0;rev<=8;rev++){
   void*g=gc(gen,var,rev,0); if(!g)continue; int valid=gv(g); if(valid){
     desc_t d=dc(); memcpy(d.b,&g,8); *(uint32_t*)(d.b+8)=0x400; *(uint32_t*)(d.b+12)=0x40; *(uint32_t*)(d.b+16)=0x1000; *(uint64_t*)(d.b+24)=0x1000;
     void*p=pc(&d); if(p){printf("parser_ok requested=%u,%u,%u getters=%u,%u,%u\n",gen,var,rev,gg(g),gvar(g),gr(g));ok++;pd(p);}
   } gd(g);
 }
 void*g=gc(15,4,5,0); printf("target gpu=%p valid=%d getters=%u,%u,%u\n",g,g?gv(g):0,g?gg(g):0,g?gvar(g):0,g?gr(g):0); if(g)gd(g);
 printf("parser_ok_count=%u\n",ok); return 0;
}
