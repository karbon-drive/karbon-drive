#ifndef KARBON_DRIVE
#define KARBON_DRIVE


#ifdef __cplusplus
extern "C" {
#endif


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
} kd_struct_id;


typedef enum _kd_bool {
        KD_FALSE = 0,
        KD_TRUE = 1,
} kd_bool;


extern void *ctx;


/* --------------------------------------------------------------- Context -- */


kd_result
kd_ctx_get_vendor_string(
        char **out_buffer,
        int *out_size);


typedef int(*KD_CTX_GET_VENDOR_STRING_FN)(void *,char**, int*);
extern KD_CTX_GET_VENDOR_STRING_FN kd_ctx_get_vendor_string_fn;



/* ------------------------------------------------------------- Transform -- */





/* ------------------------------------------------------------------ Mesh -- */





/* -------------------------------------------------------------- Material -- */





/* ------------------------------------------------------------ Renderable -- */





/* ----------------------------------------------------------------- Chunk -- */


struct kd_mesh_desc {
        int mesh_buffer;
        int mesh_data;
        int mesh_offset;
        int mesh_size;
        
        int index_buffer;
        int index_data;
        int index_offset;
        int index_size;
};


struct kd_shader_desc {
        int shader_buffer;
        int shader_offset;
        int shader_size;
};


struct kd_material_desc {
        int shader_id;
};


struct kd_transform {
        float position[3];
        float scale[3];
        float rotation[4];
};


struct kd_entity_desc {
        int instance_id;
        int material_id;
        int mesh_id;
        
        struct kd_transform transform;
};


struct kd_chunk_desc {
        kd_struct_id type_id;
        void *ext;
        
        void **buffers;
        int *buffers_sizes;
        int buffer_count;
        
        struct kd_mesh_desc *meshes;
        struct kd_shader_desc *shaders;
        
        struct kd_material_desc *material;
};


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
extern KD_ALLOC_FN kd_alloc_fn;


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
extern KD_WINDOW_GET_FN kd_window_get_fn;


typedef int(*KD_WINDOW_SET_FN)(void *, const struct kd_window_desc *desc);
extern KD_WINDOW_SET_FN kd_window_set_fn;


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
#pragma comment(linker, "/export:kd_load=_kd_load")
#else
#define KD_EXPORT
#endif


enum kd_api_hooks {
        KD_PTR_CTX,
        KD_FUNC_CTX_VENDOR_STRING,
        KD_FUNC_ALLOC,
        KD_FUNC_WINDOW_GET,
        KD_FUNC_WINDOW_SET,
        
        KD_FUNC_COUNT
};


int
kd_load(void ** funcs);


/* ----------------------------------------------------------------- Hooks -- */


#define KD_HOOK_LOAD_STR "kd_load"
typedef int(*KD_LOAD_FN)(void**);

#define KD_HOOK_PROJECT_ENTRY_STR "kd_project_entry"
typedef int(*KD_PROJENTRYFN)();

#define KD_HOOK_LOGIC_STR "kd_logic_tick"
typedef int(*KD_LOGICTICKFN)();


#ifdef __cplusplus
} /* extern */
#endif


/* inc guard */
#endif
