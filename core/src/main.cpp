
#include <karbon/core.h>
#include <stdio.h>


void
app_log(const char *msg) {
        printf("%s\n", msg);
};


int
main(int argc, const char **argv)
{
        (void)argc;
        (void)argv;

        /* create and load a karbon ctx */        
        struct kc_ctx_desc ctx_desc;
        ctx_desc.type_id = KC_STRUCT_CTX_DESC;
        ctx_desc.ext = 0;
        ctx_desc.user_data = 0;
        ctx_desc.log_fn = app_log;

        kc_ctx_t kc_ctx = 0;
        kc_ctx_create(&ctx_desc, &kc_ctx);

        /* start application */
        struct kc_application_desc app_desc;
        app_desc.type_id = KC_STRUCT_APPLICATION_DESC;
        app_desc.ext = 0;
        app_desc.load_directory = "./";
        kc_application_start(kc_ctx, &app_desc);

        return 0;
}
