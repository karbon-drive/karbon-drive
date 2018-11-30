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


typedef enum _kc_bool {
        KC_FALSE = 0,
        KC_TRUE = 1,
} kc_bool;


/* -------------------------------------------------------------- Lifetime -- */


kc_bool
fundamental_startup();


kc_bool
fundamental_shutdown();


/* ------------------------------------------------------------ Allocation -- */


typedef enum _kc_alloc_type {
        KC_ALLOC_HOUSEKEEPING,
        KC_ALLOC_RENDERER,
} kc_alloc_type;


kc_bool
kc_alloc_frame(kc_alloc_type type, void **out_addr, int *bytes);


/* ---------------------------------------------------------------- Assert -- */


#include <assert.h>
#define KC_ASSERT(expr) assert(expr)


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


#define kc_array_create_with_buffer(arr, buf, buf_size) do { _kci_arr_create_with_buffer((void**)&arr, sizeof(arr[0]), buf, buf_size); } while(0);
#define kc_array_create(arr)                            do { _kci_arr_create((void**)&arr, sizeof(arr[0]), 1); } while(0)
#define kc_array_create_with_capacity(arr, cap)         do { _kci_arr_create((void**)&arr, sizeof(arr[0]), cap); } while(0)
#define kc_array_destroy(arr)                           do { _kci_arr_destroy((void**)&arr); } while(0)
#define kc_array_size(arr)                              _kci_arr_size((void**)&arr, sizeof(arr[0]))
#define kc_array_capacity(arr)                          _kci_arr_capacity((void**)&arr, sizeof(arr[0]))
#define kc_array_resize(arr, cap)                       do { _kci_arr_resize((void**)&arr, cap, sizeof(arr[0])); } while(0)
#define kc_array_push(arr, item)                        do { _kci_arr_push((void**)&arr, sizeof(arr[0])); arr[kc_array_size(arr) - 1] = item;} while(0)
#define kc_array_erase(arr, index)                      do { _kci_arr_erase((void**)&arr, index, sizeof(arr[0])); } while(0)
#define kc_array_pop(arr)                               do { _kci_arr_pop((void**)&arr, sizeof(arr[0])); } while(0)
#define kc_array_insert(arr, i, item)                   do { _kci_arr_insert((void**)&arr, i, sizeof(arr[0])); arr[i] = item;} while(0)
#define kc_array_back(arr)                              arr[_kc_array_size(arr) - 1]
#define kc_array_clear(arr)                             do { _kci_arr_clear((void**)&arr); } while(0)
#define kc_array_data(arr)                              arr


/* ------------------------------------------------ Stretchy Array Private -- */
/*
  These are internal you shouldn't be calling them directly.
*/


void        _kci_arr_create_with_buffer(void **ptr, unsigned stride, void *buffer, unsigned size);
void        _kci_arr_create(void **ptr, unsigned stride, unsigned capacity);
void        _kci_arr_destroy(void **ptr);
unsigned    _kci_arr_size(void **ptr, unsigned stride);
unsigned    _kci_arr_capacity(void **ptr, unsigned stride);
void        _kci_arr_resize(void **ptr, unsigned size, unsigned stride);
unsigned    _kci_arr_push(void **ptr, unsigned stride);
void        _kci_arr_erase(void **ptr, unsigned index, unsigned stride);
void        _kci_arr_pop(void **ptr, unsigned stride);
unsigned    _kci_arr_insert(void **ptr, unsigned index, unsigned stride);
void        _kci_arr_clear(void**ptr);


/* inc guard */
#endif
