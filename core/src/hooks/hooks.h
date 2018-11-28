#ifndef HOOKS_INCLUDED
#define HOOKS_INCLUDED


int
kdi_ctx_get_vendor_string(
        void *ctx,
        char **str,
        int *len);


int
kdi_ctx_get_exe_dir(
        void *ctx,
        char **str,
        int *len);

int
kdi_alloc_tagged(
        void *ctx,
        const struct kd_alloc_desc *desc,
        void **out_addr,
        int *out_bytes);


int
kdi_window_get(
        void *ctx,
        struct kd_window_desc *desc);

int
kdi_window_set(
        void *ctx,
        const struct kd_window_desc *desc);

int
kdi_chunk_add(
        void *ctx,
        const struct kd_chunk_desc *desc);


#endif
