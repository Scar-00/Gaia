#ifndef _GAIA_H_
#define _GAIA_H_

#ifdef __cplusplusp
extern "C" {
#endif

#if defined(DLL_BUILD)
    #define GAIA_API __declspec(dllexport)
#elif defined(DLL_USE)
    #define GAIA_API __declspec(dllimport)
#else
    #define GAIA_API
#endif

//TODO: let user provide own allocator 

#include "types.h"

#include "util/string.h"
#include "util/memory.h"
#include "util/array.h"
#include "util/stack.h"

#include "io/io.h"

#include "window/window.h"

#ifdef GAIA_SHORT_NAMING
#include "names.h"
#endif

#ifdef __cplusplus
}
#endif
#endif