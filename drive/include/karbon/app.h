#ifndef KARBON_APP_INCLUDED
#define KARBON_APP_INCLUDED


#include <karbon/drive.h>


/* ------------------------------------------------------- App Description -- */


#define KD_APP_NAME(name) \
        KD_API KD_EXPORT const char * \
        kd_app_name() { return name; } \


#define KD_APP_DESC(desc) \
        KD_API KD_EXPORT const char * \
        kd_app_desc() { return desc; } \


#define KD_APP_GRAPHICS_API(api) \
        KD_API KD_EXPORT const char * \
        kd_app_gfx_api() { return api; } \


#define KD_APP_ICON_LARGE(icon_data, width, height) \
        KD_API KD_EXPORT void \
        kd_app_icon_large_details(uint8_t *d, int *w, int *h) { \
          d = icon_data; *w = width, *h = height; } \


/* -------------------------------------------------------- Lifetime hooks -- */


#define KD_APP_STARTUP_FN(startup_fn) \
        KD_API KD_EXPORT KD_STARTUPFN \
        kd_app_startup() { return startup_fn; } \


#define KD_APP_TICK_FN(tick_fn) \
        KD_API KD_EXPORT KD_TICKFN \
        kd_app_tick() { return tick_fn; } \


#define KD_APP_SHUTDOWN_FN(shutdown_fn) \
        KD_API KD_EXPORT KD_SHUTDOWNFN \
        kd_app_shutdown() { return shutdown_fn; } \


#endif
