#include <karbon/drive.h>
#include "../config.h"


void *ctx = 0;


/* --------------------------------------------------------------- Context -- */


KD_CTX_GET_VENDOR_STRING_FN kd_ctx_get_vendor_string_fn = 0;


kd_result
kd_ctx_get_vendor_string(
        char **out_buffer,
        int *out_size)
{
        if(KD_PCHECK && !ctx) {
                KD_ASSERT(0 && "KD_RESULT_CORRUPTED");
                return KD_RESULT_CORRUPTED;
        }
        
        if(KD_PCHECK && (!out_buffer && !out_size)) {
                KD_ASSERT(0 && "KD_RESULT_INVALID_PARAM");
                return KD_RESULT_INVALID_PARAM;
        }
        
        int res = kd_ctx_get_vendor_string_fn(ctx, out_buffer, out_size);
        
        return res ? KD_RESULT_OK : KD_RESULT_FAIL;
}


KD_CTX_GET_EXE_DIR_FN kd_ctx_get_exe_dir_fn = 0;


kd_result
kd_ctx_get_exe_dir(
        char **out_buffer,
        int *out_size)
{
        if(KD_PCHECK && !ctx) {
                KD_ASSERT(0 && "KD_RESULT_CORRUPTED");
                return KD_RESULT_CORRUPTED;
        }

        if(KD_PCHECK && (!out_buffer && !out_size)) {
                KD_ASSERT(0 && "KD_RESULT_INVALID_PARAM");
                return KD_RESULT_INVALID_PARAM;
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
                KD_ASSERT(0 && "KD_RESULT_CORRUPTED");
                return KD_RESULT_CORRUPTED;
        }
        
        if(KD_PCHECK && !desc) {
                KD_ASSERT(0 && "KD_RESUILT_INVALID_PARAM");
                return KD_RESULT_INVALID_PARAM;
        }
        
        if(KD_PCHECK && (desc->type_id != KD_STRUCT_ALLOC_DESC)) {
                KD_ASSERT(0 && "KD_RESULT_INVALID_DESC");
                return KD_RESULT_INVALID_DESC;
        }
        
        if(KD_PCHECK && !desc->allocator_desc) {
                KD_ASSERT(0 && "KD_RESULT_INVALID_DESC");
                return KD_RESULT_INVALID_DESC;
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
                KD_ASSERT(0 && "KD_RESULT_CORRUPTED");
                return KD_RESULT_CORRUPTED;
        }
        
        if(KD_PCHECK && !desc) {
                KD_ASSERT(0 && "KD_RESUILT_INVALID_PARAM");
                return KD_RESULT_INVALID_PARAM;
        }
        
        if(KD_PCHECK && (desc->type_id != KD_STRUCT_WINDOW_DESC)) {
                KD_ASSERT(0 && "KD_RESULT_INVALID_DESC");
                return KD_RESULT_INVALID_DESC;
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
                KD_ASSERT(0 && "KD_RESULT_CORRUPTED");
                return KD_RESULT_CORRUPTED;
        }
        
        if(KD_PCHECK && !desc) {
                KD_ASSERT(0 && "KD_RESUILT_INVALID_PARAM");
                return KD_RESULT_INVALID_PARAM;
        }
        
        if(KD_PCHECK && (desc->type_id != KD_STRUCT_WINDOW_DESC)) {
                KD_ASSERT(0 && "KD_RESULT_INVALID_DESC");
                return KD_RESULT_INVALID_DESC;
        }

        int res = kd_window_set_fn(ctx, desc);
        return res ? KD_RESULT_OK : KD_RESULT_FAIL;
}


/* ----------------------------------------------------------------- Chunk -- */


KD_CHUNK_ADD_FN kd_chunk_add_fn = 0;


kd_result
kd_chunk_add(
        const struct kd_chunk_desc * desc,
        uint32_t *out_chunk_id)
{
        if (KD_PCHECK && !ctx) {
                KD_ASSERT(0 && "KD_RESULT_CORRUPTED");
                return KD_RESULT_CORRUPTED;
        }

        if (KD_PCHECK && !desc) {
                KD_ASSERT(0 && "KD_RESUILT_INVALID_PARAM");
                return KD_RESULT_INVALID_PARAM;
        }

        if (KD_PCHECK && (desc->type_id != KD_STRUCT_CHUNK_DESC)) {
                KD_ASSERT(0 && "KD_RESULT_INVALID_DESC");
                return KD_RESULT_INVALID_DESC;
        }

        int res = kd_chunk_add_fn(ctx, desc, out_chunk_id);
        return res ? KD_RESULT_OK : KD_RESULT_FAIL;
}


/* --------------------------------------------------------------- Logging -- */


KD_LOG_FN kd_log_fn = 0;


kd_result
kd_log(
        kd_log_type type,
        const char *msg)
{
        int res = kd_log_fn(ctx, type, msg);
        return res ? KD_RESULT_OK : KD_RESULT_FAIL;
}


/* ---------------------------------------------------------------- Loader -- */


int
kd_load(void ** funcs)
{
        ctx = funcs[KD_PTR_CTX];
        kd_ctx_get_vendor_string_fn = funcs[KD_FUNC_CTX_VENDOR_STRING];
        kd_ctx_get_exe_dir_fn = funcs[KD_FUNC_CTX_EXE_DIR];
        kd_window_get_fn = funcs[KD_FUNC_WINDOW_GET];
        kd_window_set_fn = funcs[KD_FUNC_WINDOW_SET];
        kd_alloc_fn = funcs[KD_FUNC_ALLOC];
        kd_chunk_add_fn = funcs[KD_FUNC_CHUNK_ADD];
        kd_log_fn = funcs[KD_FUNC_LOG];

        return 0;
}
