#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

typedef struct XAsyncBlock {
    void *queue;
    void *context;
    void (__stdcall *callback)(struct XAsyncBlock *);
    unsigned char internal[32];
} XAsyncBlock;

static int got;
static void __stdcall done(XAsyncBlock *b) { (void)b; got = 1; }
static HRESULT __stdcall work(XAsyncBlock *b) { (void)b; return 0; }

int main(void) {
    HMODULE m = LoadLibraryA("xgameruntime.dll");
    HRESULT (__stdcall *init)(uint64_t,uint64_t,uint32_t,const void*);
    HRESULT (__stdcall *query)(const GUID*,const GUID*,void**);
    GUID id = {0x073b7dcb,0x1fcf,0x4030,{0x94,0xbe,0xe3,0xc9,0xeb,0x62,0x34,0x28}};
    void *thr = NULL, *q = NULL;
    void **vt;
    XAsyncBlock b;
    HRESULT hr;
    if (!m) return 10;
    init=(void*)GetProcAddress(m,"InitializeApiImplEx2");
    query=(void*)GetProcAddress(m,"QueryApiImpl");
    if (!init || !query) return 11;
    if (init(0x633600002712ull,0x65f400001e9full,0,NULL)) return 12;
    if (query(&id,&id,&thr) || !thr) return 13;
    vt=*(void***)thr;
    if (((HRESULT (__stdcall *)(void*,uint32_t,uint32_t,void**))vt[12])(thr,0,0,&q)) return 14;
    memset(&b,0,sizeof b);
    b.queue=q; b.callback=done;
    hr=((HRESULT (__stdcall *)(void*,XAsyncBlock*,void*))vt[6])(thr,&b,work);
    if (hr) return 15;
    ((void (__stdcall *)(void*,void*))vt[17])(thr,q);
    hr=((HRESULT (__stdcall *)(void*,XAsyncBlock*,unsigned char))vt[3])(thr,&b,1);
    printf("status=%08lx callback=%d\n",(unsigned long)hr,got);
    return (hr==0 && got) ? 0 : 16;
}
