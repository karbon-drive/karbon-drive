#ifndef CORE_CTX_INCLUDED
#define CORE_CTX_INCLUDED


#include <karbon/core.h>
#include <karbon/drive.h>
#include <vector>
#include "allocators/tagged_allocator.h"
#include "platform/platform.h"


typedef enum _kd_alloc_id {
        KD_ALLOCTAG_HOUSEKEEPING = 1,
} kd_alloc_id;


struct kci_ctx_fn {
        KD_CTX_GET_VENDOR_STRING_FN vendor_ctx;
        KD_WINDOW_GET_FN win_get;
        KD_WINDOW_SET_FN win_set;
        KD_ALLOC_FN alloc;
};


struct kci_alloc_fn {
        int i;
};


/* ------------------------------------------------------------------ Libs -- */


struct kd_app {
        void *lib;
        KD_STARTUPFN start;
        KD_TICKFN tick;
        KD_SHUTDOWNFN close;
};


struct kci_applications {
        std::vector<kd_app> libs;
        size_t curr;
        size_t next;
};


/* --------------------------------------------------------------- Context -- */


struct kc_ctx {
        struct kci_ctx_fn fn_ctx;
        struct kci_applications apps;
        
        KC_LOG_FN log_fn;
        
        struct kci_tagged_allocator allocator_tagged;
        struct kci_platform platform;
        
        void *user_data;

        uint64_t frame_events;
};

#endif
