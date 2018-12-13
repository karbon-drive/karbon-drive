#include "fundamental.h"
#include "allocators/tagged_allocator.h"
#include <stdlib.h>
#include <string.h>


struct kci_tagged_allocator tag_alloc;


/* -------------------------------------------------------------- Lifetime -- */


kc_bool
fundamental_startup() {
        kci_tag_alloc_init(&tag_alloc);

        return KC_TRUE;
}


kc_bool
fundamental_tick() {
        kci_tag_alloc_clear_tag(
                &tag_alloc,
                KC_ALLOC_HOUSEKEEPING);

        kci_tag_alloc_clear_tag(
                &tag_alloc,
                KC_ALLOC_RENDERER);

        return KC_TRUE;
}


kc_bool
fundamental_shutdown() {
        return KC_FALSE;
}


/* ------------------------------------------------------------ Allocation -- */


kc_bool
kc_alloc_frame(kc_alloc_type type, void **out_addr, int *bytes) {
        struct kci_tagged_memory mem = kci_tag_alloc_get(&tag_alloc, type);

        *out_addr = mem.memory;
        *bytes = mem.bytes;

        return KC_FALSE;
}



/* -------------------------------------------------------- Stretchy Array -- */


/* turn this on if you suspect something in array */
#ifndef NDEBUG
#define KC_ARR_PEDANTIC_CHECKS 0
#else
#define KC_ARR_PEDANTIC_CHECKS 1
#endif


void* _kci_arr_grow(void **ptr, unsigned stride, unsigned capacity);
void _kci_arr_should_grow(void **ptr, unsigned stride);


struct kc_array_header {
        unsigned char * capacity;
        unsigned char * end;
};


void
_kci_arr_create_with_buffer(
        void **ptr,
        unsigned stride,
        void *buffer,
        unsigned buffer_size)
{
        if (KC_ARR_PEDANTIC_CHECKS) {
                KC_ASSERT(ptr);
                KC_ASSERT(*ptr == 0);
                KC_ASSERT(stride < buffer_size);
                KC_ASSERT(buffer);
                KC_ASSERT(buffer_size);
        }

        struct kc_array_header *header = (struct kc_array_header*)buffer; 
        unsigned char *begin = (unsigned char*)&header[1];

        header[0].end = begin;
        header[0].capacity = (unsigned char*)header + buffer_size;

        *ptr = (void*)begin;

}


void
_kci_arr_create(
        void **ptr,
        unsigned stride,
        unsigned capacity)
{
        if (capacity == 0) {
                capacity = 1;
        }

        unsigned array_bytes = stride * capacity;
        unsigned bytes = sizeof(struct kc_array_header) + array_bytes;

        void *buffer = malloc(bytes);

        _kci_arr_create_with_buffer(ptr, stride, buffer, bytes);

        /*
        unsigned array_bytes = stride * capacity;
        unsigned bytes = sizeof(struct kc_array_header) + array_bytes;

        struct kc_array_header *header = malloc(bytes);
        unsigned char *begin = (unsigned char*)&header[1];

        header[0].end = begin;
        header[0].capacity = begin + array_bytes;

        *ptr = (void*)begin;
        */
}


void
_kci_arr_destroy(void **ptr)
{
        if (KC_ARR_PEDANTIC_CHECKS) {
                KC_ASSERT(ptr);
                KC_ASSERT(*ptr);
        }

        struct kc_array_header *header = ((struct kc_array_header*)*ptr);
        header--;

        free(header);
        *ptr = 0;
}


unsigned
_kci_arr_size(void **ptr, unsigned stride)
{
        if (KC_ARR_PEDANTIC_CHECKS) {
KC_ASSERT(ptr);
                KC_ASSERT(*ptr);
                KC_ASSERT(stride);
        }

        struct kc_array_header *header = ((struct kc_array_header*)*ptr);
        header--;

        unsigned char * begin = ((unsigned char*)*ptr);
        unsigned count = (header->end - begin) / stride;

        return count;
}


unsigned
_kci_arr_capacity(void **ptr, unsigned stride)
{
        if (KC_ARR_PEDANTIC_CHECKS) {
                KC_ASSERT(ptr);
                KC_ASSERT(*ptr);
                KC_ASSERT(stride);
        }

        struct kc_array_header *header = ((struct kc_array_header*)*ptr);
        header--;

        return (header->capacity - ((unsigned char*)*ptr)) / stride;
}


void
_kci_arr_resize(void **ptr, unsigned size, unsigned stride)
{
        if (KC_ARR_PEDANTIC_CHECKS) {
                KC_ASSERT(ptr);
                KC_ASSERT(*ptr);
                KC_ASSERT(size);
                KC_ASSERT(stride);
        }

        struct kc_array_header *header = ((struct kc_array_header*)*ptr);
        header--;

        header = (kc_array_header*)_kci_arr_grow(ptr, stride, size);
        header->end = header->capacity;
}


void
_kci_arr_erase(void **ptr, unsigned index, unsigned stride)
{
        if (KC_ARR_PEDANTIC_CHECKS) {
                KC_ASSERT(ptr);
                KC_ASSERT(*ptr);
                KC_ASSERT(stride);
        }

        struct kc_array_header *header = ((struct kc_array_header*)*ptr);
        header--;

        unsigned char *dst = &((unsigned char*)(*ptr))[stride * index];
        unsigned char *src = &((unsigned char*)(*ptr))[stride * (index + 1)];

        unsigned count = _kci_arr_size(ptr, stride);

        if (count > 0) {
                unsigned len = (stride * (count - 1)) - (stride * index);
                memmove(dst, src, len);

                header->end -= stride;
        }
}


void
_kci_arr_pop(void **ptr, unsigned stride)
{
        if (KC_ARR_PEDANTIC_CHECKS) {
                KC_ASSERT(ptr);
                KC_ASSERT(*ptr);
        }

        unsigned char *begin = (unsigned char*)*ptr;

        struct kc_array_header *header = ((struct kc_array_header*)*ptr);
        header--;

        if (begin < header->end) {
                header->end -= stride;
        }
}


void
_kci_arr_clear(void**ptr)
{
        if (KC_ARR_PEDANTIC_CHECKS) {
                KC_ASSERT(ptr);
                KC_ASSERT(*ptr);
        }

        struct kc_array_header *header = ((struct kc_array_header*)*ptr);
        header--;

        unsigned char *begin = (unsigned char*)&header[1];

        header->end = begin;
}


void*
_kci_arr_grow(void **ptr, unsigned stride, unsigned capacity)
{
        if (KC_ARR_PEDANTIC_CHECKS) {
                KC_ASSERT(ptr);
                KC_ASSERT(*ptr);
                KC_ASSERT(stride);
                KC_ASSERT(capacity);
        }

        /* increase buffer */
        struct kc_array_header *header = ((struct kc_array_header*)*ptr);
        header--;

        unsigned count = _kci_arr_size(ptr, stride);
        unsigned bytes = (stride * capacity) + sizeof(struct kc_array_header);

        header = (struct kc_array_header*)realloc(header, bytes);

        unsigned char *begin = (unsigned char*)&header[1];

        header->end = begin + (stride * count);
        header->capacity = begin + (stride * capacity);

        *ptr = &header[1];

        return header;
}


unsigned
_kci_arr_push(void **ptr, unsigned stride)
{
        if (KC_ARR_PEDANTIC_CHECKS) {
                KC_ASSERT(ptr);
                KC_ASSERT(*ptr);
                KC_ASSERT(stride);
        }

        struct kc_array_header *header = ((struct kc_array_header*)*ptr);
        header--;

        if (header->end >= header->capacity) {
                unsigned count = _kci_arr_size(ptr, stride);
                header = (struct kc_array_header *)_kci_arr_grow(
                        ptr,
                        stride,
                        count * 2
                );
        }

        unsigned char * begin = (unsigned char*)(&header[1]);

        /* increment end */
        unsigned index = (header->end - begin) / stride;
        header->end += stride;

        return index;
}


unsigned
_kci_arr_insert(void **ptr, unsigned index, unsigned stride)
{
        if (KC_ARR_PEDANTIC_CHECKS) {
                KC_ASSERT(ptr);
                KC_ASSERT(*ptr);
                KC_ASSERT(stride);
        }

        /* increase the size to make space */
        _kci_arr_push(ptr, stride);

        struct kc_array_header *header = ((struct kc_array_header*)*ptr);
        header--;

        unsigned count = _kci_arr_size(ptr, stride);

        unsigned char * begin = (unsigned char*)(*ptr);

        unsigned char *dst = &begin[stride * (index + 1)];
        unsigned char *src = &begin[stride * index];

        unsigned len = (count - index - 1) * stride;

        memmove(dst, src, len);

        return index;
}


void
_kci_arr_should_grow(void **ptr, unsigned stride)
{
        if (KC_ARR_PEDANTIC_CHECKS) {
                KC_ASSERT(ptr);
                KC_ASSERT(*ptr);
                KC_ASSERT(stride);
        }

        struct kc_array_header *curr_arr = ((struct kc_array_header*)*ptr);
        curr_arr--;

        unsigned count = _kci_arr_size(ptr, stride) + 1;

        if (count > _kci_arr_capacity(ptr, stride)) {
                _kci_arr_grow(ptr, stride, count * 2);
        }
}


/* ------------------------------------------------------ [ Array Config ] -- */


#undef KC_ARR_PEDANTIC_CHECKS
