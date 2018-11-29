#ifndef FUNDAMENTAL_INCLUDED
#define FUNDAMENTAL_INCLUDED


/* ----------------------------------------------------------------- Types -- */


#ifndef __cplusplus
        #ifdef _MSC_VER
                typedef signed   __int8  int8_t;
                typedef unsigned __int8  uint8_t;
                typedef signed   __int16 int16_t;
                typedef unsigned __int16 uint16_t;
                typedef signed   __int32 int32_t;
                typedef unsigned __int32 uint32_t;
                typedef signed   __int64 int64_t;
                typedef unsigned __int64 uint64_t;
                #include <stddef.h>
        #else
                #include <stdint.h>
                #include <stddef.h>
        #endif
#else
        #include <cstdint>
        #include <cstddef>
#endif


/* --------------------------------------------------------- Array Helpers -- */


#define KC_ARR_COUNT(arr) sizeof(arr) / sizeof(arr[0])
#define KC_ARR_DATA(arr) &arr[0]


/* -------------------------------------------------------- Stretchy Array -- */
/*
  C is a little awkward for containers, but this is a good balance for arrays.
  It uses macros unforunatly but it means we have a typesafe buffer.

  int *array = NULL;
  codex_array_create(array);
  codex_array_push(array, 123);
*/


#define kc_array_create(arr)                            do { kc_internal_array_create((void**)&arr, sizeof(arr[0]), 1); } while(0)
#define kc_array_create_with_capacity(arr, cap)         do { kc_internal_array_create((void**)&arr, sizeof(arr[0]), cap); } while(0)
#define kc_array_destroy(arr)                           do { kc_internal_array_destroy((void**)&arr); } while(0)
#define kc_array_size(arr)                              kc_internal_array_size((void**)&arr, sizeof(arr[0]))
#define kc_array_capacity(arr)                          kc_internal_array_capacity((void**)&arr, sizeof(arr[0]))
#define kc_array_resize(arr, cap)                       do { kc_internal_array_resize((void**)&arr, cap, sizeof(arr[0])); } while(0)
#define kc_array_push(arr, item)                        do { kc_internal_array_push((void**)&arr, sizeof(arr[0])); arr[kc_array_size(arr) - 1] = item;} while(0)
#define kc_array_erase(arr, index)                      do { kc_internal_array_erase((void**)&arr, index, sizeof(arr[0])); } while(0)
#define kc_array_pop(arr)                               do { kc_internal_array_pop((void**)&arr, sizeof(arr[0])); } while(0)
#define kc_array_insert(arr, i, item)                   do { kc_internal_array_insert((void**)&arr, i, sizeof(arr[0])); arr[i] = item;} while(0)
#define kc_array_back(arr)                              arr[kc_array_size(arr) - 1]
#define kc_array_clear(arr)                             do { kc_internal_array_clear((void**)&arr); } while(0)
#define kc_array_data(arr)                              arr


/* ------------------------------------------------ Stretchy Array Private -- */
/*
  These are internal you shouldn't be calling them directly.
*/


void        kc_internal_array_create(void **ptr, unsigned stride, unsigned capacity);
void        kc_internal_array_destroy(void **ptr);
unsigned    kc_internal_array_size(void **ptr, unsigned stride);
unsigned    kc_internal_array_capacity(void **ptr, unsigned stride);
void        kc_internal_array_resize(void **ptr, unsigned size, unsigned stride);
unsigned    kc_internal_array_push(void **ptr, unsigned stride);
void        kc_internal_array_erase(void **ptr, unsigned index, unsigned stride);
void        kc_internal_array_pop(void **ptr, unsigned stride);
unsigned    kc_internal_array_insert(void **ptr, unsigned index, unsigned stride);
void        kc_internal_array_clear(void**ptr);


/* inc guard */
#endif
