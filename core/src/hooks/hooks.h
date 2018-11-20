#ifndef HOOKS_INCLUDED
#define HOOKS_INCLUDED


int
kdi_ctx_get_vendor_string(
        void *ext,
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


#endif
