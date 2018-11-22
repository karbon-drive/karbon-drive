
#include <karbon/core.h>
#include <karbon/drive.h>
#include "../ctx.h"
#include "../allocators/tagged_allocator.h"
#include "hooks.h"
#include <string.h>
#include "../fundamental.h"

#ifdef _WIN32
#include <Windows.h>
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

        static char buffer_path[4096] = "\0";

        #if defined(_WIN32)
        if (strcmp(buffer_path, "") == 0)
        {
                HMODULE hModule = GetModuleHandle(NULL);
                if (hModule != NULL)
                {
                        GetModuleFileNameA(NULL, buffer_path, KC_ARR_COUNT(buffer_path));

                        size_t path_length = 0;
                        for (size_t i = 0; i < KC_ARR_COUNT(buffer_path); i++)
                        {
                                if (buffer_path[i] == '\0') {
                                        path_length = i;
                                        break;
                                }
                        }

                        size_t last_slash_index = 0;
                        for (size_t i = 0; i < path_length; i++)
                        {
                                size_t r_i = (path_length - 1) - i;
                                if (buffer_path[r_i] == '/' || buffer_path[r_i] == '\\') {
                                        last_slash_index = r_i;
                                        break;
                                }
                        }

                        buffer_path[last_slash_index + 1] = '\0';
                }
        }
        #elif defined(__linux__)
        if (strlen(buffer_path) == 0)
        {
                char buffer[2048] = {0};

                unsigned int count = readlink("/proc/self/exe", buffer, sizeof(buffer));

                if (count != (unsigned)-1)
                {
                        const char *path = dirname(buffer);
                        strcpy(buffer_path, path);
                        strcat(buffer_path, "/");
                }
        }

        return lib_ARR_DATA(buffer_path);
        #elif defined(__APPLE__)
        if (strlen(buffer_path) == 0)
        {
                proc_pidpath(getpid(), buffer_path, sizeof(buffer_path));

                size_t i;
                size_t buffer_length = (size_t)strlen(buffer_path);

                size_t last_slash_index = 0;
                for (i = 0; i < buffer_length; i++) {
                        size_t r_i = (buffer_length - 1) - i;
                        if (buffer_path[r_i] == '/' || buffer_path[r_i] == '\\') {
                                last_slash_index = r_i;
                                break;
                        }
                }

                buffer_path[last_slash_index + 1] = '\0';
        }
        #endif

        if(out_size) {
                *out_size = strlen(buffer_path) + 1;
        }

        if(out_str) {
                memcpy(*out_str, buffer_path, strlen(buffer_path) + 1);
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
