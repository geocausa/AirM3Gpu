// E862 hosted synthetic token-wire probe; private parser helper only, no GPU access.
#include <dlfcn.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

typedef void (*decode_token_t)(void *out, const uint8_t *src, uint64_t *bitpos, uint8_t *ok);

typedef struct { uint8_t b[0x68]; } token_t;

static void dump_one(decode_token_t fn, unsigned b0, unsigned fill) {
    uint8_t src[128]; memset(src, (int)fill, sizeof(src)); src[0]=(uint8_t)b0;
    token_t t; memset(&t,0xcc,sizeof(t)); uint64_t pos=0; uint8_t ok=0;
    fn(&t,src,&pos,&ok);
    uint32_t f0=0,f4=0,f10=0; uint16_t f8=0,fea=0; uint64_t f18=0,f20=0,f28=0;
    memcpy(&f0,t.b+0x00,4); memcpy(&f4,t.b+0x04,4); memcpy(&f8,t.b+0x08,2);
    memcpy(&f10,t.b+0x10,4); memcpy(&fea,t.b+0x0a,2);
    memcpy(&f18,t.b+0x18,8); memcpy(&f20,t.b+0x20,8); memcpy(&f28,t.b+0x28,8);
    printf("fill=%02x b0=%02x ok=%u bitpos=%llu type=%u f4=%u len8=%u f0a=%u f10=%u f18=%016llx f20=%016llx f28=%016llx\n",
           fill,b0,ok,(unsigned long long)pos,f0,f4,f8,fea,f10,
           (unsigned long long)f18,(unsigned long long)f20,(unsigned long long)f28);
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
    puts("=== known framing aliases, ff fill ===");
    const unsigned aliases[]={0x0f,0x4f,0x8f,0xcf};
    for(unsigned i=0;i<4;i++) dump_one(fn,aliases[i],0xff);
    puts("=== selected token candidates with aa fill ===");
    for(unsigned b=0;b<256;b++) {
        uint8_t src[128]; memset(src,0xaa,sizeof(src)); src[0]=(uint8_t)b;
        token_t t; memset(&t,0,sizeof(t)); uint64_t pos=0; uint8_t ok=0; fn(&t,src,&pos,&ok);
        uint32_t ty=0; memcpy(&ty,t.b,4);
        if(ok && ty<17) dump_one(fn,b,0xaa);
    }
    dlclose(h); return 0;
}
