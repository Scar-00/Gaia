#ifndef _GAIA_ITER_H_
#define _GAIA_ITER_H_

#include <Gaia/gaia.h>

typedef struct GaiaIterator {
    u32 pos;
    size_t size;
    void *ptr;
    size_t data_size;
}GaiaIterator;

GaiaIterator gaia_iter_init(size_t size, void *ptr, size_t data_size);

#define gaia_iter_loop(iter, i) for(size_t i = 0; i < iter.size; i++, iter.pos++)

#endif