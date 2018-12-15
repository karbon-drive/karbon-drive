#include <karbon/drive.h>
#include "../config.h"


extern "C" {


void *ctx = 0;


/* --------------------------------------------------------------- Context -- */


KD_CTX_GET_VENDOR_STRING_FN kd_ctx_get_vendor_string_fn = 0;


kd_result
kd_ctx_get_vendor_string(
        char *out_buffer,
        int *out_size)
{
        if(KD_PCHECK && !ctx) {
                KD_ASSERT(!"KD_RESULT_CORRUPTED");
                return KD_RESULT_CORRUPTED;
        }
        
        if(KD_PCHECK && (!out_buffer && !out_size)) {
                KD_ASSERT(!"KD_RESULT_INVALID_PARAM");
                return KD_RESULT_INVALID_PARAM;
        }

        if (KD_PCHECK && !kd_ctx_get_vendor_string_fn) {
                KD_ASSERT(!"KD_RESULT_NO_IMPLEMENTATION");
                return KD_RESULT_NO_IMPLEMENTATION;
        }
        
        int res = kd_ctx_get_vendor_string_fn(ctx, out_buffer, out_size);
        
        return res ? KD_RESULT_OK : KD_RESULT_FAIL;
}


KD_CTX_GET_GRAPHICS_API_FN kd_ctx_get_graphics_api_fn = 0;

kd_result
kd_ctx_get_graphics_api(
        kd_graphics_api *out_api,
        int *out_major,
        int *out_minor,
        int *out_patch)
{
        if (KD_PCHECK && !ctx) {
                KD_ASSERT(!"KD_RESULT_CORRUPTED");
                return KD_RESULT_CORRUPTED;
        }

        if (KD_PCHECK && !out_api) {
                KD_ASSERT(!"KD_RESULT_INVALID_PARAM");
                return KD_RESULT_INVALID_PARAM;
        }

        if (KD_PCHECK && !kd_ctx_get_graphics_api_fn) {
                KD_ASSERT(!"KD_RESULT_NO_IMPLEMENTATION");
                return KD_RESULT_NO_IMPLEMENTATION;
        }

        int res = kd_ctx_get_graphics_api_fn(ctx, out_api, out_major, out_minor, out_patch);

        return res ? KD_RESULT_OK : KD_RESULT_FAIL;
}


KD_CTX_GET_EXE_DIR_FN kd_ctx_get_exe_dir_fn = 0;


kd_result
kd_ctx_get_exe_dir(
        char **out_buffer,
        int *out_size)
{
        if(KD_PCHECK && !ctx) {
                KD_ASSERT(!"KD_RESULT_CORRUPTED");
                return KD_RESULT_CORRUPTED;
        }

        if(KD_PCHECK && (!out_buffer && !out_size)) {
                KD_ASSERT(!"KD_RESULT_INVALID_PARAM");
                return KD_RESULT_INVALID_PARAM;
        }

        if (KD_PCHECK && !kd_ctx_get_exe_dir_fn) {
                KD_ASSERT(!"KD_RESULT_NO_IMPLEMENTATION");
                return KD_RESULT_NO_IMPLEMENTATION;
        }

        int res = kd_ctx_get_exe_dir_fn(ctx, out_buffer, out_size);

        return res ? KD_RESULT_OK : KD_RESULT_FAIL;
}


/* ------------------------------------------------------------- Allocator -- */


KD_ALLOC_FN kd_alloc_fn = 0;


kd_result
kd_alloc(
        const struct kd_alloc_desc * desc,
        void **out_addr,
        int *bytes)
{
        if(KD_PCHECK && !ctx) {
                KD_ASSERT(!"KD_RESULT_CORRUPTED");
                return KD_RESULT_CORRUPTED;
        }
        
        if(KD_PCHECK && !desc) {
                KD_ASSERT(!"KD_RESUILT_INVALID_PARAM");
                return KD_RESULT_INVALID_PARAM;
        }
        
        if(KD_PCHECK && (desc->type_id != KD_STRUCT_ALLOC_DESC)) {
                KD_ASSERT(!"KD_RESULT_INVALID_DESC");
                return KD_RESULT_INVALID_DESC;
        }
        
        if(KD_PCHECK && !desc->allocator_desc) {
                KD_ASSERT(!"KD_RESULT_INVALID_DESC");
                return KD_RESULT_INVALID_DESC;
        }

        if (KD_PCHECK && !kd_alloc_fn) {
                KD_ASSERT(!"KD_RESULT_NO_IMPLEMENTATION");
                return KD_RESULT_NO_IMPLEMENTATION;
        }
        
        int res = kd_alloc_fn(ctx, desc, out_addr, bytes);
        
        return res ? KD_RESULT_OK : KD_RESULT_FAIL;
}


/* ---------------------------------------------------------------- Window -- */


KD_WINDOW_GET_FN kd_window_get_fn = 0;


kd_result
kd_window_get(
        struct kd_window_desc *desc)
{
        if(KD_PCHECK && !ctx) {
                KD_ASSERT(!"KD_RESULT_CORRUPTED");
                return KD_RESULT_CORRUPTED;
        }
        
        if(KD_PCHECK && !desc) {
                KD_ASSERT(!"KD_RESUILT_INVALID_PARAM");
                return KD_RESULT_INVALID_PARAM;
        }
        
        if(KD_PCHECK && (desc->type_id != KD_STRUCT_WINDOW_DESC)) {
                KD_ASSERT(!"KD_RESULT_INVALID_DESC");
                return KD_RESULT_INVALID_DESC;
        }

        if (KD_PCHECK && !kd_window_get_fn) {
                KD_ASSERT(!"KD_RESULT_NO_IMPLEMENTATION");
                return KD_RESULT_NO_IMPLEMENTATION;
        }

        int res = kd_window_get_fn(ctx, desc);
        return res ? KD_RESULT_OK : KD_RESULT_FAIL;
}


KD_WINDOW_SET_FN kd_window_set_fn = 0;


kd_result
kd_window_set(
        const struct kd_window_desc *desc)
{
        if(KD_PCHECK && !ctx) {
                KD_ASSERT(!"KD_RESULT_CORRUPTED");
                return KD_RESULT_CORRUPTED;
        }
        
        if(KD_PCHECK && !desc) {
                KD_ASSERT(!"KD_RESUILT_INVALID_PARAM");
                return KD_RESULT_INVALID_PARAM;
        }
        
        if(KD_PCHECK && (desc->type_id != KD_STRUCT_WINDOW_DESC)) {
                KD_ASSERT(!"KD_RESULT_INVALID_DESC");
                return KD_RESULT_INVALID_DESC;
        }

        if (KD_PCHECK && !kd_window_set_fn) {
                KD_ASSERT(!"KD_RESULT_NO_IMPLEMENTATION");
                return KD_RESULT_NO_IMPLEMENTATION;
        }

        int res = kd_window_set_fn(ctx, desc);
        return res ? KD_RESULT_OK : KD_RESULT_FAIL;
}


/* ----------------------------------------------------------------- Input -- */


KD_INPUT_GET_KEYBOARDS_FN kd_input_get_keyboards_fn = 0;


kd_result
kd_input_get_keyboards(
        struct kd_keyboard_desc *out_desc)
{
        if (KD_PCHECK && !ctx) {
                KD_ASSERT(!"KD_RESULT_CORRUPTED");
                return KD_RESULT_CORRUPTED;
        }

        if (KD_PCHECK && !out_desc) {
                KD_ASSERT(!"KD_RESUILT_INVALID_PARAM");
                return KD_RESULT_INVALID_PARAM;
        }

        if (KD_PCHECK && !kd_input_get_keyboards) {
                KD_ASSERT(!"KD_RESULT_NO_IMPLEMENTATION");
                return KD_RESULT_NO_IMPLEMENTATION;
        }

        int res = kd_input_get_keyboards_fn(ctx, out_desc);
        return res ? KD_RESULT_OK : KD_RESULT_FAIL;
}


/* --------------------------------------------------------------- Open GL -- */


KD_GL_MAKE_CURRENT kd_gl_make_current_fn = 0;


kd_result
kd_gl_make_current()
{
        if(KD_PCHECK && !ctx) {
                KD_ASSERT(!"KD_RESULT_CORRUPTED");
                return KD_RESULT_CORRUPTED;
        }
        
        if(KD_PCHECK && !kd_gl_make_current) {
                KD_ASSERT(!"KD_RESULT_NO_IMPLEMENTATION");
                return KD_RESULT_NO_IMPLEMENTATION;
        }
        
        int res = kd_gl_make_current_fn(ctx);
}


/* --------------------------------------------------------------- Logging -- */


KD_LOG_FN kd_log_fn = 0;


kd_result
kd_log(
        kd_log_type type,
        const char *msg)
{
        if (KD_PCHECK && !ctx) {
                KD_ASSERT(!"KD_RESULT_CORRUPTED");
                return KD_RESULT_CORRUPTED;
        }

        if (KD_PCHECK && !kd_log_fn) {
                KD_ASSERT(!"KD_RESULT_NO_IMPLENTATION");
                return KD_RESULT_NO_IMPLEMENTATION;
        }

        int res = kd_log_fn(ctx, type, msg);
        return res ? KD_RESULT_OK : KD_RESULT_FAIL;
}


/* ---------------------------------------------------------------- Loader -- */


int
kd_load(void ** funcs)
{
        if (!funcs) {
                return 0;
        }

        ctx = funcs[KD_PTR_CTX];
        kd_ctx_get_vendor_string_fn = (KD_CTX_GET_VENDOR_STRING_FN)funcs[KD_FUNC_CTX_VENDOR_STRING];
        kd_ctx_get_graphics_api_fn = (KD_CTX_GET_GRAPHICS_API_FN)funcs[KD_FUNC_CTX_GRAPHICS_API];
        kd_ctx_get_exe_dir_fn = (KD_CTX_GET_EXE_DIR_FN)funcs[KD_FUNC_CTX_EXE_DIR];
        kd_alloc_fn = (KD_ALLOC_FN)funcs[KD_FUNC_ALLOC];
        kd_window_get_fn = (KD_WINDOW_GET_FN)funcs[KD_FUNC_WINDOW_GET];
        kd_window_set_fn = (KD_WINDOW_SET_FN)funcs[KD_FUNC_WINDOW_SET];
        kd_gl_make_current_fn = (KD_GL_MAKE_CURRENT)funcs[KD_FUNC_OPENGL_MAKE_CURRENT];
        kd_input_get_keyboards_fn = (KD_INPUT_GET_KEYBOARDS_FN)funcs[KD_FUNC_INPUT_KEYBOARD_GET];
        
        kd_log_fn = (KD_LOG_FN)funcs[KD_FUNC_LOG];

        return 1;
}


} /* extern C */
