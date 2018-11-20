#include <karbon/core.h>
#include <stdlib.h>
#include "../thirdparty/tinydir.h"
#include "../allocators/tagged_allocator.h"


#ifdef _WIN32
#include <Windows.h>
#else
#include <dlfcn.h>
#endif

#include "../hooks/hooks.h"
#include "../ctx.h"
#include "../allocators/tagged_allocator.h"


/* --------------------------------------------------------------- Helpers -- */


void
kci_alloc_housekeeping(struct kc_ctx *ctx, void **addr, int *bytes) {
        struct kd_allocator_frame_desc allocator_desc;
        allocator_desc.type_id = KD_STRUCT_ALLOCATOR_FRAME_DESC;
        allocator_desc.ext = 0;

        struct kd_alloc_desc alloc_desc;
        alloc_desc.type_id = KD_STRUCT_ALLOC_DESC;
        alloc_desc.ext = 0;
        alloc_desc.allocator_desc = &allocator_desc;
        
        kdi_alloc_tagged(ctx, &alloc_desc, addr, bytes);
}


/* --------------------------------------------------------------- Context -- */


kc_result
kc_ctx_create(
        struct kc_ctx_desc * desc,
        kc_ctx_t *out_ctx)
{
        struct kc_ctx *ctx = (struct kc_ctx*)malloc(sizeof(*ctx));
        
        ctx->fn_ctx.vendor_ctx = desc->ctx_vendor_str_fn;
        ctx->fn_ctx.alloc = desc->alloc_fn;
        ctx->fn_ctx.win_get = desc->win_get_fn;
        ctx->fn_ctx.win_set = desc->win_set_fn;
        ctx->user_data = desc->user_data;
        
        kci_tag_alloc_init(&ctx->allocator_tagged);
        
        *out_ctx = ctx;
        
        return KC_OK;
}


kc_result
kc_ctx_user_data(
        kc_ctx_t ctx,
        void **out_user_data)
{
        *out_user_data = ctx->user_data;
        
        return KC_OK;
}


/* ----------------------------------------------------------- Application -- */


int
kci_str_ends_with(
        const char *str,
        const char *str_end)
{
        if (!str || !str_end) {
                return 0;
        }
        
        int len = strlen(str);
        int len_end = strlen(str_end);
        
        if (len_end >  len) {
                return 0;
        }
        
        return strncmp(str + len - len_end, str_end, len_end) == 0;
}


int
kci_load_libs(
        const char *base_dir,
        char *in_out_buffer,
        int *in_out_buffer_size,
        kc_lib *out_libs,
        int *out_count,
        void **funcs)
{
        int count = 0;
        kc_lib *libs = (kc_lib)&in_out_buffer[0];
        
        DIR *dir;
        struct dirent *ent;
        if ((dir = opendir (base_dir)) != NULL) {
                while ((ent = readdir (dir)) != NULL) {
                        if(kci_str_ends_with(ent->d_name, ".dylib")) {
                                char str_buffer[2048] = {0};
                                strcat(str_buffer, base_dir);
                                strcat(str_buffer, ent->d_name);
                                
                                #ifndef _WIN32
                                kc_lib lib =  dlopen(str_buffer, RTLD_NOW);
                                KD_LOAD_FN load_fn = (KD_LOAD_FN)dlsym(lib, "kd_load");
                                #else
                                kc_lib lib = (void*)LoadLibrary(str_buffer);
                                KD_LOAD_FN load_fn = (KD_LOAD_FN)GetProcAddress(lib, "kd_load");
                                #endif
                                
                                if(lib && load_fn) {
                                        load_fn(funcs);
                                        libs[count++] = lib;
                                }
                        }
                }
        }
        
        in_out_buffer_size += (sizeof(kc_lib) * count);
        in_out_buffer += (sizeof(kc_lib) * count);
        
        *out_libs = libs;
        *out_count = count;
        
        return 1;
}


kc_result
kc_application_start(
        kc_ctx_t ctx,
        const struct kc_application_desc * desc)
{
        void *addr = 0; int bytes = 0;
        kci_alloc_housekeeping(ctx, &addr, &bytes);
        
        if(!addr) {
                return KC_FAIL;
        }

        /* base directory */
        char *buffer = (char *)addr;
        const char *base_dir = "/Users/PhilCK/Developer/roa/build/bin/Development/";
        strncat(buffer, base_dir, bytes);
        bytes -= strlen(buffer);
        
        /* symbols to load */
        void * funcs[KD_FUNC_COUNT];
        funcs[KD_PTR_CTX] = (void*)ctx;
        funcs[KD_FUNC_CTX_VENDOR_STRING] = kdi_ctx_get_vendor_string;
        funcs[KD_FUNC_WINDOW_SET] = kdi_window_set;
        funcs[KD_FUNC_WINDOW_GET] = kdi_window_get;
        funcs[KD_FUNC_ALLOC] = kdi_alloc_tagged;
        
        /* find libs and load symbols */
        int lib_count = 0;
        kc_lib *libs = (kc_lib)&buffer[0];
        
        DIR *dir;
        struct dirent *ent;
        
        if ((dir = opendir (base_dir)) == NULL) {
                return KC_FAIL;
        }
        
        #if defined(__APPLE__)
        const char *ext = ".dylib";
        #elif defined(_WIN32)
        const char *ext = ".dll";
        #elif defined(__linux__)
        const char *ext = ".so";
        #endif

        while ((ent = readdir (dir)) != NULL) {
                if(!kci_str_ends_with(ent->d_name, ext)) {
                        continue;
                }
                
                char str_buffer[2 << 11] = {0};
                strcat(str_buffer, base_dir);
                strcat(str_buffer, ent->d_name);
                
                #ifndef _WIN32
                kc_lib lib =  dlopen(str_buffer, RTLD_NOW);
                void *sym = dlsym(lib, KD_HOOK_LOAD_STR);
                KD_LOAD_FN load_fn = (KD_LOAD_FN)sym;
                #else
                kc_lib lib = (void*)LoadLibrary(str_buffer);
                FARPROC sym = GetProcAddress(lib, KD_HOOK_LOAD_STR);
                KD_LOAD_FN load_fn = (KD_LOAD_FN)sym;
                #endif
                
                if(lib && load_fn) {
                        load_fn(funcs);
                        libs[lib_count++] = lib;
                }
        }
        
        buffer += (sizeof(kc_lib) * lib_count);
        bytes -= (sizeof(kc_lib) * lib_count);
        
        /* copy libs out of frame buffer */
        int lib_bytes = sizeof(kc_lib) * lib_count;
        
        ctx->libs.count = lib_count;
        ctx->libs.libs = malloc(lib_bytes);
        memcpy(ctx->libs.libs, libs, lib_bytes);
        
        /* call project entry on libs */
        int i;
        
        for(i = 0; i < lib_count; ++i) {
                kc_lib lib = libs[i];
        
                #ifndef _WIN32
                void *sym = dlsym(lib, KD_HOOK_PROJECT_ENTRY_STR);
                KD_PROJENTRYFN entry_fn = (KD_PROJENTRYFN)sym;
                #else
                FARPROC sym = GetProcAddress(lib, KD_HOOK_PROJECT_ENTRY_STR);
                KD_PROJENTRYFN entry_fn = (KD_PROJENTRYFN)sym;
                #endif
                entry_fn();
        }
 
        return KC_OK;
}
