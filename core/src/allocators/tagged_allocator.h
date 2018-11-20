#ifndef ALLOCATOR_INCLUDED
#define ALLOCATOR_INCLUDED


#include <stdint.h>


struct kci_tagged_allocator {
        uint64_t tags[512];
        void *memory[512];
};


void
kci_tag_alloc_init(
        struct kci_tagged_allocator *allocator);


void
kci_tag_alloc_clear_tag(
        struct kci_tagged_allocator *allocator,
        uint64_t tag);


struct kci_tagged_memory {
        void *memory;
        int bytes;
};

struct kci_tagged_memory
kci_tag_alloc_get(
        struct kci_tagged_allocator *allocator,
        uint64_t tag);


/* inc guard */
#endif
