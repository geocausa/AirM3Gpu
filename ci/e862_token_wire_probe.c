#include <dlfcn.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef void *obj_t; typedef void *cls_t; typedef void *sel_t;
typedef cls_t (*objc_getClass_t)(const char*);
typedef sel_t (*sel_registerName_t)(const char*);
typedef void *(*class_getInstanceMethod_t)(cls_t,sel_t);
typedef obj_t (*msg_id_t)(obj_t,sel_t);
typedef uint64_t (*msg_u64_t)(obj_t,sel_t);
static void inspect_framework(const char *path){
  void *h=dlopen(path,RTLD_NOW|RTLD_LOCAL);
  printf("DLOPEN path=%s ok=%d err=%s\n",path,h!=NULL,h?"none":dlerror());
  if(!h)return;
  void *oh=dlopen("/usr/lib/libobjc.A.dylib",RTLD_NOW|RTLD_GLOBAL);
  if(!oh){printf("OBJC_DLOPEN_FAIL %s\n",dlerror());dlclose(h);return;}
  objc_getClass_t getc=(objc_getClass_t)dlsym(oh,"objc_getClass");
  sel_registerName_t sels=(sel_registerName_t)dlsym(oh,"sel_registerName");
  class_getInstanceMethod_t gim=(class_getInstanceMethod_t)dlsym(oh,"class_getInstanceMethod");
  void *ms=dlsym(oh,"objc_msgSend");
  cls_t c=getc?getc("GTGPUAPSConfig"):0;
  printf("CLASS path=%s GTGPUAPSConfig=%p\n",path,c);
  if(c&&sels&&gim&&ms){
    const char *need[]={"initForProfiling","cliqueTraceLevel","emitPos","emitThreadControlFlow","countPeriod","pulsePeriod","systemTimePeriod","tileTracing","eslInstTracing","syncDrainMode",NULL};
    for(int i=0;need[i];i++) printf("METHOD %s present=%d\n",need[i],gim(c,sels(need[i]))!=NULL);
    if(gim(c,sels("initForProfiling"))){
      obj_t o=((msg_id_t)ms)((obj_t)c,sels("alloc"));
      obj_t p=((msg_id_t)ms)(o,sels("initForProfiling"));
      printf("PROFILE obj=%p",p);
      const char *keys[]={"cliqueTraceLevel","emitPos","emitThreadControlFlow","countPeriod","pulsePeriod","systemTimePeriod","tileTracing","eslInstTracing","syncDrainMode",NULL};
      for(int i=0;keys[i];i++) if(gim(c,sels(keys[i]))) printf(" %s=%llu",keys[i],(unsigned long long)((msg_u64_t)ms)(p,sels(keys[i])));
      printf("\n");
    }
  }
  dlclose(h);
}
int main(void){
  dlopen("/System/Library/Frameworks/Foundation.framework/Foundation",RTLD_NOW|RTLD_GLOBAL);
  FILE *fp=popen("find /Applications/Xcode_16.3.app -type f \\( -name GPUToolsReplay -o -name GPUToolsReplay.framework \\) 2>/dev/null | head -40","r");
  char line[4096];int n=0;
  if(fp){while(fgets(line,sizeof(line),fp)){line[strcspn(line,"\r\n")]=0;if(!*line)continue;printf("FOUND %s\n",line);size_t L=strlen(line);if(L>10&&strstr(line,"GPUToolsReplay.framework")&&strcmp(line+L-10,".framework")){inspect_framework(line);n++;}}pclose(fp);}
  const char *fallbacks[]={
    "/Applications/Xcode_16.3.app/Contents/SharedFrameworks/GPUToolsReplay.framework/Versions/A/GPUToolsReplay",
    "/Applications/Xcode_16.3.app/Contents/Frameworks/GPUToolsReplay.framework/Versions/A/GPUToolsReplay",
    "/Applications/Xcode_16.3.app/Contents/Developer/Library/PrivateFrameworks/GPUToolsReplay.framework/Versions/A/GPUToolsReplay",
    NULL};
  for(int i=0;fallbacks[i];i++) inspect_framework(fallbacks[i]);
  printf("SUMMARY inspected=%d\n",n);
  return 0;
}
