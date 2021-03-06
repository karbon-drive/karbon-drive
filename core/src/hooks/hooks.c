
#include <karbon/core.h>
#include <karbon/drive.h>
#include <app/ctx.h>
#include <allocators/tagged_allocator.h>
#include <thirdparty/glfw/include/GLFW/glfw3.h>
#include "hooks.h"
#include <string.h>
#include <app/fundamental.h>
#include <stdio.h>
#include <stdint.h>


#ifdef _WIN32
#include <Windows.h>
#elif defined(__linux__)
#include <unistd.h>
#include <libgen.h>
#elif defined(__APPLE__)
#include <unistd.h>
#include <libgen.h>
#include <libproc.h>
#endif


/* --------------------------------------------------------------- Context -- */


int
kdi_ctx_get_vendor_string(
        void *ctx,
        char *out_str,
        int *out_size)
{
        (void)ctx;
        
        #if defined(__APPLE__)
        const char *vendor_str = "ROA(macOS)";
        #elif defined(_WIN32)
        const char *vendor_str = "ROA(Windows)";
        #elif defined(__linux__)
        const char *vendor_str = "ROA(Linux)";
        #endif
        
        if(out_size) {
                *out_size = (int)strlen(vendor_str) + 1;
        }
        
        if(out_str) {
                void *dst = (void*)out_str;
                void *src = (void*)vendor_str;
                size_t len = strlen(vendor_str) + 1;
                memcpy(dst, src, len);
        }
        
        return 1;
}


int
kdi_ctx_get_graphics_api(
        void *ctx,
        kd_graphics_api *out_api,
        int *out_major,
        int *out_minor,
        int *out_patch)
{
        (void)ctx;

        if (out_api) {
                *out_api = KD_GRAPHICS_API_OPENGL;
        }

        if (out_major) {
                *out_major = -1;
        }

        if(out_minor) {
                *out_minor = -1;
        }
        
        if(out_patch) {
                *out_patch = -1;
        }

        return 1;
}


int
kdi_ctx_get_exe_dir(
        void *ctx,
        char *out_str,
        int *out_size)
{
        (void)ctx;

        #define KC_PATH_MAX 4096

        static char buf_path[KC_PATH_MAX] = "\0";

        #if defined(_WIN32)
        if (strcmp(buf_path, "") == 0) {
                HMODULE hModule = GetModuleHandle(NULL);
                if(hModule == NULL) {
                        return 0;
                }

                GetModuleFileNameA(NULL, buf_path, KC_ARR_COUNT(buf_path));

                size_t path_length = 0;
                for (size_t i = 0; i < KC_ARR_COUNT(buf_path); i++) {
                        if (buf_path[i] == '\0') {
                                path_length = i;
                                break;
                        }
                }

                size_t last_slash_index = 0;
                for (size_t i = 0; i < path_length; i++) {
                        size_t r_i = (path_length - 1) - i;
                        if (buf_path[r_i] == '/' || buf_path[r_i] == '\\') {
                                last_slash_index = r_i;
                                break;
                        }
                }

                buf_path[last_slash_index + 1] = '\0';
        }
        #elif defined(__linux__)
        if (strlen(buf_path) == 0) {
                char buf[KC_PATH_MAX] = {0};
                unsigned count = readlink("/proc/self/exe", buf, sizeof(buf));

                if (count == (unsigned)-1) {
                        return 0;
                }

                const char *path = dirname(buf);
                strcpy(buf_path, path);
                strcat(buf_path, "/");
        }
        #elif defined(__APPLE__)
        if (strlen(buf_path) == 0) {
                proc_pidpath(getpid(), buf_path, sizeof(buf_path));

                size_t i;
                size_t length = (size_t)strlen(buf_path);

                if(length == 0) {
                        return 0;
                }

                size_t last_slash_index = 0;
                for (i = 0; i < length; i++) {
                        size_t r_i = (length - 1) - i;
                        if (buf_path[r_i] == '/' || buf_path[r_i] == '\\') {
                                last_slash_index = r_i;
                                break;
                        }
                }

                buf_path[last_slash_index + 1] = '\0';
        }
        #endif

        #undef KC_PATH_MAX

        if(out_size) {
                *out_size = (int)strlen(buf_path) + 1;
        }

        if(out_str) {
                memcpy(out_str, buf_path, strlen(buf_path) + 1);
        }

        return 1;
}


int
kdi_ctx_app_index_get(
        void *ctx,
        int *curr_idx,
        int *count)
{
        kc_ctx_t core_ctx = (kc_ctx_t)ctx;

        *curr_idx = core_ctx->apps.curr;
        *count = (int)kc_array_size(core_ctx->apps.libs);
        
        return 1;
}


int
kdi_ctx_app_index_set(
        void *ctx,
        int next_idx)
{
        kc_ctx_t core_ctx = (kc_ctx_t)ctx;

        /*if((int)core_ctx->apps.libs.size() < next_idx) {
                return 0;
        }*/

        core_ctx->apps.next = next_idx;

        return 1;
}


int
kdi_ctx_close(
        void *ctx)
{
        kc_ctx_t core_ctx = (kc_ctx_t)ctx;

        glfwSetWindowShouldClose(
                core_ctx->win,
                1);

        return 1;
}


/* ---------------------------------------------------------------- Events -- */


int
kdi_events_get(
        void *ctx,
        uint64_t *evts)
{
        kc_ctx_t core_ctx = (kc_ctx_t)ctx;
        *evts = core_ctx->frame_events;

        return 1;
}


/* ------------------------------------------------------------- Allocator -- */


int
kdi_alloc_tagged(
        void *ctx,
        const struct kd_alloc_desc *desc,
        void **out_addr,
        int *out_bytes)
{
        (void)desc;
        kc_ctx_t core_ctx = (kc_ctx_t)ctx;
        
        struct kci_tagged_allocator *alloc_ctx = &core_ctx->allocator_tagged;
        struct kci_tagged_memory mem = kci_tag_alloc_get(alloc_ctx, 1);
        
        *out_addr = mem.memory;
        *out_bytes = mem.bytes;
        
        return 1;
}


/* ---------------------------------------------------------------- Window -- */


int
kdi_window_get(
        void *ctx,
        struct kd_window_desc *desc)
{
        kc_ctx_t core_ctx = (kc_ctx_t)ctx;

        glfwGetWindowSize(
                core_ctx->win,
                &desc->width,
                &desc->height);

        return 1;
}

int
kdi_window_set(
        void *ctx,
        const struct kd_window_desc *desc)
{
        (void)ctx;
        (void)desc;
        
        return 0;
}


/* ----------------------------------------------------------------- Input -- */


int
kdi_input_get_keyboards(
        void *ctx,
        struct kd_keyboard_desc *out_desc)
{
        kc_ctx_t core_ctx = (kc_ctx_t)ctx;
 
        static uint8_t *kbs[1];
        kbs[0] = core_ctx->keys;

        out_desc->kb_count = 1;
        out_desc->kb_state = kbs;

        return 1;
}


int
kdi_input_get_mice(
        void *ctx,
        struct kd_mouse_desc *out_desc)
{
        kc_ctx_t core_ctx = (kc_ctx_t)ctx;
        
        static struct ms_state mi[1];
        mi[0] = core_ctx->mouse;
        
        out_desc->ms_count = 1;
        out_desc->ms_state = mi;
        
        return 1;
}


/* --------------------------------------------------------------- Open GL -- */


int
kdi_gl_make_current(
        void *ctx)
{
        kc_ctx_t core_ctx = (kc_ctx_t)ctx;

        glfwMakeContextCurrent(core_ctx->win);
        return 1;
}


/* --------------------------------------------------------------- Logging -- */


int
kdi_log(
        void *ctx,
        kd_log_type type,
        const char *msg)
{
        (void)ctx;
        (void)type;
        (void)msg;

        static const char *types[4] = {"INFO", "WARNING", "ERROR", "FATAL"}; 

        printf("[%s] %s\n", types[(int)type], msg);

        return 1;
}

