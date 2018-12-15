#ifdef __APPLE__


int
kci_platform_setup(
        struct kci_platform *win)
{
        
        return 0;
}


int
kci_platform_process(
        struct kci_platform *win)
{
        return 0;
}


int
kci_platform_destroy(
        struct kci_platform *plat)
{
        return 0;
}


/* --------------------------------------------------------------- Open GL -- */


int
kci_platform_opengl_make_current(
        struct kci_platform *plat)
{
        return 0;
}


/* platform guard */
#endif
