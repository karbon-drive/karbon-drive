#include "tagged_allocator.h"
#include <stdlib.h>


/* --------------------------------------------------------------- Config --  */

/* 2MB */
#define TAGGED_BLOCK 2097152


void
kci_tag_alloc_init(
        struct kci_tagged_allocator *allocator)
{
        int count = sizeof(allocator->tags) / sizeof(allocator->tags[0]);
        int init = count >> 2;
        int i;
        
        /* init memory pool, but only allocate memory part of the pool */
        for(i = 0; i < count; ++i) {
                allocator->tags[i] = 0;
                allocator->memory[i] = 0;
                
                if(i < init) {
                        allocator->memory[i] = malloc(TAGGED_BLOCK);
                }
        }
}


void
kci_tag_alloc_clear_tag(
        struct kci_tagged_allocator *allocator,
        uint64_t tag)
{
        int count = sizeof(allocator->tags[0]) / sizeof(allocator->tags);
        int i;
        
        /* init memory pool, but only allocate memory part of the pool */
        for(i = 0; i < count; ++i) {
                if(allocator->tags[i] == tag) {
                        allocator->tags[i] = 0;
                }
        }
}


struct kci_tagged_memory
kci_tag_alloc_get(
        struct kci_tagged_allocator *allocator,
        uint64_t tag)
{
        int count = sizeof(allocator->tags) / sizeof(allocator->tags[0]);
        int i;
        
        /* init memory pool, but only allocate memory part of the pool */
        for(i = 0; i < count; ++i) {
                if(allocator->tags[i] != 0) {
                        continue;
                }
                
                allocator->tags[i] = tag;
                
                struct kci_tagged_memory mem;
                
                if(!allocator->memory[i]) {
                        allocator->memory[i] = malloc(TAGGED_BLOCK);
                }
                
                mem.memory = allocator->memory[i];
                mem.bytes = TAGGED_BLOCK;
                
                return mem;
        }

        struct kci_tagged_memory mem;
        mem.bytes = 0;
        mem.memory = 0;
        
        return mem;
}


/* --------------------------------------------------------------- Config --  */

#undef TAGGED_BLOCK
