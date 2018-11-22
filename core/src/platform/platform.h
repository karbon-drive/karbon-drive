#ifndef WINDOW_INCLUDED
#define WINDOW_INCLUDED


struct kci_platform {
        void *h_instance;
        void *hwnd;
        void *dc;

        int width;
        int height;
};


int
kci_platform_setup(
        struct kci_platform *win);


int
kci_platform_process(
        struct kci_platform *win);


#endif
