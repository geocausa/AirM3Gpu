// E868 hosted token name/description census for Xcode 16.3 (15,4,5). No GPU access.
#include <dlfcn.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef void *(*gpu_create_t)(uint32_t,uint32_t,uint32_t,uint32_t);
typedef void (*gpu_destroy_t)(void*);
typedef uint64_t (*num_t)(void*);
typedef int (*text_t)(void*,uint64_t,char*,uint64_t);
static void *sym(void *h,const char*n){void*p=dlsym(h,n);if(!p){fprintf(stderr,"missing %s\n",n);exit(3);}return p;}
int main(void){
 const char *P="/Applications/Xcode.app/Contents/PlugIns/GPUDebugger.ideplugin/Contents/Frameworks/GTShaderProfiler.framework/Versions/A/GTShaderProfiler";
 void*h=dlopen(P,RTLD_NOW|RTLD_LOCAL);if(!h){fprintf(stderr,"dlopen:%s\n",dlerror());return 2;}
 gpu_create_t create=(gpu_create_t)sym(h,"agxps_gpu_create"); gpu_destroy_t destroy=(gpu_destroy_t)sym(h,"agxps_gpu_destroy");
 num_t count=(num_t)sym(h,"agxps_aps_get_token_type_num"); text_t name=(text_t)sym(h,"agxps_aps_get_token_type_name"); text_t desc=(text_t)sym(h,"agxps_aps_get_token_type_description");
 void*g=create(15,4,5,0); if(!g)return 4; uint64_t n=count(g); printf("TOKEN_TYPE_NUM=%llu\n",(unsigned long long)n);
 for(uint64_t i=0;i<n;i++){char nb[256]={0},db[1024]={0};int rn=name(g,i,nb,sizeof(nb));int rd=desc(g,i,db,sizeof(db));printf("TYPE=%llu name_rc=%d name=%s desc_rc=%d desc=%s\n",(unsigned long long)i,rn,nb,rd,db);}
 destroy(g);dlclose(h);return 0;
}
