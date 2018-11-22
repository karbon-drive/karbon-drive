#ifndef WINDOW_INCLUDED
#define WINDOW_INCLUDED

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

struct kci_platform {
        #ifdef _WIN32
        void *h_instance;
        void *hwnd;
        void *dc;

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
        #endif
};


int
kci_platform_setup(
        struct kci_platform *win);


int
kci_platform_process(
        struct kci_platform *win);


#endif
