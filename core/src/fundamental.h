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


/* --------------------------------------------------------- Dynamic Array -- */


/* inc guard */
#endif
