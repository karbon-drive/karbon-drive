#ifndef KARBON_DRIVE
#define KARBON_DRIVE


/* ------------------------------------------------- Types and Identifiers -- */


#ifndef __cplusplus
        #ifdef _MSC_VER
                typedef signed   __int8  int8_t;
                typedef unsigned __int8  uint8_t;
                typedef signed   __int16 int16_t;
                typedef unsigned __int16 uint16_t;
                typedef signed   __int32 int32_t;
                typedef unsigned __int32 uint32_t;
                typedef signed   __int64 int64_t;
                typedef unsigned __int64 uint64_t;
                #include <stddef.h>
        #else
                #include <stdint.h>
                #include <stddef.h>
        #endif
#else
        #include <cstdint>
        #include <cstddef>
#endif


#ifdef __cplusplus
extern "C" {
#endif


typedef enum _kd_result {
  KD_RESULT_OK,
        KD_RESULT_FAIL,
        KD_RESULT_CORRUPTED,
        KD_RESULT_INVALID_PARAM,
        KD_RESULT_INVALID_DESC,
} kd_result;


typedef enum _kd_struct_id {
        KD_STRUCT_WINDOW_DESC,
        KD_STRUCT_ALLOCATOR_FRAME_DESC,
        KD_STRUCT_ALLOC_DESC,
        KD_STRUCT_CHUNK_DESC,
} kd_struct_id;


typedef enum _kd_bool {
        KD_FALSE = 0,
        KD_TRUE = 1,
} kd_bool;


/* --------------------------------------------------------------- Context -- */


kd_result
kd_ctx_get_vendor_string(
        char **out_buffer,
        int *out_size);


typedef enum _kd_graphics_api {
        KD_GRAPHICS_API_OPENGL,
        KD_GRAPHICS_API_DIRECT_X,
        KD_GRAPHICS_API_METAL,
        KD_GRAPHICS_API_VULKAN,
} kd_graphics_api;


kd_result
kd_ctx_get_graphics_api(
        kd_graphics_api *out_api,               /* required */
        int *out_major,                         /* optional */
        int *out_minor,                         /* optional */
        int *out_patch);                        /* optional */


kd_result
kd_ctx_get_exe_dir(
        char **out_buffer,
        int *out_size);


typedef int(*KD_CTX_GET_VENDOR_STRING_FN)(void *,char**, int*);
typedef int(*KD_CTX_GET_GRAPHICS_API_FN)(void*, kd_graphics_api*,int*,int*,int*);
typedef int(*KD_CTX_GET_EXE_DIR_FN)(void *, char**, int*);


/* ------------------------------------------------------------- Allocator -- */


struct kd_allocator_frame_desc {
        kd_struct_id type_id; /* KD_STRUCT_ALLOCATOR_FRAME_DESC */
        void * ext;
};


struct kd_alloc_desc {
        kd_struct_id type_id; /* KD_STRUCT_ALLOC_DESC */
        void * ext;
        
        void * allocator_desc; /* one of the above allocators */
};


kd_result
kd_alloc(
        const struct kd_alloc_desc * desc,
        void **out_addr,
        int *bytes);


typedef int(*KD_ALLOC_FN)(void *,const struct kd_alloc_desc *desc, void ** out_addr, int *bytes);


/* ---------------------------------------------------------------- Window -- */


struct kd_window_desc {
        kd_struct_id type_id; /* KD_STRUCT_WINDOW_DESC */
        void * ext;
        
        int width;
        int height;
        kd_bool fullscreen;
};


kd_result
kd_window_get(
        struct kd_window_desc *desc);


kd_result
kd_window_set(
        const struct kd_window_desc *desc);


typedef int(*KD_WINDOW_GET_FN)(void *,struct kd_window_desc *desc);
typedef int(*KD_WINDOW_SET_FN)(void *, const struct kd_window_desc *desc);


/* --------------------------------------------------------------- Logging -- */


typedef enum _kd_log_type {
        KD_LOG_INFO = 0,
        KD_LOG_WARNING = 1,
        KD_LOG_ERROR = 2,
        KD_LOG_FATAL = 3
} kd_log_type;


kd_result
kd_log(
    kd_log_type type,
    const char *msg);


typedef int(*KD_LOG_FN)(void *, kd_log_type, const char *);


/* -------------------------------------------------------------- Platform -- */


#if defined(_WIN32)
#define KD_WINDOWS 1
#define KD_MACOS 0
#define KD_LINUX 0
#define KD_NIX 0
#define KD_UNKNOWN 0
#elif defined(__APPLE__)
#define KD_WINDOWS 0
#define KD_MACOS 1
#define KD_LINUX 0
#define KD_NIX 1
#define KD_UNKNOWN 0
#elif defined(__linux__)
#define KD_WINDOWS 0
#define KD_MACOS 0
#define KD_LINUX 1
#define KD_NIX 1
#define KD_UNKNOWN 0
#else
#define KD_WINDOWS 0
#define KD_MACOS 0
#define KD_LINUX 0
#define KD_NIX 0
#define KD_UNKNOWN 1
#endif


/* ---------------------------------------------------------------- Loader -- */


#ifdef _WIN32
#define KD_EXPORT __declspec(dllexport)
#define KD_API extern "C"
//#pragma comment(linker, "/export:kd_load=_kd_load")
#else
#define KD_API extern "C"
#define KD_EXPORT
#endif


enum kd_api_hooks {
        KD_PTR_CTX,
        KD_FUNC_CTX_VENDOR_STRING,
        KD_FUNC_CTX_GRAPHICS_API,
        KD_FUNC_CTX_EXE_DIR,
        KD_FUNC_ALLOC,
        KD_FUNC_WINDOW_GET,
        KD_FUNC_WINDOW_SET,
        KD_FUNC_LOG,
        KD_FUNC_COUNT
};


KD_EXPORT int
kd_load(void ** funcs);


/* ----------------------------------------------------------------- Hooks -- */


#define KD_HOOK_LOAD_STR "kd_load"
typedef int(*KD_LOAD_FN)(void**);

#define KD_HOOK_PROJECT_ENTRY_STR "kd_project_entry"
typedef void(*KD_PROJENTRYFN)();

#define KD_HOOK_SETUP_STR "kd_setup"
typedef void(*KD_SETUPFN)();

#define KD_HOOK_SHUTDOWN_STR "kd_shutdown"
typedef void(*KD_SHUTDOWNFN)();

#define KD_HOOK_EARLY_THINK_STR "kd_early_think"
typedef void(*KD_EARLYTHINKFN)();

#define KD_HOOK_THINK_STR "kd_think"
typedef void(*KD_THINKFN)();

#define KD_HOOK_LATE_THINK_STR "kd_late_think"
typedef void(*KD_LATETHINKFN)();


#ifdef __cplusplus
} /* extern */
#endif


/* inc guard */
#endif
