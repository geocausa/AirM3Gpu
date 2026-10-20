// E862 hosted synthetic token-wire probe; private parser helper only, no GPU access.
#include <dlfcn.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

typedef void (*decode_token_t)(void *out, const uint8_t *src, uint64_t *bitpos, uint8_t *ok);
typedef struct { uint8_t b[0x68]; } token_t;

static void setbit(uint8_t *p, unsigned bit) { p[bit >> 3] |= (uint8_t)(1u << (bit & 7)); }
static void decode(decode_token_t fn, const uint8_t *src, const char *tag) {
    token_t t; memset(&t,0,sizeof(t)); uint64_t pos=0; uint8_t ok=0;
    fn(&t,src,&pos,&ok);
    uint32_t f0=0,f4=0,f10_32=0,f14=0; uint16_t f8=0,f10=0,f12=0; uint64_t f18=0,f20=0,f28=0;
    memcpy(&f0,t.b+0x00,4); memcpy(&f4,t.b+0x04,4); memcpy(&f8,t.b+0x08,2);
    memcpy(&f10,t.b+0x10,2); memcpy(&f12,t.b+0x12,2); memcpy(&f10_32,t.b+0x10,4); memcpy(&f14,t.b+0x14,4);
    memcpy(&f18,t.b+0x18,8); memcpy(&f20,t.b+0x20,8); memcpy(&f28,t.b+0x28,8);
    printf("%s ok=%u bitpos=%llu type=%u f4=%u f8=%u f10=%u f12=%u f10_32=%u f14=%u f18=%016llx f20=%016llx f28=%016llx\n",
           tag,ok,(unsigned long long)pos,f0,f4,f8,f10,f12,f10_32,f14,
           (unsigned long long)f18,(unsigned long long)f20,(unsigned long long)f28);
}
static void dump_one(decode_token_t fn, unsigned b0, unsigned fill) {
    uint8_t src[128]; memset(src,(int)fill,sizeof(src)); src[0]=(uint8_t)b0;
    char tag[64]; snprintf(tag,sizeof(tag),"fill=%02x b0=%02x",fill,b0); decode(fn,src,tag);
}

int main(void) {
    const char *path="/Applications/Xcode.app/Contents/PlugIns/GPUDebugger.ideplugin/Contents/Frameworks/GTShaderProfiler.framework/Versions/A/GTShaderProfiler";
    void *h=dlopen(path,RTLD_NOW|RTLD_LOCAL); if(!h){fprintf(stderr,"dlopen:%s\n",dlerror());return 2;}
    void *sym=dlsym(h,"agxps_aps_parser_parse"); if(!sym)return 3;
    Dl_info di={0}; if(!dladdr(sym,&di))return 4; uintptr_t base=(uintptr_t)di.dli_fbase;
    decode_token_t fn=(decode_token_t)(base+0x838de8ULL);
    printf("base=%p helper=%p helper_off=0x838de8\n",(void*)base,(void*)fn);

    puts("=== first-byte sweep, zero fill ===");
    for(unsigned b=0;b<256;b++) dump_one(fn,b,0x00);

    puts("=== token0 raw-bit map (prefix 00, bits 2..14) ===");
    { uint8_t src[128]={0}; decode(fn,src,"t0 baseline");
      for(unsigned bit=2;bit<=14;bit++){ memset(src,0,sizeof(src)); setbit(src,bit); char tag[64]; snprintf(tag,sizeof(tag),"t0 bit=%u",bit); decode(fn,src,tag); } }

    puts("=== token7 cache-reference raw-bit map (prefix low4=0x2, flag bit12=0) ===");
    { uint8_t src[128]={0}; src[0]=0x02; decode(fn,src,"t7ref baseline");
      for(unsigned bit=4;bit<=11;bit++){ memset(src,0,sizeof(src)); src[0]=0x02; setbit(src,bit); char tag[64]; snprintf(tag,sizeof(tag),"t7ref clique_bit=%u",bit); decode(fn,src,tag); }
      for(unsigned bit=13;bit<=17;bit++){ memset(src,0,sizeof(src)); src[0]=0x02; setbit(src,bit); char tag[64]; snprintf(tag,sizeof(tag),"t7ref cache_bit=%u",bit); decode(fn,src,tag); } }

    puts("=== token7 inline-initial-PC raw-bit map (prefix low4=0x2, flag bit12=1) ===");
    { uint8_t src[128]={0}; src[0]=0x02; setbit(src,12); decode(fn,src,"t7pc baseline");
      for(unsigned bit=4;bit<=11;bit++){ memset(src,0,sizeof(src)); src[0]=0x02; setbit(src,12); setbit(src,bit); char tag[64]; snprintf(tag,sizeof(tag),"t7pc clique_bit=%u",bit); decode(fn,src,tag); }
      for(unsigned bit=13;bit<=59;bit++){ memset(src,0,sizeof(src)); src[0]=0x02; setbit(src,12); setbit(src,bit); char tag[64]; snprintf(tag,sizeof(tag),"t7pc payload_bit=%u",bit); decode(fn,src,tag); } }

    puts("=== token16 framing aliases ===");
    const unsigned aliases[]={0x0f,0x4f,0x8f,0xcf}; for(unsigned i=0;i<4;i++) dump_one(fn,aliases[i],0xff);
    dlclose(h); return 0;
}
