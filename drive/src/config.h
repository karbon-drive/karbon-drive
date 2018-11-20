#ifndef CONFIG_INCLUDED
#define CONFIG_INCLUDED


#ifndef KD_ASSERT
#include <assert.h>
#define KD_ASSERT(expr) assert(expr)
#endif


#ifndef KD_PCHECK
        #ifndef NDEBUG
                #define KD_PCHECK 1
        #else
                #define KD_PCHECK 0
        #endif
#endif


#endif
