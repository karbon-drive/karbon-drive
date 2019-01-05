#ifndef KARBON_UTILS_INCLUDED
#define KARBON_UTILS_INCLUDED


#include <karbon/drive.h>


#ifdef __cplusplus
extern "C" {
#endif


struct kdu_buffer {
        void *memory;
        int used;
        int capacity;
};


kd_result
kdu_memory_to_buffer(void *mem, int size, void **out);


kd_result
kdu_append(void *mem, void *data, int size);


kd_result
kdu_pop(void *mem, int size);


#ifdef __cplusplus
} /* extern */
#endif


#endif
