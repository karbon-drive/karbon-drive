#if defined(__linux__)
#include "platform.h"

#include <X11/X.h>
#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>


int
kci_platform_setup(
        struct kci_platform *ctx)
{
        Display                 *dpy;
        Window                  root;
        GLint                   att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
//        XVisualInfo             *vi;
        Colormap                cmap;
        XSetWindowAttributes    swa;
        Window                  win;
        GLXContext              glc;
        XWindowAttributes       gwa;
        
        dpy = XOpenDisplay(NULL);
        root = DefaultRootWindow(dpy);
        /*
        vi = glXChooseVisual(dpy, 0, att);
        cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
        */

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

        //glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
        //glXMakeCurrent(dpy, win, glc);

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
                //glXSwapBuffers(win->dpy, win->win);
        }
                
        else if(xev.type == KeyPress) {
                //glXMakeCurrent(win->dpy, None, NULL);
                //glXDestroyContext(win->dpy, win->glc);
                XDestroyWindow(win->dpy, win->win);
                XCloseDisplay(win->dpy);
                return 0;
        }

        return 1;
}



/* linux */
#endif

