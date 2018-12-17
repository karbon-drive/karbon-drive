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


void
kci_load_libs(
        struct kd_app apps[],
        int count) {

        /* symbol strings */
        const char *sym_strs[] = {
                KD_HOOK_STARTUP_STR,
                KD_HOOK_TICK_STR,
                KD_HOOK_SHUTDOWN_STR,
        };

        /* get symbols */
        int i, j;
        for(i = 0; i < count; ++i) {
                void *syms[sizeof(sym_strs) / sizeof(sym_strs[0])]{};
                void *lib = apps[i].lib;
                
                int sym_count = sizeof(sym_strs) / sizeof(sym_strs[0]);
                for(j = 0; j < sym_count; ++j) {
                        #if defined(__linux__) || defined(__APPLE__)
                        void *sym = dlsym(lib, sym_strs[j]);
                        syms[j] = sym;
                        #elif defined(_WIN32)
                        void *sym = GetProcAddress((HMODULE)lib, sym_strs[j]);
                        syms[j] = sym;
                        #endif
                }
                
                struct kd_app *app = &apps[i];
                
                /* pull out the function pointers */
                /* order is important -  must match sym_strs */
                j = 0;
                if(syms[j]) {
                        app->start = ((KD_STARTUPHOOKFN)syms[j])();
                } else {
                        app->start = [](){};
                }
                ++j;
                
                if(syms[j]) {
                        app->tick = ((KD_TICKHOOKFN)syms[j])();
                } else {
                        app->tick = [](){};
                }
                ++j;
                
                if(syms[j]) {
                        app->close = ((KD_SHUTDOWNHOOKFN)syms[j])();
                } else {
                        app->close = [](){};
                }
                ++j;
        }
}


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

        /* base directory */
        int count = 0;
        std::vector<char> base_dir;

        kdi_ctx_get_exe_dir(ctx, 0, &count);
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
        
        std::vector<kd_app> loaded_libs;
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
                void *lib =  dlopen(path.c_str(), RTLD_NOW);
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
                                loaded_libs.emplace_back(
                                        kd_app{
                                                lib,
                                                nullptr,
                                                nullptr,
                                                nullptr});
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
                        
                        if(lib) {
                                #ifndef _WIN32
                                dlclose(lib);
                                #else
                                assert(0); // what is close on windows */
                                #endif
                        }
                }
        }
        
        /* copy libs out */
        loaded_libs.shrink_to_fit();
        ctx->apps.libs = std::move(loaded_libs);
        
        if(ctx->apps.libs.empty()) {
                if(KC_EXTRA_LOGGING) {
                        ctx->log_fn("No KD apps to launch");
                }
                return KC_FAIL;
        }
        
        /* get tick funcs */
        kci_load_libs(ctx->apps.libs.data(), ctx->apps.libs.size());

        if(KC_EXTRA_LOGGING) {
                ctx->log_fn("Karbon Core application loop");
        }

        ctx->apps.libs[0].start();

        /* loop */
        while(kci_platform_process(&ctx->platform)) {

                //fundamental_tick();
                
                ctx->apps.libs[0].tick();

//                renderer_dx12_render();
        }

        ctx->apps.libs[0].close();

        if(KC_EXTRA_LOGGING) {
                ctx->log_fn("Karbon has finished");
        }

        return KC_OK;
}
