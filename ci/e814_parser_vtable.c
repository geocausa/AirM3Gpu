#include <dlfcn.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

typedef struct { unsigned char b[0x68]; } desc_t;
typedef void *(*gpu_create_t)(uint32_t,uint32_t,uint32_t,uint32_t);
typedef void (*gpu_destroy_t)(void*);
typedef desc_t (*desc_create_t)(void);
typedef void *(*parser_create_t)(desc_t*);
typedef void (*parser_destroy_t)(void*);

static void show_ptr(const char *name, void *p, uintptr_t base) {
    Dl_info di = {0};
    if (p && dladdr(p, &di)) {
        printf("%s=%p off=0x%llx sym=%s image=%s\n", name, p,
               (unsigned long long)((uintptr_t)p - (uintptr_t)di.dli_fbase),
               di.dli_sname ? di.dli_sname : "?",
               di.dli_fname ? di.dli_fname : "?");
    } else {
        printf("%s=%p\n", name, p);
    }
    (void)base;
}

int main(void) {
    const char *path = "/Applications/Xcode.app/Contents/PlugIns/GPUDebugger.ideplugin/Contents/Frameworks/GTShaderProfiler.framework/Versions/A/GTShaderProfiler";
    void *h = dlopen(path, RTLD_NOW | RTLD_LOCAL);
    if (!h) { fprintf(stderr, "dlopen: %s\n", dlerror()); return 2; }

    gpu_create_t gc = (gpu_create_t)dlsym(h, "agxps_gpu_create");
    gpu_destroy_t gd = (gpu_destroy_t)dlsym(h, "agxps_gpu_destroy");
    desc_create_t dc = (desc_create_t)dlsym(h, "agxps_aps_descriptor_create");
    parser_create_t pc = (parser_create_t)dlsym(h, "agxps_aps_parser_create");
    parser_destroy_t pd = (parser_destroy_t)dlsym(h, "agxps_aps_parser_destroy");
    void *parse_export = dlsym(h, "agxps_aps_parser_parse");
    if (!gc || !gd || !dc || !pc || !pd || !parse_export) {
        fprintf(stderr, "required export missing\n"); return 3;
    }

    Dl_info bi = {0};
    if (!dladdr(parse_export, &bi)) { fprintf(stderr, "dladdr failed\n"); return 4; }
    uintptr_t base = (uintptr_t)bi.dli_fbase;
    printf("base=%p\n", (void*)base);
    show_ptr("parse_export", parse_export, base);
    show_ptr("parser_create", (void*)pc, base);

    void *g = gc(15,4,5,0);
    if (!g) { fprintf(stderr, "gpu create failed\n"); return 5; }
    desc_t d = dc();
    memcpy(d.b, &g, 8);
    *(uint32_t*)(d.b + 0x08) = 0x400;
    *(uint32_t*)(d.b + 0x0c) = 0x40;
    *(uint32_t*)(d.b + 0x10) = 0x1000;
    *(uint64_t*)(d.b + 0x18) = 0x1000;
    void *p = pc(&d);
    printf("gpu=%p parser=%p\n", g, p);
    if (!p) { gd(g); fprintf(stderr, "parser create failed\n"); return 6; }

    uintptr_t *obj = (uintptr_t*)p;
    for (int i = 0; i < 12; i++) {
        uintptr_t q = obj[i];
        printf("obj+0x%02x=0x%016" PRIxPTR, i*8, q);
        if (q >= base && q < base + 0x10000000ULL) printf(" image_off=0x%llx", (unsigned long long)(q-base));
        putchar('\n');
    }

    void **vt = *(void ***)p;
    printf("vtable=%p", (void*)vt);
    if ((uintptr_t)vt >= base && (uintptr_t)vt < base + 0x10000000ULL)
        printf(" off=0x%llx", (unsigned long long)((uintptr_t)vt-base));
    putchar('\n');
    for (int i = -4; i < 24; i++) {
        void *q = vt[i];
        Dl_info qi = {0};
        if (q && dladdr(q, &qi)) {
            printf("vt[%2d]=%p off=0x%llx sym=%s\n", i, q,
                   (unsigned long long)((uintptr_t)q-(uintptr_t)qi.dli_fbase),
                   qi.dli_sname ? qi.dli_sname : "?");
        } else printf("vt[%2d]=%p\n", i, q);
    }

    pd(p);
    gd(g);
    dlclose(h);
    return 0;
}
