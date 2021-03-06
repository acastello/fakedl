#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dlfcn.h>
#include <GL/gl.h>

char *nosyms[] =
    /* { "glColorPointer", "glFogf", "glFogi", "glDepthRange", "glDepthMask"
    , "glClipPlane", "glLightf", "glLightfv", "wglSwapLayerBuffers"
    , "glDrawArrays", "glDrawElements", "glDepthFunc", "glLightModeli"
    , "glEnableClientState", "glVertexPointer", "glLightModelfv", "glBlendFunc"
    , "glTexGenfv", "glTexGeni", "glLoadMatrixf", "glLoadIdentity", "glPixelStorei"
    , "glColorMaterial", "glMaterialf", "glVertexAttribPointerARB"
    }; */
    /* { "glBindTexture", "glEnable", "glLightModelfv", "glDepthRange"
    , "glVertexAttribPointerARB", "glDrawRangeElementsEXT", "glMatrixMode"
    , "glLoadMatrixf", "glBufferDataARB", "glBufferSubDataARB", "glDisable"
    , "glActiveTextureARB", "glClientActiveTextureARB", "glEnableClientState"
    , "glBindProgramARB", "glBindBufferARB", "glDepthFunc", "glDepthMask"
    , "glProgramEnvParameters4fvEXT", "glDrawArrays", "glGenQueriesARB"
    , "glTexParameteri", "glGetProgramivARB", "glGenProgramsARB"
    }; */
    {"wglSwapLayerBuffers"
    };

char *debugsyms[] = 
    {
        "XQueryPointer"
    };

void *NOP(void)
{
    // puts("NOP");
    return NULL;
}

void *NOP_D(void)
{
    puts("NOP");
    usleep(10000);
    return NULL;
}

void *real_winelib;
void (*real_dll_load)(const char *, char *, int, int *);

__attribute__((constructor)) void fakedl_init(void)
{
    real_winelib = dlopen("libwine.so", RTLD_LAZY);
    real_dll_load = (void (*)(const char *, char *, int, int *)) dlsym(real_winelib, "wine_dll_load");
}

int is_out(const char *symbol)
{
    int i;
    for (i=0; i<sizeof(nosyms)/sizeof(void *); i++) {
        if (!strcmp(nosyms[i], symbol))
            return 1;
    }
    return 0;
}

int is_debug(const char *symbol)
{
    int i;
    for (i=0; i<sizeof(debugsyms)/sizeof(void *); i++) {
        if (!strcmp(nosyms[i], symbol))
            return 1;
    }
    return 0;
}
    

void *wine_dlsym(void *handle, const char *symbol, char *error, size_t errorsize)
{
    void *ret;
    const char *s;

    printf("dlsym (%p, %s)\n", handle, symbol);
    dlerror(); dlerror();
    if (is_debug(symbol))
        return NOP_D;
    if (is_out(symbol))
        return NOP;
    ret = dlsym(handle, symbol);
    s = dlerror();
    if (error && errorsize) {
        if (s) {
            size_t len = strlen(s);
            if (len >= errorsize) 
                len = errorsize - 1;
            memcpy(error, s, len);
            error[len] = 0;
        }
        else
            error[0] = 0;
    }
    dlerror();
    return ret;
}

void wine_dll_load(const char *filename, char *error, int errorsize, int *file_exists)
{
    #define str(s) xstr(s)
    #define xstr(s) #s
    #define WOV "./wov3_" str(__WORDSIZE) ".dll.so"
    printf("dll_load %s\n", filename);
    real_dll_load(filename, error, errorsize, file_exists);
    if (!strcmp("user32.dll", filename)) {
        char err[1024];
        int __p;
        puts(WOV);
        // real_dll_load(WOV, err, sizeof(err), &__p);
    }
    return;
}
