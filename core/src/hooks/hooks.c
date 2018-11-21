
#include <karbon/core.h>
#include <karbon/drive.h>
#include "../ctx.h"
#include "../allocators/tagged_allocator.h"
#include "hooks.h"
#include <string.h>


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

        #ifdef __linux__
        const char *base_dir = "/home/phil/scratch/karbon-test/build/bin/Development/";
        #else
        const char *base_dir = "/Users/PhilCK/Developer/roa/build/bin/Development/";
        #endif

        if(out_size) {
                *out_size = strlen(base_dir) + 1;
        }

        if(out_str) {
                memcpy(*out_str, base_dir, strlen(base_dir) + 1);
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
