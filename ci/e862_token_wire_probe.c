// E869 hosted single-token -> clique-surface probe, Xcode16.3 (15,4,5). No GPU access.
#include <dlfcn.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct { unsigned char b[0x68]; } desc_t;
typedef void *(*gpu_create_t)(uint32_t,uint32_t,uint32_t,uint32_t); typedef void(*destroy_t)(void*);
typedef desc_t(*desc_create_t)(void); typedef void*(*parser_create_t)(desc_t*); typedef void*(*parse_t)(void*,const void*,uint64_t,uint32_t,uint32_t*); typedef uint64_t(*num_t)(void*); typedef void(*decode_t)(void*,const uint8_t*,uint64_t*,uint8_t*);
static void *sym(void*h,const char*n){void*p=dlsym(h,n);if(!p){fprintf(stderr,"missing %s\n",n);exit(3);}return p;}
static void setbit(uint8_t *p,uint64_t bit,unsigned v){ if(v)p[bit>>3]|=(uint8_t)(1u<<(bit&7)); else p[bit>>3]&=(uint8_t)~(1u<<(bit&7)); }
static unsigned getbit(const uint8_t*p,uint64_t bit){return (p[bit>>3]>>(bit&7))&1u;}
static void append_bits(uint8_t*dst,uint64_t pos,const uint8_t*src,uint64_t n){for(uint64_t i=0;i<n;i++)setbit(dst,pos+i,getbit(src,i));}
static void one(void*h,decode_t dec,gpu_create_t gc,destroy_t gd,desc_create_t dc,parser_create_t pc,destroy_t pd,destroy_t dd,parse_t parse,num_t toks,num_t bits,num_t wn,num_t en,unsigned b0){
 uint8_t raw[4096]={0},probe[128]={0},frame[8]={0}; probe[0]=(uint8_t)b0; frame[0]=0x0f;
 unsigned char tok[0x68]={0}; uint64_t p=0;uint8_t ok=0;dec(tok,probe,&p,&ok);uint32_t type=0;memcpy(&type,tok,4);
 unsigned char ft[0x68]={0};uint64_t fp=0;uint8_t fok=0;dec(ft,frame,&fp,&fok);
 append_bits(raw,0,probe,p); append_bits(raw,p,frame,fp);
 void*g=gc(15,4,5,0);desc_t d=dc();memcpy(d.b,&g,8);*(uint32_t*)(d.b+8)=0x400;*(uint32_t*)(d.b+12)=0x40;*(uint32_t*)(d.b+16)=0x1000;*(uint64_t*)(d.b+24)=0x1000;void*par=pc(&d);uint32_t err=999;void*r=par?parse(par,raw,sizeof(raw),0x21,&err):0;
 printf("CASE b0=%02x decoded_ok=%u type=%u token_bits=%llu frame_ok=%u frame_bits=%llu parse_err=%u profile=%p parsed_tokens=%llu parsed_bits=%llu work=%llu esl=%llu\n",b0,ok,type,(unsigned long long)p,fok,(unsigned long long)fp,err,r,r?(unsigned long long)toks(r):0,r?(unsigned long long)bits(r):0,r?(unsigned long long)wn(r):0,r?(unsigned long long)en(r):0);
 if(r)dd(r);if(par)pd(par);if(g)gd(g);
}
int main(void){const char*P="/Applications/Xcode.app/Contents/PlugIns/GPUDebugger.ideplugin/Contents/Frameworks/GTShaderProfiler.framework/Versions/A/GTShaderProfiler";void*h=dlopen(P,RTLD_NOW|RTLD_LOCAL);if(!h)return 2;void*ps=sym(h,"agxps_aps_parser_parse");Dl_info di={0};if(!dladdr(ps,&di))return 4;uintptr_t base=(uintptr_t)di.dli_fbase;decode_t dec=(decode_t)(base+0x838de8);gpu_create_t gc=(gpu_create_t)sym(h,"agxps_gpu_create");destroy_t gd=(destroy_t)sym(h,"agxps_gpu_destroy"),pd=(destroy_t)sym(h,"agxps_aps_parser_destroy"),dd=(destroy_t)sym(h,"agxps_aps_profile_data_destroy");desc_create_t dc=(desc_create_t)sym(h,"agxps_aps_descriptor_create");parser_create_t pc=(parser_create_t)sym(h,"agxps_aps_parser_create");parse_t parse=(parse_t)ps;num_t toks=(num_t)sym(h,"agxps_aps_profile_data_get_parsed_tokens_num"),bits=(num_t)sym(h,"agxps_aps_profile_data_get_parsed_bits_num"),wn=(num_t)sym(h,"agxps_aps_profile_data_get_work_cliques_num"),en=(num_t)sym(h,"agxps_aps_profile_data_get_esl_cliques_num");
 const unsigned cases[]={0x05,0x0d,0x06,0x13,0x07,0x17,0x0b,0x02,0x01,0x15,0x0e}; for(unsigned i=0;i<sizeof(cases)/sizeof(cases[0]);i++)one(h,dec,gc,gd,dc,pc,pd,dd,parse,toks,bits,wn,en,cases[i]);dlclose(h);return 0;}
