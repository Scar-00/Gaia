#include <Gaia/gaia.h>

GaiaIterator gaia_iter_init(size_t size, void *ptr, size_t data_size) {
    return (GaiaIterator){.size = size, .data_size = data_size, .ptr = ptr, .pos = 0};
}