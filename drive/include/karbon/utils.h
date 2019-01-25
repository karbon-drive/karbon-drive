#ifndef KARBON_UTILS_INCLUDED
#define KARBON_UTILS_INCLUDED


#include <karbon/drive.h>
#include <stdint.h>
#include <string.h>


#ifdef __cplusplus
extern "C" {
#endif


/* --------------------------------------------------------------- C-Array -- */
/*
  Some static c-array helpers, more to help code description.
*/


#define KD_ARR_COUNT(arr) (sizeof(arr) / sizeof(arr[0]))
#define KD_ARR_DATA(arr) &arr[0]


/* --------------------------------------------------------------- Generic -- */
/*
  Generic wrappers that may or maynot be useful.
*/


#define KD_UNUSED(var) ((void)var)
#define KD_MEMZERO(var) (memset(&var, 0, sizeof(var)))
#define KD_NULL 0


#ifdef __cplusplus
} /* extern */
#endif


#endif
