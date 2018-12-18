#ifndef WINDOW_INCLUDED
#define WINDOW_INCLUDED


#include <karbon/drive.h>


#if defined(__linux__)
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <GL/glx.h>
#include <GL/glxext.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif


struct kci_platform {
        #ifdef _WIN32
        void *h_instance;
        void *hwnd;
        void *dc;
        void *glrc;

        int width;
        int height;

        #elif defined(__linux__)
        Display                 *dpy;
        Window                  root;
        XVisualInfo             *vi;
        Colormap                cmap;
        XSetWindowAttributes    swa;
        Window                  win;
        XWindowAttributes       gwa;

        GLXContext glc;
        #endif

        uint8_t keystate[KD_KB_COUNT];
};


int
kci_platform_setup(
        struct kci_platform *win);


int
kci_platform_process(
        struct kci_platform *win);


int
kci_platform_destroy(
        struct kci_platform *plat);


/* --------------------------------------------------------------- Open GL -- */


int
kci_platform_opengl_make_current(
        struct kci_platform *plat);


#endif
