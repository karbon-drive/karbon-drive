
#include <karbon/core.h>
#include <karbon/drive.h>
#include "../ctx.h"
#include "../allocators/tagged_allocator.h"
#include "hooks.h"
#include <string.h>
#include "../fundamental.h"

#ifdef _WIN32
#include <Windows.h>
#elif defined(__linux__)
#include <unistd.h>
#include <libgen.h>
#endif


int
kdi_ctx_get_vendor_string(
        void *ctx,
        char **out_str,
        int *out_size)
{
        (void)ctx;
        
        #if defined(__APPLE__)
        const char *vendor_string = "ROA(macOS)";
        #elif defined(_WIN32)
        const char *vendor_string = "ROA(Windows)";
        #elif defined(__linux__)
        const char *vendor_string = "ROA(Linux)";
        #endif
        
        if(out_size) {
                *out_size = strlen(vendor_string) + 1;
        }
        
        if(out_str) {
                memcpy(*out_str, vendor_string, strlen(vendor_string) + 1);
        }
        
        return KC_OK;
}


int
kdi_ctx_get_exe_dir(
        void *ctx,
        char **out_str,
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
                *out_size = strlen(buf_path) + 1;
        }

        if(out_str) {
                memcpy(*out_str, buf_path, strlen(buf_path) + 1);
        }

        return KC_OK;
}


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


int
kdi_window_get(
        void *ctx,
        struct kd_window_desc *desc)
{
        (void)ctx;
        (void)desc;
        
        return 0;
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
