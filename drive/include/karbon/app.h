#ifndef KARBON_APP_INCLUDED
#define KARBON_APP_INCLUDED


#include <karbon/drive.h>


/* ------------------------------------------------------- App Description -- */


#define KD_APP_NAME(name) \
        KD_API const char * KD_EXPORT \
        kd_app_name() { return name; } \


#define KD_APP_DESC(desc) \
        KD_API const char * KD_EXPORT \
        kd_app_desc() { return desc; } \


#define KD_APP_GRAPHICS_API(api) \
        KD_API const char * KD_EXPORT \
        kd_app_gfx_api() { return api; } \


#define KD_APP_ICON_LARGE(icon_data, width, height) \
        KD_API void KD_EXPORT \
        kd_app_icon_large_details(uint8_t *d, int *w, int *h) { \
          d = icon_data; *w = width, *h = height; } \


/* -------------------------------------------------------- Lifetime hooks -- */


#define KD_APP_STARTUP_FN(startup_fn) \
        KD_API KD_STARTUPFN KD_EXPORT \
        kd_app_startup() { return startup_fn; } \


#define KD_APP_TICK_FN(tick_fn) \
        KD_API KD_TICKFN KD_EXPORT \
        kd_app_startup() { return tick_fn; } \


#define KD_APP_SHUTDOWN_FN(shutdown_fn) \
        KD_API KD_SHUTDOWNFN KD_EXPORT \
        kd_app_startup() { return shutdown_fn; } \


#endif
