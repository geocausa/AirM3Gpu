// E868 resolve Xcode16.3 token-name/description dispatch implementations. No GPU access.
#include <dlfcn.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef void*(*gpu_create_t)(uint32_t,uint32_t,uint32_t,uint32_t); typedef void(*destroy_t)(void*);
typedef int(*wrap_t)(void*,uint64_t,char*,uint64_t); typedef void*(*find_t)(void*,const void*);
struct tuple { uint32_t gen,variant,rev; };
static void *sym(void*h,const char*n){void*p=dlsym(h,n);if(!p){fprintf(stderr,"missing %s\n",n);exit(3);}return p;}
static void dump_impl(const char*label,uintptr_t slide,uintptr_t mapoff,find_t find){struct tuple t={15,4,5};void*map=(void*)(slide+mapoff);void*node=find(map,&t);void*end=(void*)((uintptr_t)map+8);printf("DISPATCH %s map=%p node=%p end=%p\n",label,map,node,end);if(node&&node!=end){uintptr_t impl=*(uintptr_t*)((unsigned char*)node+0x30);printf("IMPL %s addr=%p image_off=0x%llx bytes=",label,(void*)impl,(unsigned long long)(impl-slide));for(int i=0;i<96;i++)printf("%02x",((unsigned char*)impl)[i]);printf("\n");}}
int main(void){const char*P="/Applications/Xcode.app/Contents/PlugIns/GPUDebugger.ideplugin/Contents/Frameworks/GTShaderProfiler.framework/Versions/A/GTShaderProfiler";void*h=dlopen(P,RTLD_NOW|RTLD_LOCAL);if(!h)return 2;gpu_create_t create=(gpu_create_t)sym(h,"agxps_gpu_create");destroy_t destroy=(destroy_t)sym(h,"agxps_gpu_destroy");void*name=sym(h,"agxps_aps_get_token_type_name");void*desc=sym(h,"agxps_aps_get_token_type_description");void*g=create(15,4,5,0);if(!g)return 4;char b[8]={0};((wrap_t)name)(g,0,b,sizeof(b));((wrap_t)desc)(g,0,b,sizeof(b));uintptr_t slide=(uintptr_t)name-0x6f285c;find_t find=(find_t)(slide+0x6f5008);printf("SLIDE=%p name=%p desc=%p find=%p\n",(void*)slide,name,desc,(void*)find);dump_impl("name",slide,0x1108578,find);dump_impl("desc",slide,0x1108590,find);destroy(g);dlclose(h);return 0;}
