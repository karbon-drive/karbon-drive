#ifndef KARBON_CORE
#define KARBON_CORE


#include <karbon/drive.h>


#ifdef __cplusplus
extern "C" {
#endif


/* ------------------------------------------------- Types and Identifiers -- */


typedef enum kc_result {
        KC_OK,
        KC_FAIL,
        KC_INVALID_CTX,
        KC_INVALID_PARAMS,
        KC_INVALID_DESC,
} kc_result;


typedef enum kc_struct_id {
        KC_STRUCT_CTX_DESC,
        KC_STRUCT_APPLICATION_DESC,
} kc_struct_id;


/* --------------------------------------------------------------- Context -- */


typedef struct kc_ctx * kc_ctx_t;
typedef void(*KC_LOG_FN)(const char*);


struct kc_ctx_desc {
        kc_struct_id type_id;
        void * ext;
        
        KC_LOG_FN log_fn;
        
        void * user_data;
};


kc_result
kc_ctx_create(
        struct kc_ctx_desc * desc,
        kc_ctx_t *out_ctx);


kc_result
kc_ctx_user_data(
        kc_ctx_t ctx,
        void **out_user_data);


/* ----------------------------------------------------------- Application -- */


struct kc_application_desc {
        kc_struct_id type_id;
        void * ext;
        
        const char * load_directory;
};


kc_result
kc_application_start(
        kc_ctx_t ctx,
        const struct kc_application_desc * desc);


#ifdef __cplusplus
} /* extern */
#endif


/* inc guard */
#endif
