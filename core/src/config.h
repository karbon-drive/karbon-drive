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


#endif
