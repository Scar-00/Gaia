#ifndef _GAIA_ITER_H_
#define _GAIA_ITER_H_

#include "../types.h"

typedef struct GaiaIter {
    size_t size;
    void *begin;
    void *end;
}GaiaIter;

#endif
