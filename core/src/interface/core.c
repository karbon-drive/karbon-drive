#include <karbon/core.h>
#include <app/fundamental.h>
#include <stdlib.h>
#include <string.h>
#include <app/config.h>
#include <dirent.h>
#include <thirdparty/glfw/include/GLFW/glfw3.h>
#include <allocators/tagged_allocator.h>
#include <stdio.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <dlfcn.h>
#endif

#include <hooks/hooks.h>
#include <app/ctx.h>
#include <allocators/tagged_allocator.h>


/* ------------------------------------------------------------------ GLFW -- */


static void
glfw_err_cb(
        int error,
        const char* description)
{
        fprintf(stderr, "Error: %s\n", description);
}


static kd_kb_key*
glfw_kb_lookup() {

        static kd_kb_key *keys = 0;

        if (keys) {
                return keys;
        }

        static kd_kb_key key_map[GLFW_KEY_LAST + 1] = {0};
        keys = &key_map[0];

        keys[GLFW_KEY_SPACE]          = KD_KB_SPACE;
        keys[GLFW_KEY_APOSTROPHE]     = KD_KB_ANY;
        keys[GLFW_KEY_COMMA]          = KD_KB_COMMA;
        keys[GLFW_KEY_MINUS]          = KD_KB_MINUS;
        keys[GLFW_KEY_PERIOD]         = KD_KB_PERIOD;
        keys[GLFW_KEY_SLASH]          = KD_KB_ANY;
        keys[GLFW_KEY_0]              = KD_KB_0;
        keys[GLFW_KEY_1]              = KD_KB_1;
        keys[GLFW_KEY_2]              = KD_KB_2;
        keys[GLFW_KEY_3]              = KD_KB_3;
        keys[GLFW_KEY_4]              = KD_KB_4;
        keys[GLFW_KEY_5]              = KD_KB_5;
        keys[GLFW_KEY_6]              = KD_KB_6;
        keys[GLFW_KEY_7]              = KD_KB_7;
        keys[GLFW_KEY_8]              = KD_KB_8;
        keys[GLFW_KEY_9]              = KD_KB_9;
        keys[GLFW_KEY_SEMICOLON]      = KD_KB_ANY;
        keys[GLFW_KEY_EQUAL]          = KD_KB_ANY;
        keys[GLFW_KEY_A]              = KD_KB_A;
        keys[GLFW_KEY_B]              = KD_KB_B;
        keys[GLFW_KEY_C]              = KD_KB_C;
        keys[GLFW_KEY_D]              = KD_KB_D;
        keys[GLFW_KEY_E]              = KD_KB_E;
        keys[GLFW_KEY_F]              = KD_KB_F;
        keys[GLFW_KEY_G]              = KD_KB_G;
        keys[GLFW_KEY_H]              = KD_KB_H;
        keys[GLFW_KEY_I]              = KD_KB_I;
        keys[GLFW_KEY_J]              = KD_KB_J;
        keys[GLFW_KEY_K]              = KD_KB_K;
        keys[GLFW_KEY_L]              = KD_KB_L;
        keys[GLFW_KEY_M]              = KD_KB_M;
        keys[GLFW_KEY_N]              = KD_KB_N;
        keys[GLFW_KEY_O]              = KD_KB_O;
        keys[GLFW_KEY_P]              = KD_KB_P;
        keys[GLFW_KEY_Q]              = KD_KB_Q;
        keys[GLFW_KEY_R]              = KD_KB_R;
        keys[GLFW_KEY_S]              = KD_KB_S;
        keys[GLFW_KEY_T]              = KD_KB_T;
        keys[GLFW_KEY_U]              = KD_KB_U;
        keys[GLFW_KEY_V]              = KD_KB_V;
        keys[GLFW_KEY_W]              = KD_KB_W;
        keys[GLFW_KEY_X]              = KD_KB_X;
        keys[GLFW_KEY_Y]              = KD_KB_Y;
        keys[GLFW_KEY_Z]              = KD_KB_Z;
        keys[GLFW_KEY_LEFT_BRACKET]   = KD_KB_ANY;
        keys[GLFW_KEY_BACKSLASH]      = KD_KB_ANY;
        keys[GLFW_KEY_RIGHT_BRACKET]  = KD_KB_ANY;
        keys[GLFW_KEY_GRAVE_ACCENT]   = KD_KB_ANY;
        keys[GLFW_KEY_WORLD_1]        = KD_KB_ANY;
        keys[GLFW_KEY_WORLD_2]        = KD_KB_ANY;

        keys[GLFW_KEY_ESCAPE]         = KD_KB_ESC;
        keys[GLFW_KEY_ENTER]          = KD_KB_ANY;
        keys[GLFW_KEY_TAB]            = KD_KB_TAB;
        keys[GLFW_KEY_BACKSPACE]      = KD_KB_ANY;
        keys[GLFW_KEY_INSERT]         = KD_KB_ANY;
        keys[GLFW_KEY_DELETE]         = KD_KB_ANY;
        keys[GLFW_KEY_RIGHT]          = KD_KB_RIGHT;
        keys[GLFW_KEY_LEFT]           = KD_KB_LEFT;
        keys[GLFW_KEY_DOWN]           = KD_KB_DOWN;
        keys[GLFW_KEY_UP]             = KD_KB_UP;
        keys[GLFW_KEY_PAGE_UP]        = KD_KB_ANY;
        keys[GLFW_KEY_PAGE_DOWN]      = KD_KB_ANY;
        keys[GLFW_KEY_HOME]           = KD_KB_ANY;
        keys[GLFW_KEY_END]            = KD_KB_ANY;
        keys[GLFW_KEY_CAPS_LOCK]      = KD_KB_ANY;
        keys[GLFW_KEY_SCROLL_LOCK]    = KD_KB_ANY;
        keys[GLFW_KEY_NUM_LOCK]       = KD_KB_ANY;
        keys[GLFW_KEY_PRINT_SCREEN]   = KD_KB_ANY;
        keys[GLFW_KEY_PAUSE]          = KD_KB_ANY;
        keys[GLFW_KEY_F1]             = KD_KB_F1;
        keys[GLFW_KEY_F2]             = KD_KB_F2;
        keys[GLFW_KEY_F3]             = KD_KB_F3;
        keys[GLFW_KEY_F4]             = KD_KB_F4;
        keys[GLFW_KEY_F5]             = KD_KB_F5;
        keys[GLFW_KEY_F6]             = KD_KB_F6;
        keys[GLFW_KEY_F7]             = KD_KB_F7;
        keys[GLFW_KEY_F8]             = KD_KB_F8;
        keys[GLFW_KEY_F9]             = KD_KB_F9;
        keys[GLFW_KEY_F10]            = KD_KB_F10;
        keys[GLFW_KEY_F11]            = KD_KB_F11;
        keys[GLFW_KEY_F12]            = KD_KB_F12;
        keys[GLFW_KEY_F13]            = KD_KB_ANY;
        keys[GLFW_KEY_F14]            = KD_KB_ANY;
        keys[GLFW_KEY_F15]            = KD_KB_ANY;
        keys[GLFW_KEY_F16]            = KD_KB_ANY;
        keys[GLFW_KEY_F17]            = KD_KB_ANY;
        keys[GLFW_KEY_F18]            = KD_KB_ANY;
        keys[GLFW_KEY_F19]            = KD_KB_ANY;
        keys[GLFW_KEY_F20]            = KD_KB_ANY;
        keys[GLFW_KEY_F21]            = KD_KB_ANY;
        keys[GLFW_KEY_F22]            = KD_KB_ANY;
        keys[GLFW_KEY_F23]            = KD_KB_ANY;
        keys[GLFW_KEY_F24]            = KD_KB_ANY;
        keys[GLFW_KEY_F25]            = KD_KB_ANY;
        keys[GLFW_KEY_KP_0]           = KD_KB_ANY;
        keys[GLFW_KEY_KP_1]           = KD_KB_ANY;
        keys[GLFW_KEY_KP_2]           = KD_KB_ANY;
        keys[GLFW_KEY_KP_3]           = KD_KB_ANY;
        keys[GLFW_KEY_KP_4]           = KD_KB_ANY;
        keys[GLFW_KEY_KP_5]           = KD_KB_ANY;
        keys[GLFW_KEY_KP_6]           = KD_KB_ANY;
        keys[GLFW_KEY_KP_7]           = KD_KB_ANY;
        keys[GLFW_KEY_KP_8]           = KD_KB_ANY;
        keys[GLFW_KEY_KP_9]           = KD_KB_ANY;
        keys[GLFW_KEY_KP_DECIMAL]     = KD_KB_ANY;
        keys[GLFW_KEY_KP_DIVIDE]      = KD_KB_ANY;
        keys[GLFW_KEY_KP_MULTIPLY]    = KD_KB_ANY;
        keys[GLFW_KEY_KP_SUBTRACT]    = KD_KB_ANY;
        keys[GLFW_KEY_KP_ADD]         = KD_KB_ANY;
        keys[GLFW_KEY_KP_ENTER]       = KD_KB_ANY;
        keys[GLFW_KEY_KP_EQUAL]       = KD_KB_ANY;
        keys[GLFW_KEY_LEFT_SHIFT]     = KD_KB_ANY;
        keys[GLFW_KEY_LEFT_CONTROL]   = KD_KB_ANY;
        keys[GLFW_KEY_LEFT_ALT]       = KD_KB_ANY;
        keys[GLFW_KEY_LEFT_SUPER]     = KD_KB_ANY;
        keys[GLFW_KEY_RIGHT_SHIFT]    = KD_KB_ANY;
        keys[GLFW_KEY_RIGHT_CONTROL]  = KD_KB_ANY;
        keys[GLFW_KEY_RIGHT_ALT]      = KD_KB_ANY;
        keys[GLFW_KEY_RIGHT_SUPER]    = KD_KB_ANY;
        keys[GLFW_KEY_MENU]           = KD_KB_ANY;

        return keys;
}

static void
glfw_key_cb(
        GLFWwindow *win,
        int glfw_key,
        int scancode,
        int glfw_action,
        int mods)
{
        kc_ctx_t ctx = glfwGetWindowUserPointer(win);
        assert(ctx && "GLFW - User Ptr not set");

        kd_kb_key key = glfw_kb_lookup()[glfw_key];

        ctx->keys[key] = 0;

        if (glfw_action == GLFW_PRESS) {
                ctx->keys[key] |= KD_KEY_DOWN_EVENT;
                ctx->keys[key] |= KD_KEY_DOWN;

                ctx->keys[KD_KB_ANY] |= KD_KEY_DOWN_EVENT;
                ctx->keys[KD_KB_ANY] |= KD_KEY_DOWN;

                ctx->frame_events |= KD_EVENT_INPUT_KB;
        }
        else if(glfw_action == GLFW_RELEASE) {
                ctx->keys[key] |= KD_KEY_UP_EVENT;
                ctx->keys[key] |= KD_KEY_UP;

                ctx->keys[KD_KB_ANY] |= KD_KEY_UP_EVENT;
                ctx->keys[KD_KB_ANY] |= KD_KEY_UP;

                ctx->frame_events |= KD_EVENT_INPUT_KB;
        }
}


static kd_ms_key*
glfw_ms_lookup() {

        static kd_ms_key *keys = 0;

        if (keys) {
                return keys;
        }

        static kd_ms_key key_map[GLFW_MOUSE_BUTTON_LAST + 1] = {0};
        keys = &key_map[0];

        int i;
        for(i = 0; i < GLFW_MOUSE_BUTTON_LAST; ++i) {
                keys[i] = KD_MS_ANY;
        }

        keys[GLFW_MOUSE_BUTTON_1] = KD_MS_LEFT;
        keys[GLFW_MOUSE_BUTTON_3] = KD_MS_MIDDLE;
        keys[GLFW_MOUSE_BUTTON_2] = KD_MS_RIGHT;

        return keys;
}


static void
glfw_ms_key_cb(
        GLFWwindow *win,
        int key_code,
        int action,
        int mods)
{
        kc_ctx_t ctx = glfwGetWindowUserPointer(win);

        kd_ms_key *keys = glfw_ms_lookup();
        kd_ms_key key = keys[key_code];

        ctx->mouse.keys[key] = 0;

        if (action == GLFW_PRESS) {
                ctx->mouse.keys[key] |= KD_KEY_DOWN_EVENT;
                ctx->mouse.keys[key] |= KD_KEY_DOWN;

                ctx->mouse.keys[KD_MS_ANY] |= KD_KEY_DOWN_EVENT;
                ctx->mouse.keys[KD_MS_ANY] |= KD_KEY_DOWN;

                ctx->frame_events |= KD_EVENT_INPUT_MS;
        }
        else if(action == GLFW_RELEASE) {
                ctx->mouse.keys[key] |= KD_KEY_UP_EVENT;
                ctx->mouse.keys[key] |= KD_KEY_UP;

                ctx->mouse.keys[KD_MS_ANY] |= KD_KEY_UP_EVENT;
                ctx->mouse.keys[KD_MS_ANY] |= KD_KEY_UP;

                ctx->frame_events |= KD_EVENT_INPUT_MS;
        }
}


static void
glfw_mouse_move_cb(
        GLFWwindow *win,
        double x,
        double y)
{
        kc_ctx_t ctx = glfwGetWindowUserPointer(win);

        ctx->mouse.dx = (float)x - ctx->mouse.x;
        ctx->mouse.dy = (float)y - ctx->mouse.y;
        ctx->mouse.x = (float)x;
        ctx->mouse.y = (float)y;

        ctx->frame_events |= KD_EVENT_INPUT_MS;
}


static void
glfw_window_resize_cb(
        GLFWwindow *win,
        int x,
        int y)
{
        (void)x;
        (void)y;

        kc_ctx_t ctx = glfwGetWindowUserPointer(win);
        assert(ctx && "GLFW - User Ptr not set");

        ctx->frame_events |= KD_EVENT_VIEWPORT_RESIZE;
}


/* --------------------------------------------------------------- Helpers -- */


void
kci_alloc_housekeeping(struct kc_ctx *ctx, void **addr, int *bytes) {
        struct kd_allocator_frame_desc allocator_desc;
        allocator_desc.type_id = KD_STRUCT_ALLOCATOR_FRAME_DESC;
        allocator_desc.ext = 0;

        struct kd_alloc_desc alloc_desc;
        alloc_desc.type_id = KD_STRUCT_ALLOC_DESC;
        alloc_desc.ext = 0;
        alloc_desc.allocator_desc = &allocator_desc;

        kdi_alloc_tagged(ctx, &alloc_desc, addr, bytes);
}


void
kci_log_stub(const char *msg) {
        (void)msg;
}


/* --------------------------------------------------------------- Context -- */


kc_result
kc_ctx_create(
        struct kc_ctx_desc * desc,
        kc_ctx_t *out_ctx)
{
        /* glfw */
        if (!glfwInit()) {
                return KC_FAIL;
        }
        glfwSetErrorCallback(glfw_err_cb);

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

        #ifndef __APPLE__
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        #else
        glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint (GLFW_COCOA_RETINA_FRAMEBUFFER, 0);
        #endif

        GLFWwindow *win = glfwCreateWindow(640, 480, "Karbon Drive", NULL, NULL);

        glfwMakeContextCurrent(win);
        glfwSetKeyCallback(win, glfw_key_cb);
        glfwSetWindowSizeCallback(win, glfw_window_resize_cb);
        glfwSetMouseButtonCallback(win, glfw_ms_key_cb);
        glfwSetCursorPosCallback(win, glfw_mouse_move_cb);

        //fundamental_startup();

        struct kc_ctx *ctx = malloc(sizeof(*ctx));

        glfwSetWindowUserPointer(win, ctx);
        glfwSwapInterval(1);

        GLFWmonitor* mon = glfwGetPrimaryMonitor();

        const GLFWvidmode *mode = glfwGetVideoMode(mon);
        int width = (mode->width / 3) * 2;
        int height = (mode->height / 3) * 2;

        glfwSetWindowSize(win, width, height);

        int x_pos = (mode->width / 2) - (width / 2);
        int y_pos = (mode->height / 2) - (height / 2);
        glfwSetWindowPos(win, x_pos, y_pos);


        ctx->win = win;
        ctx->user_data = desc->user_data;
        ctx->log_fn = desc->log_fn ? desc->log_fn : kci_log_stub;

        //kci_tag_alloc_init(&ctx->allocator_tagged);
        //kci_platform_setup(&ctx->platform);

        *out_ctx = ctx;

        ctx->log_fn("Karbon CTX created");

        return KC_OK;
}


kc_result
kc_ctx_user_data(
        kc_ctx_t ctx,
        void **out_user_data)
{
        *out_user_data = ctx->user_data;

        return KC_OK;
}


/* ----------------------------------------------------------- Application -- */


void            kci_dummy_hook()        { /* does nothing */     }
const char *    kci_dummy_hook_name()   { return "Karbon Drive"; }


void
kci_load_libs(
        struct kd_app apps[],
        int count) {

        /* symbol strings */
        const char *sym_strs[] = {
                KD_HOOK_STARTUP_STR,
                KD_HOOK_TICK_STR,
                KD_HOOK_SHUTDOWN_STR,
                KD_HOOK_APP_NAME_STR,
        };

        /* must match sym strings */
        enum {
                HOOK_STARTUP = 0,
                HOOK_TICK,
                HOOK_SHUTDOWN,
                HOOK_APP_NAME,

                HOOK_COUNT
        };

        /* get symbols */
        int i, j;
        for(i = 0; i < count; ++i) {
                void *syms[KC_ARR_COUNT(sym_strs)] = {0};
                void *lib = apps[i].lib;

                for(j = 0; j < HOOK_COUNT; ++j) {
                        #if defined(__linux__) || defined(__APPLE__)
                        void *sym = dlsym(lib, sym_strs[j]);
                        syms[j] = sym;
                        #elif defined(_WIN32)
                        void *sym = GetProcAddress((HMODULE)lib, sym_strs[j]);
                        syms[j] = sym;
                        #endif
                }

                struct kd_app *app = &apps[i];

                /* pull out the function pointers */
                /* order is important -  must match sym_strs */
                if(syms[HOOK_STARTUP]) {
                        app->start = ((KD_STARTUPHOOKFN)syms[HOOK_STARTUP])();
                } else {
                        app->start = kci_dummy_hook;
                }

                if(syms[HOOK_TICK]) {
                        app->tick = ((KD_TICKHOOKFN)syms[HOOK_TICK])();
                } else {
                        app->tick = kci_dummy_hook;
                }

                if(syms[HOOK_SHUTDOWN]) {
                        app->close = ((KD_SHUTDOWNHOOKFN)syms[HOOK_SHUTDOWN])();
                } else {
                        app->close = kci_dummy_hook;
                }

                if(syms[HOOK_APP_NAME]) {
                        app->name = (KD_APPNAMEFN)syms[HOOK_APP_NAME];
                } else {
                        app->name = kci_dummy_hook_name;
                }
        }
}


int
str_ends_with(const char *str, const char *suffix)
{
        if (!str || !suffix)
                return 0;

        size_t lenstr = strlen(str);
        size_t lensuffix = strlen(suffix);
        if (lensuffix > lenstr)
                return 0;
        return strncmp(str + lenstr - lensuffix, suffix, lensuffix) == 0;
}


kc_result
kc_application_start(
        kc_ctx_t ctx,
        const struct kc_application_desc * desc)
{
        (void)desc;

        if(KC_EXTRA_LOGGING) {
                ctx->log_fn("Karbon Core Startup... ");
        }

        /* base directory */
        int count = 0;
        char *base_dir = 0;

        kdi_ctx_get_exe_dir(ctx, 0, &count);
        kc_array_create_with_capacity(base_dir, count);

        kdi_ctx_get_exe_dir(ctx, &base_dir[0], 0);

        /* symbols to load */
        void * funcs[KD_FUNC_COUNT];
        funcs[KD_PTR_CTX] = ctx;
        funcs[KD_FUNC_CTX_VENDOR_STRING]   = kdi_ctx_get_vendor_string;
        funcs[KD_FUNC_CTX_GRAPHICS_API]    = 0;
        funcs[KD_FUNC_CTX_EXE_DIR]         = kdi_ctx_get_exe_dir;
        funcs[KD_FUNC_CTX_APP_INDEX_GET]   = kdi_ctx_app_index_get;
        funcs[KD_FUNC_CTX_APP_INDEX_SET]   = kdi_ctx_app_index_set;
        funcs[KD_FUNC_CTX_CLOSE]           = kdi_ctx_close;
        funcs[KD_FUNC_EVENTS_GET]          = kdi_events_get;
        funcs[KD_FUNC_ALLOC]               = kdi_alloc_tagged;
        funcs[KD_FUNC_WINDOW_GET]          = kdi_window_get;
        funcs[KD_FUNC_WINDOW_SET]          = kdi_window_set;
        funcs[KD_FUNC_INPUT_KEYBOARD_GET]  = kdi_input_get_keyboards;
        funcs[KD_FUNC_INPUT_MICE_GET]      = kdi_input_get_mice;
        funcs[KD_FUNC_OPENGL_MAKE_CURRENT] = kdi_gl_make_current;
        funcs[KD_FUNC_LOG]                 = kdi_log;

        DIR *dir;
        struct dirent *ent;

        if ((dir = opendir (base_dir)) == NULL) {
                return KC_FAIL;
        }

        #if defined(__APPLE__)
        const char *ext = ".dylib";
        #elif defined(_WIN32)
        const char *ext = ".dll";
        #elif defined(__linux__)
        const char *ext = ".so";
        #endif

        struct kd_app *loaded_libs = 0;
        kc_array_create_with_capacity(loaded_libs, 32);

        while ((ent = readdir (dir)) != NULL) {
                const char *item_name = ent->d_name;

                /* check if file ends with extension */
                int ends_with = str_ends_with(item_name, ext);

                if(!ends_with && KC_EXTRA_LOGGING) {
                        char msg[2048] = {0};
                        strncat(msg, " - Ignore ", sizeof(msg) - strlen(msg));
                        strncat(msg, item_name, sizeof(msg) - strlen(msg));

                        ctx->log_fn(msg);
                }

                if(!ends_with) {
                        continue;
                }

                if(KC_EXTRA_LOGGING) {
                        char msg[2048] = {0};
                        strncat(msg, " - Trying ", sizeof(msg) - strlen(msg));
                        strncat(msg, item_name, sizeof(msg) - strlen(msg));

                        ctx->log_fn(msg);
                }

                /* try load */
                char path[2048] = {0};
                strncat(path, base_dir, sizeof(path) - strlen(path));
                strncat(path, item_name, sizeof(path) - strlen(path));

                #ifndef _WIN32
                void *lib =  dlopen(path, RTLD_NOW);
                #else
                HMODULE lib = LoadLibrary(path);
                #endif

                if(!lib && KC_EXTRA_LOGGING) {
                        char msg[2048] = {0};
                        strncat(msg, " - Failed ", sizeof(msg) - strlen(msg));
                        strncat(msg, item_name, sizeof(msg) - strlen(msg));

                        #ifndef _WIN32
                        strncat(msg, " - Err: ", sizeof(msg) - strlen(msg));
                        strncat(msg, dlerror(), sizeof(msg) - strlen(msg));
                        #endif

                        ctx->log_fn(msg);
                }

                if (!lib) {
                        continue;
                }

                #ifndef _WIN32
                void *sym = dlsym(lib, KD_HOOK_LOAD_STR);
                KD_LOAD_FN load_fn = (KD_LOAD_FN)sym;
                #else
                FARPROC sym = GetProcAddress((HMODULE)lib, KD_HOOK_LOAD_STR);
                KD_LOAD_FN load_fn = (KD_LOAD_FN)sym;
                #endif

                if(!load_fn) {
                        #ifndef _WIN32
                        dlclose(lib);
                        #else
                        assert(0); // what is close on windows */
                        #endif
                        continue;
                }

                if(KC_EXTRA_LOGGING) {
                        char msg[2048] = {0};
                        int buf_size = sizeof(msg) - strlen(msg);
                        strncat(msg, " - Loaded ", buf_size);

                        buf_size = sizeof(msg) - strlen(msg);
                        strncat(msg, item_name, buf_size);

                        ctx->log_fn(msg);
                }

                if(load_fn(funcs)) {
                        struct kd_app app = {0};
                        app.lib = lib;

                        kc_array_push(loaded_libs, app);
                }

        }

        /* copy libs out */
        ctx->apps.libs = loaded_libs;

        int load_count = kc_array_size(loaded_libs);

        if(!load_count) {
                if(KC_EXTRA_LOGGING) {
                        ctx->log_fn("No KD apps to launch");
                }
                return KC_FAIL;
        }

        /* get tick funcs */
        kci_load_libs(loaded_libs, load_count);

        if(KC_EXTRA_LOGGING) {
                ctx->log_fn("Karbon Core application loop");
        }

        ctx->apps.curr = -1;
        ctx->apps.next = 0;

        memset(ctx->mouse.keys, 0, sizeof(ctx->mouse.keys));

        /* loop */
        while(!glfwWindowShouldClose(ctx->win)) {
                glfwPollEvents();

                if(ctx->apps.curr != ctx->apps.next) {
                        ctx->frame_events = KD_EVENT_VIEWPORT_RESIZE;

                        if(ctx->apps.curr >= 0) {
                                ctx->apps.libs[ctx->apps.curr].close();
                        }

                        ctx->apps.curr = ctx->apps.next;

                        struct kd_app *app = &ctx->apps.libs[ctx->apps.curr];
                        app->start();

                        const char *name = app->name();
                        glfwSetWindowTitle(ctx->win, name);
                }

                //fundamental_tick();

                ctx->apps.libs[ctx->apps.curr].tick();

                //renderer_dx12_render();

                ctx->frame_events = 0;

                glfwSwapBuffers(ctx->win);

                /* clear key events */
                int i;
                for(i = 0; i < KD_KB_COUNT; ++i) {
                        ctx->keys[i] &= ~(KD_KEY_DOWN_EVENT);
                        ctx->keys[i] &= ~(KD_KEY_UP_EVENT);
                }

                for(i = 0; i < KD_MS_COUNT; ++i) {
                        ctx->mouse.keys[i] &= ~(KD_KEY_DOWN_EVENT);
                        ctx->mouse.keys[i] &= ~(KD_KEY_UP_EVENT);
                }

                ctx->frame_events = 0;
        }

        if(ctx->apps.curr >= 0) {
                ctx->apps.libs[ctx->apps.curr].close();
        }

        if(KC_EXTRA_LOGGING) {
                ctx->log_fn("Karbon has finished");
        }

        return KC_OK;
}
