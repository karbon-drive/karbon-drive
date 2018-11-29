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


//extern void *ctx; /* remove */


/* --------------------------------------------------------------- Context -- */


kd_result
kd_ctx_get_vendor_string(
        char **out_buffer,
        int *out_size);


kd_result
kd_ctx_get_exe_dir(
        char **out_buffer,
        int *out_size);


typedef int(*KD_CTX_GET_VENDOR_STRING_FN)(void *,char**, int*);
//extern KD_CTX_GET_VENDOR_STRING_FN kd_ctx_get_vendor_string_fn;

typedef int(*KD_CTX_GET_EXE_DIR_FN)(void *, char**, int*);
//extern KD_CTX_GET_EXE_DIR_FN kd_ctx_get_exe_dir_fn;


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
//extern KD_ALLOC_FN kd_alloc_fn;


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
//extern KD_WINDOW_GET_FN kd_window_get_fn;


typedef int(*KD_WINDOW_SET_FN)(void *, const struct kd_window_desc *desc);
//extern KD_WINDOW_SET_FN kd_window_set_fn;


/* ----------------------------------------------------------------- Chunk -- */


struct kd_buffer_data {
        int buffer;
        int buffer_offset;
        int buffer_size;
};


struct kd_mesh_data {
        struct kd_buffer_data geometry;
        int geometry_type;
        
        struct kd_buffer_data index;
        int index_type;
};


struct kd_chunk_desc {
        kd_struct_id type_id; /* KD_STRUCT_CHUNK_DESC */
        void * ext;
        
        struct kd_mesh_data *mesh_data;
        int mesh_count;

        uint8_t **buffers;
        int buffer_count;
};


kd_result
kd_chunk_add(
        const struct kd_chunk_desc * desc,
        uint32_t *out_chunk_id);


typedef int(*KD_CHUNK_ADD_FN)(void *, const struct kd_chunk_desc *desc, uint32_t *out_chunk_id);
//extern KD_CHUNK_ADD_FN kd_chunk_add_fn;


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
        KD_FUNC_CTX_EXE_DIR,
        KD_FUNC_ALLOC,
        KD_FUNC_WINDOW_GET,
        KD_FUNC_WINDOW_SET,
        KD_FUNC_CHUNK_ADD,
        
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
