#ifndef HOOKS_INCLUDED
#define HOOKS_INCLUDED


#include <stdint.h>
#include <karbon/drive.h>


/* --------------------------------------------------------------- Context -- */


int
kdi_ctx_get_vendor_string(
        void *ctx,
        char *str,
        int *len);


int
kdi_ctx_get_graphics_api(
        void *ctx,
        kd_graphics_api *out_api,
        int *out_major,
        int *out_minor,
        int *out_patch);


int
kdi_ctx_get_exe_dir(
        void *ctx,
        char *str,
        int *len);


int
kdi_ctx_app_index_get(
        void *ctx,
        int *curr_idx,
        int *count);


int
kdi_ctx_app_index_set(
        void *ctx,
        int next_idx);


int
kdi_ctx_close(
        void *ctx);


/* ---------------------------------------------------------------- Events -- */


int
kdi_events_get(
        void *ctx,
        uint64_t *evts);


/* ------------------------------------------------------------- Allocator -- */


int
kdi_alloc_tagged(
        void *ctx,
        const struct kd_alloc_desc *desc,
        void **out_addr,
        int *out_bytes);


/* ---------------------------------------------------------------- Window -- */


int
kdi_window_get(
        void *ctx,
        struct kd_window_desc *desc);

int
kdi_window_set(
        void *ctx,
        const struct kd_window_desc *desc);


/* ----------------------------------------------------------------- Input -- */


int
kdi_input_get_keyboards(
        void *ctx,
        struct kd_keyboard_desc *out_desc);


int
kdi_input_get_mice(
        void *ctx,
        struct kd_mouse_desc *out_desc);


/* --------------------------------------------------------------- Open GL -- */


int
kdi_gl_make_current(
        void *ctx);


/* --------------------------------------------------------------- Logging -- */


int
kdi_log(
        void *ctx,
        kd_log_type type,
        const char *msg);


/* -------------------------------------------------------------- Platform -- */


#endif
