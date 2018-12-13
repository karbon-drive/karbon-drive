#if defined(__linux__)
#include "platform.h"

#include <X11/X.h>
#include <X11/Xlib.h>


int
kci_platform_setup(
        struct kci_platform *ctx)
{
        Display                 *dpy;
        Window                  root;
        Colormap                cmap;
        XSetWindowAttributes    swa;
        Window                  win;
        XWindowAttributes       gwa;
        
        dpy = XOpenDisplay(NULL);
        root = DefaultRootWindow(dpy);

        long visualMask = VisualScreenMask;
        int numberOfVisuals;
        XVisualInfo vii = {};
        vii.screen = DefaultScreen(dpy);
        XVisualInfo *vi = XGetVisualInfo(dpy, visualMask, &vii, &numberOfVisuals);

        cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
        
        swa.colormap = cmap;
        swa.event_mask = ExposureMask | KeyPressMask;

        win = XCreateWindow(dpy, root, 0, 0, 600, 600, 0, vi->depth, InputOutput, vi->visual, CWColormap | CWEventMask, &swa);

        XMapWindow(dpy, win);
        XStoreName(dpy, win, "Karbon Drive");

        ctx->dpy = dpy;
        ctx->root = root;
        ctx->gwa = gwa;
        ctx->win = win;

        return 0;
}


int
kci_platform_process(
        struct kci_platform *win)
{
        XEvent xev;
        XNextEvent(win->dpy, &xev);
        
        if(xev.type == Expose) {
                XGetWindowAttributes(win->dpy, win->win, &win->gwa);
        }
                
        else if(xev.type == KeyPress) {
                XDestroyWindow(win->dpy, win->win);
                XCloseDisplay(win->dpy);
                return 0;
        }

        return 1;
}


int
kci_platform_destroy(
        struct kci_platform *plat)
{
        (void)plat;
        return 0;
}



/* linux */
#endif

