#include <karbon/core.h>
#include "../fundamental.h"
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include "../config.h"

#ifdef _WIN32
#include "../thirdparty/dirent.h"
#include "../renderer/renderer_dx12.h"
#else
#include <dirent.h>
#endif

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


void
kci_log_stub(const char *msg) {
        (void)msg;
}


/* --------------------------------------------------------------- Context -- */


kc_result
kc_ctx_create(
        struct kc_ctx_desc * desc,
        kc_ctx_t *out_ctx)
{
        //fundamental_startup();

        struct kc_ctx *ctx = new kc_ctx{};
        
        ctx->user_data = desc->user_data;
        ctx->log_fn = desc->log_fn ? desc->log_fn : kci_log_stub;
        
        //kci_tag_alloc_init(&ctx->allocator_tagged);
        kci_platform_setup(&ctx->platform);
       
        *out_ctx = ctx;

        ctx->log_fn("Karbon CTX created");
        
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


kc_result
kc_application_start(
        kc_ctx_t ctx,
        const struct kc_application_desc * desc)
{
        (void)desc;

        if(KC_EXTRA_LOGGING) {
                ctx->log_fn("Karbon Core Startup... ");
        }

        //void *addr = 0; int bytes = 0;
        //kci_alloc_housekeeping(ctx, &addr, &bytes);

        std::vector<unsigned char> buf(4096);
        void *addr = buf.data();
        int bytes = buf.size();
        


        if(!addr) {
                return KC_FAIL;
        }

        /* base directory */
        int count = 0;
        kdi_ctx_get_exe_dir(ctx, 0, &count);
        std::vector<char> base_dir;
        base_dir.resize(count);
        kdi_ctx_get_exe_dir(ctx, &base_dir[0], 0);

        /* symbols to load */
        void * funcs[KD_FUNC_COUNT];
        funcs[KD_PTR_CTX] = (void*)ctx;
        funcs[KD_FUNC_CTX_VENDOR_STRING] = (void*)kdi_ctx_get_vendor_string;
        funcs[KD_FUNC_CTX_GRAPHICS_API] = nullptr;
        funcs[KD_FUNC_CTX_EXE_DIR] = (void*)kdi_ctx_get_exe_dir;
        funcs[KD_FUNC_ALLOC] = (void*)kdi_alloc_tagged;
        funcs[KD_FUNC_WINDOW_GET] = (void*)kdi_window_get;
        funcs[KD_FUNC_WINDOW_SET] = (void*)kdi_window_set;
        funcs[KD_FUNC_INPUT_KEYBOARD_GET] = nullptr;
        funcs[KD_FUNC_OPENGL_MAKE_CURRENT] = (void*)kdi_gl_make_current;
        funcs[KD_FUNC_LOG] = (void*)kdi_log;
                
        DIR *dir;
        struct dirent *ent;
        
        if ((dir = opendir (base_dir.data())) == NULL) {
                return KC_FAIL;
        }
        
        #if defined(__APPLE__)
        const char *ext = ".dylib";
        #elif defined(_WIN32)
        const char *ext = ".dll";
        #elif defined(__linux__)
        const char *ext = ".so";
        #endif
        
        std::vector<kc_lib> loaded_libs;
        loaded_libs.reserve(32);
        
        while ((ent = readdir (dir)) != NULL) {
                std::string item_name = ent->d_name;
                
                /* check if file ends with extension */
                if(!kci_str_ends_with(item_name.c_str(), ext)) {
                        if(KC_EXTRA_LOGGING) {
                                std::string msg = "- Ignore " + item_name;
                                ctx->log_fn(msg.c_str());
                        }
                
                        continue;
                }

                if(KC_EXTRA_LOGGING) {
                        std::string msg = "- Trying " + item_name;
                        ctx->log_fn(msg.c_str());
                }

                /* try load */
                std::string path = base_dir.data();
                path.append(item_name);
                
                #ifndef _WIN32
                kc_lib lib =  dlopen(path.c_str(), RTLD_NOW);
                void *sym = dlsym(lib, KD_HOOK_LOAD_STR);
                KD_LOAD_FN load_fn = (KD_LOAD_FN)sym;
                #else
                kc_lib lib = (void*)LoadLibrary(path.c_str());
                FARPROC sym = GetProcAddress((HMODULE)lib, KD_HOOK_LOAD_STR);
                KD_LOAD_FN load_fn = (KD_LOAD_FN)sym;
                #endif
                
                if(lib && load_fn) {
                        if(KC_EXTRA_LOGGING) {
                                std::string msg = "- Loaded " + item_name;
                                ctx->log_fn(msg.c_str());
                        }
                        
                        if(load_fn(funcs)) {
                                loaded_libs.emplace_back(lib);
                        }
                } else {
                        if(KC_EXTRA_LOGGING && !lib) {
                                std::string msg = "- Load Fail " + item_name;
                                ctx->log_fn(msg.c_str());

                        }
                        if(KC_EXTRA_LOGGING && !load_fn) {
                                std::string msg = "- No Loader " + item_name;
                                ctx->log_fn(msg.c_str());
                        }
                }
        }
        
        /* copy libs out */
        int lib_bytes = sizeof(kc_lib) * loaded_libs.size();

        /* get tick funcs */
        KD_TICKFN tick_fn = 0;
        KD_STARTUPFN setup_fn = 0;
        KD_SHUTDOWNFN shutdown_fn = 0;

//        void *clib = ctx->apps.libs[0]; /* bug */
        void *clib = loaded_libs[0];
        void *sym;
        
        #if defined(__linux__) || defined(__APPLE__)
        sym = dlsym(clib, KD_HOOK_TICK_STR);
        KD_TICKHOOKFN tickhook_fn = (KD_TICKHOOKFN)sym;
        tickhook_fn ? tick_fn = tickhook_fn() : tick_fn = 0;

        sym = dlsym(clib, KD_HOOK_STARTUP_STR);
        KD_STARTUPHOOKFN setuphook_fn = (KD_STARTUPHOOKFN)sym;
        setuphook_fn ? setup_fn = setuphook_fn() : setup_fn = 0;

        sym = dlsym(clib, KD_HOOK_SHUTDOWN_STR);
        KD_SHUTDOWNHOOKFN shutdownhook_fn = (KD_SHUTDOWNHOOKFN)sym;
        shutdownhook_fn ? shutdown_fn = shutdownhook_fn() : shutdown_fn = 0;
        
        #elif defined(_WIN32)
        sym = GetProcAddress((HMODULE)clib, KD_HOOK_TICK_STR);
        KD_TICKHOOKFN tickhook_fn = (KD_TICKHOOKFN)sym;
        tickhook_fn ? tick_fn = tickhook_fn() : tick_fn = 0;

        sym = GetProcAddress((HMODULE)clib, KD_HOOK_STARTUP_STR);
        KD_STARTUPHOOKFN setuphook_fn = (KD_STARTUPHOOKFN)sym;
        setuphook_fn ? setup_fn = setuphook_fn() : setup_fn = 0;

        sym = GetProcAddress((HMODULE)clib, KD_HOOK_SHUTDOWN_STR);
        KD_SHUTDOWNHOOKFN shutdownhook_fn = (KD_SHUTDOWNHOOKFN)sym;
        shutdownhook_fn ? shutdown_fn = shutdownhook_fn() : shutdown_fn = 0;
        #endif

        if(KC_EXTRA_LOGGING) {
                ctx->log_fn("Karbon Core application loop");
        }

        if (setup_fn) {
                setup_fn();
        }

        /* loop */
        while(kci_platform_process(&ctx->platform)) {

                //fundamental_tick();
                
                if (tick_fn) {
                        tick_fn();
                }

//                renderer_dx12_render();
        }

        if(shutdown_fn) {
                shutdown_fn();
        }

        ctx->log_fn("Karbon has finished");

        return KC_OK;
}
