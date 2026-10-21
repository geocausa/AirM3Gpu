#include <dlfcn.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
typedef void *obj_t;
typedef void *sel_t;
typedef obj_t (*objc_getClass_t)(const char *);
typedef sel_t (*sel_registerName_t)(const char *);
static void dump_objc_counter_configs(void *h) {
    objc_getClass_t getClass=(objc_getClass_t)dlsym(RTLD_DEFAULT,"objc_getClass");
    sel_registerName_t sel=(sel_registerName_t)dlsym(RTLD_DEFAULT,"sel_registerName");
    void *msg=dlsym(RTLD_DEFAULT,"objc_msgSend");
    if(!getClass||!sel||!msg){printf("OBJC_RUNTIME unavailable\n");return;}
    obj_t cls=getClass("XRGPUAPSDataProcessor");
    if(!cls){printf("OBJC_CLASS XRGPUAPSDataProcessor unavailable\n");return;}
    obj_t (*send0)(obj_t,sel_t)=(obj_t(*)(obj_t,sel_t))msg;
    obj_t (*initp)(obj_t,sel_t,unsigned,unsigned,unsigned,obj_t,unsigned)=(obj_t(*)(obj_t,sel_t,unsigned,unsigned,unsigned,obj_t,unsigned))msg;
    obj_t (*cfgfn)(obj_t,sel_t,bool,unsigned long long)=(obj_t(*)(obj_t,sel_t,bool,unsigned long long))msg;
    const char *(*utf8)(obj_t,sel_t)=(const char *(*)(obj_t,sel_t))msg;
    obj_t obj=send0(cls,sel("alloc"));
    obj_t proc=initp(obj,sel("initWithGPUGeneration:variant:rev:config:options:"),15,4,5,NULL,0);
    printf("OBJC_PROCESSOR %p\n",proc);
    if(!proc)return;
    for(unsigned internal=0;internal<2;internal++) for(unsigned set=0;set<3;set++) {
        obj_t cfg=cfgfn(proc,sel("counterConfigForGRC:counterSet:"),internal!=0,set);
        obj_t desc=cfg?send0(cfg,sel("description")):NULL;
        const char *txt=desc?utf8(desc,sel("UTF8String")):NULL;
        printf("COUNTER_CONFIG internal=%u set=%u %s\n",internal,set,txt?txt:"(null)");
    }
    send0(proc,sel("release"));
    (void)h;
}
typedef struct { unsigned char b[0x68]; } desc_t;
typedef struct { uint64_t count, bits; } stat_t;
typedef void *(*gpu_create_t)(uint32_t,uint32_t,uint32_t,uint32_t);
typedef void (*destroy_t)(void*);
typedef desc_t (*desc_create_t)(void);
typedef void *(*parser_create_t)(desc_t*);
typedef void *(*parse_t)(void*,const void*,uint64_t,uint32_t,uint32_t*);
typedef int (*stats_t)(void*,uint64_t,stat_t*);
typedef uint64_t (*num_t)(void*);
typedef int (*trace_t)(void*,void**,uint64_t,uint64_t);
typedef int (*copy_u64_t)(void*,uint64_t*,uint64_t,uint64_t);
static void *sym(void *h,const char*n){void*p=dlsym(h,n);if(!p){fprintf(stderr,"missing %s\n",n);exit(3);}return p;}
static int load(const char *p,uint8_t out[4096]){FILE*f=fopen(p,"rb");if(!f)return 0;size_t n=fread(out,1,4096,f);int extra=fgetc(f);fclose(f);return n==4096&&extra==EOF;}
struct api{void*h;gpu_create_t gc;destroy_t gd,pd,dd;desc_create_t dc;parser_create_t pc;parse_t parse;stats_t stats;num_t toks,bits,wn,en,kn,tn,npc,nadv;trace_t et;copy_u64_t getpcs;};
static void replay(struct api*a,const char*label,const char*path){
 uint8_t page[4096];if(!load(path,page)){fprintf(stderr,"load fail %s\n",path);exit(4);}void*g=a->gc(15,4,5,0);if(!g){fprintf(stderr,"gpu create fail\n");exit(5);}desc_t d=a->dc();memcpy(d.b,&g,8);*(uint32_t*)(d.b+8)=0x400;*(uint32_t*)(d.b+12)=0x40;*(uint32_t*)(d.b+16)=0x1000;*(uint64_t*)(d.b+24)=0x1000;void*p=a->pc(&d);uint32_t err=999;void*r=p?a->parse(p,page,4096,0x21,&err):0;
 printf("PARSE page=%s err=%u profile=%p tokens=%llu bits=%llu\n",label,err,r,r?(unsigned long long)a->toks(r):0,r?(unsigned long long)a->bits(r):0);
 if(r){for(unsigned i=0;i<17;i++){stat_t s={0};int ok=a->stats(r,i,&s);printf("TOKEN page=%s type=%u ok=%d count=%llu bits=%llu\n",label,i,ok,(unsigned long long)s.count,(unsigned long long)s.bits);}uint64_t w=a->wn(r),e=a->en(r),k=a->kn(r),t=a->tn(r);printf("OBJECTS page=%s work=%llu esl=%llu kicks=%llu tiles=%llu\n",label,(unsigned long long)w,(unsigned long long)e,(unsigned long long)k,(unsigned long long)t);for(uint64_t i=0;i<e;i++){void*tr=0;int ok=a->et(r,&tr,i,1);uint64_t np=(ok&&tr)?a->npc(tr):0,na=(ok&&tr)?a->nadv(tr):0;printf("ESL_TRACE page=%s i=%llu ok=%d init_pcs=%llu advances=%llu\n",label,(unsigned long long)i,ok,(unsigned long long)np,(unsigned long long)na);if(np){uint64_t pcs[8]={0};uint64_t take=np<8?np:8;int pok=a->getpcs(tr,pcs,0,take);for(uint64_t j=0;j<take;j++)printf("ESL_PC page=%s i=%llu n=%llu ok=%d pc=0x%llx\n",label,(unsigned long long)i,(unsigned long long)j,pok,(unsigned long long)pcs[j]);}}a->dd(r);}if(p)a->pd(p);a->gd(g);
}
int main(void){system("cp /Applications/Xcode_16.3.app/Contents/PlugIns/GPUDebugger.ideplugin/Contents/Frameworks/GTShaderProfiler.framework/Versions/A/GTShaderProfiler out/GTShaderProfiler-xcode16.3");system("shasum -a 256 out/GTShaderProfiler-xcode16.3 > out/GTShaderProfiler-xcode16.3.sha256");system("cp /Applications/Xcode_16.3.app/Contents/Applications/Instruments.app/Contents/PlugIns/GPUPlugin.xrplugin/Contents/Resources/GPUCounterGraph.plist out/GPUCounterGraph-xcode16.3.plist");system("shasum -a 256 out/GPUCounterGraph-xcode16.3.plist > out/GPUCounterGraph-xcode16.3.sha256");const char*P="/Applications/Xcode.app/Contents/PlugIns/GPUDebugger.ideplugin/Contents/Frameworks/GTShaderProfiler.framework/Versions/A/GTShaderProfiler";struct api a={0};a.h=dlopen(P,RTLD_NOW|RTLD_LOCAL);if(!a.h){fprintf(stderr,"dlopen:%s\n",dlerror());return 2;}dump_objc_counter_configs(a.h);a.gc=(gpu_create_t)sym(a.h,"agxps_gpu_create");a.gd=(destroy_t)sym(a.h,"agxps_gpu_destroy");a.dc=(desc_create_t)sym(a.h,"agxps_aps_descriptor_create");a.pc=(parser_create_t)sym(a.h,"agxps_aps_parser_create");a.pd=(destroy_t)sym(a.h,"agxps_aps_parser_destroy");a.dd=(destroy_t)sym(a.h,"agxps_aps_profile_data_destroy");a.parse=(parse_t)sym(a.h,"agxps_aps_parser_parse");a.stats=(stats_t)sym(a.h,"agxps_aps_profile_data_get_token_type_stats");a.toks=(num_t)sym(a.h,"agxps_aps_profile_data_get_parsed_tokens_num");a.bits=(num_t)sym(a.h,"agxps_aps_profile_data_get_parsed_bits_num");a.wn=(num_t)sym(a.h,"agxps_aps_profile_data_get_work_cliques_num");a.en=(num_t)sym(a.h,"agxps_aps_profile_data_get_esl_cliques_num");a.kn=(num_t)sym(a.h,"agxps_aps_profile_data_get_kicks_num");a.tn=(num_t)sym(a.h,"agxps_aps_profile_data_get_tiles_num");a.et=(trace_t)sym(a.h,"agxps_aps_profile_data_get_esl_clique_instruction_trace");a.npc=(num_t)sym(a.h,"agxps_aps_clique_instruction_trace_get_init_pcs_num");a.nadv=(num_t)sym(a.h,"agxps_aps_clique_instruction_trace_get_pc_advances_num");a.getpcs=(copy_u64_t)sym(a.h,"agxps_aps_clique_instruction_trace_get_init_pcs");replay(&a,"core00-page00","ci/e894-core00-page00.bin");replay(&a,"core08-page00","ci/e894-core08-page00.bin");dlclose(a.h);return 0;}
