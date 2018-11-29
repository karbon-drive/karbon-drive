#ifndef CORE_CTX_INCLUDED
#define CORE_CTX_INCLUDED


#include <karbon/core.h>
#include <karbon/drive.h>
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


typedef void *kc_lib;


struct kci_applications {
        /* array */ kc_lib *libs;
};


/* ----------------------------------------------------------------- Chunk -- */


struct kci_chunk {
        int i;
};



/* -------------------------------------------------------------- Renderer -- */


struct kci_rdr_chunk {
        int i;
};


struct kci_camera {
        int i;
};


#define KCI_MAX_CAMERA 4
#define KCI_MAX_CHUNKS 32


struct kci_rdr {
        /* array */ struct kci_camera *cameras;
        /* array */ struct kci_rdr_chunk *chunks;
        /* array */ struct kci_rdr_chunk **pending_chunks;
};


/* --------------------------------------------------------------- Context -- */


struct kc_ctx {
        struct kci_ctx_fn fn_ctx;
        struct kci_applications apps;
        
        KC_LOG_FN log_fn;
        
        struct kci_tagged_allocator allocator_tagged;
        struct kci_platform platform;
        
        void *user_data;

        struct kci_rdr rdr_data;
};

#endif
