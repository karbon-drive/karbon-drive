#ifndef KARBON_DRIVE
#define KARBON_DRIVE


#ifdef _WIN32
#define KD_EXPORT __declspec(dllexport)
#define KD_CALL __cdecl 
#ifdef __cplusplus
#define KD_API extern "C"
#else
#define KD_API
#endif
#else
#define KD_API extern "C"
#define KD_EXPORT
#define KD_CALL
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


#ifdef __cplusplus
extern "C" {
#endif


typedef enum _kd_result {
        KD_RESULT_OK,
        KD_RESULT_FAIL,
        KD_RESULT_CORRUPTED,
        KD_RESULT_NO_IMPLENTATION,
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
        char *out_buffer,
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


typedef int(*KD_CTX_GET_VENDOR_STRING_FN)(void *,char*, int*);
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


/* ----------------------------------------------------------------- Input -- */


typedef enum _kd_key_state {
        KD_KEY_UP = 0,
        KD_KEY_UP_EVENT = 1,
        KD_KEY_DOWN = 2,
        KD_KEY_DOWN_EVENT = 3,
} kd_key_state;


typedef enum _kd_kb_key {
        /* num row */
        KD_KB_1, KD_KB_2, KD_KB_3, KD_KB_4, KD_KB_5, KD_KB_6, KD_KB_7, KD_KB_8,
        KD_KB_9, KD_KB_0, KD_KB_PLUS, KD_KB_MINUS, KD_KB_BACKSPACE, KD_KB_TILDA,

        /* alpha */
        KD_KB_A, KD_KB_B, KD_KB_C, KD_KB_D, KD_KB_E, KD_KB_F, KD_KB_G, KD_KB_H,
        KD_KB_I, KD_KB_J, KD_KB_K, KD_KB_L, KD_KB_M, KD_KB_N, KD_KB_O, KD_KB_P,
        KD_KB_Q, KD_KB_R, KD_KB_S, KD_KB_T, KD_KB_U, KD_KB_V, KD_KB_W, KD_KB_X,
        KD_KB_Y, KD_KB_Z,

        /* func row */
        KD_KB_F1, KD_KB_F2, KD_KB_F3, KD_KB_F4, KD_KB_F5, KD_KB_F6, KD_KB_F7,
        KD_KB_F8, KD_KB_F9, KD_KB_F10, KD_KB_F11, KD_KB_F12, KD_KB_ESC,

        /* punct and mod keys */
        KD_KB_TAB, KD_KB_CAPS, KD_KB_LSHIFT, KD_KB_RSHIFT, KD_KB_LCTRL,
        KD_KB_RCTRL, KD_KB_SPACE, KD_KB_LALT, KD_KB_RALT, KD_KB_PERIOD,
        KD_KB_COMMA,

        /* dir */
        KD_KB_UP, KD_KB_DOWN, KD_KB_LEFT, KD_KB_RIGHT,

        /* key count */
        KD_KB_COUNT
};


struct kd_keyboard_desc {
        uint8_t **kb_state;
        int kb_count;
};


kd_result
kd_input_get_keyboards(
        struct kd_keyboard_desc *out_desc);


typedef int(*KD_INPUT_GET_KEYBOARDS_FN)(void*,struct kd_keyboard_desc*desc);


/* --------------------------------------------------------------- Open GL -- */


kd_result
kd_gl_make_current();


typedef int(*KD_GL_MAKE_CURRENT)(void *);


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


KD_API KD_EXPORT int KD_CALL
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
