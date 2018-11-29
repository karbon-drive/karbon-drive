#include "fundamental.h"


/* -------------------------------------------------------- Stretchy Array -- */


/* turn this on if you suspect something in array */
#ifndef NDEBUG
#define KC_ARR_PEDANTIC_CHECKS 0
#else
#define KC_ARR_PEDANTIC_CHECKS 1
#endif


void* kc_internal_array_grow(void **ptr, unsigned stride, unsigned capacity);
void kc_internal_array_should_grow(void **ptr, unsigned stride);


struct kc_array_header {
        unsigned char * capacity;
        unsigned char * end;
};


void
kc_internal_array_create(void **ptr, unsigned stride, unsigned capacity)
{
        if (KC_ARR_PEDANTIC_CHECKS) {
                KC_ASSERT(ptr);
                KC_ASSERT(*ptr == 0);
        }

        if (capacity == 0)
        {
                capacity = 1;
        }

        unsigned array_bytes = stride * capacity;
        unsigned bytes = sizeof(struct kc_array_header) + array_bytes;

        struct kc_array_header *header = malloc(bytes);
        unsigned char *begin = (unsigned char*)&header[1];

        header[0].end = begin;
        header[0].capacity = begin + array_bytes;

        *ptr = (void*)begin;
}


void
kc_internal_array_destroy(void **ptr)
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
kc_internal_array_size(void **ptr, unsigned stride)
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
kc_internal_array_capacity(void **ptr, unsigned stride)
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
kc_internal_array_resize(void **ptr, unsigned size, unsigned stride)
{
        if (KC_ARR_PEDANTIC_CHECKS) {
                KC_ASSERT(ptr);
                KC_ASSERT(*ptr);
                KC_ASSERT(size);
                KC_ASSERT(stride);
        }

        struct kc_array_header *header = ((struct kc_array_header*)*ptr);
        header--;

        header = kc_internal_array_grow(ptr, stride, size);
        header->end = header->capacity;
}


void
kc_internal_array_erase(void **ptr, unsigned index, unsigned stride)
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

        unsigned count = kc_internal_array_size(ptr, stride);

        if (count > 0) {
                unsigned len = (stride * (count - 1)) - (stride * index);
                memmove(dst, src, len);

                header->end -= stride;
        }
}


void
kc_internal_array_pop(void **ptr, unsigned stride)
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
kc_internal_array_clear(void**ptr)
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
kc_internal_array_grow(void **ptr, unsigned stride, unsigned capacity)
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

        unsigned count = kc_internal_array_size(ptr, stride);
        unsigned bytes = (stride * capacity) + sizeof(struct kc_array_header);

        header = (struct kc_array_header*)realloc(header, bytes);

        unsigned char *begin = (unsigned char*)&header[1];

        header->end = begin + (stride * count);
        header->capacity = begin + (stride * capacity);

        *ptr = &header[1];

        return header;
}


unsigned
kc_internal_array_push(void **ptr, unsigned stride)
{
        if (KC_ARR_PEDANTIC_CHECKS) {
                KC_ASSERT(ptr);
                KC_ASSERT(*ptr);
                KC_ASSERT(stride);
        }

        struct kc_array_header *header = ((struct kc_array_header*)*ptr);
        header--;

        if (header->end >= header->capacity) {
                unsigned count = kc_internal_array_size(ptr, stride);
                header = (struct kc_array_header *)kc_internal_array_grow(
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
kc_internal_array_insert(void **ptr, unsigned index, unsigned stride)
{
        if (KC_ARR_PEDANTIC_CHECKS) {
                KC_ASSERT(ptr);
                KC_ASSERT(*ptr);
                KC_ASSERT(stride);
        }

        /* increase the size to make space */
        kc_internal_array_push(ptr, stride);

        struct kc_array_header *header = ((struct kc_array_header*)*ptr);
        header--;

        unsigned count = kc_internal_array_size(ptr, stride);

        unsigned char * begin = (unsigned char*)(*ptr);

        unsigned char *dst = &begin[stride * (index + 1)];
        unsigned char *src = &begin[stride * index];

        unsigned len = (count - index - 1) * stride;

        memmove(dst, src, len);

        return index;
}


void
kc_internal_array_should_grow(void **ptr, unsigned stride)
{
        if (KC_ARR_PEDANTIC_CHECKS) {
                KC_ASSERT(ptr);
                KC_ASSERT(*ptr);
                KC_ASSERT(stride);
        }

        struct kc_array_header *curr_arr = ((struct kc_array_header*)*ptr);
        curr_arr--;

        unsigned count = kc_internal_array_size(ptr, stride) + 1;

        if (count > kc_internal_array_capacity(ptr, stride)) {
                kc_internal_array_grow(ptr, stride, count * 2);
        }
}


/* ------------------------------------------------------ [ Array Config ] -- */


#undef KC_ARR_PEDANTIC_CHECKS
