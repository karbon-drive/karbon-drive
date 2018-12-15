#ifndef CONFIG_INCLUDED
#define CONFIG_INCLUDED


#ifndef KC_ASSERT
#include <assert.h>
#define KD_ASSERT(expr) assert(expr)
#endif


#ifndef KC_PCHECK
        #ifndef NDEBUG
                #define KC_PCHECK 1
        #else
                #define KC_PCHECK 0
        #endif
#endif


#ifndef KC_EXTRA_LOGGING
        #ifndef NDEBUG
                #define KC_EXTRA_LOGGING 1
        #else
                #define KC_EXTRA_LOGGING 0
        #endif
#endif


#endif
