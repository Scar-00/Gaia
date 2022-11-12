#include "../internal.h"
#include "Gaia/gaia.h"
#include "Gaia/util/array.h"
#include "Gaia/util/iter.h"
#include <stdlib.h>

GaiaIter gaia_iter_init(void *begin, void *end, size_t data_size);

//Array funktions
GAIA_API void *gaia_array_int_create(u32 capacity, size_t elem_size) {
    GaiaArrayHeader *arr = malloc((capacity * elem_size) + sizeof(GaiaArrayHeader));
    memset(arr, 0, (capacity * elem_size) + sizeof(GaiaArrayHeader));
    arr[0].length = 0;
    arr[0].capacity = capacity;
    arr++;
    gaia_array_header(arr)->iter = gaia_iter_init(arr, (char*)arr + (capacity * elem_size), elem_size);
    return arr;
}

GAIA_API void *gaia_array_maygrow(void *arr, size_t elem_size) {
    if(gaia_array_length(arr) == gaia_array_capacity(arr)) {
        gaia_array_capacity(arr) = gaia_array_capacity(arr) * 2;
        GaiaArrayHeader *tmp = realloc(gaia_array_header(arr), (gaia_array_capacity(arr) * elem_size) + sizeof(GaiaArrayHeader));
        tmp++;
        gaia_array_header(tmp)->iter = gaia_iter_init(tmp, (char*)tmp + (gaia_array_capacity(tmp) * elem_size), elem_size);
        return tmp;
    }
    return arr;
}

GAIA_API void gaia_array_int_free(void *arr) {
    free(arr);
    arr = NULL;
}

//stack funktions
GAIA_API void *gaia_stack_int_create(u32 capacity, size_t elem_size) {
    GaiaStackHeader *arr = malloc((capacity * elem_size) + sizeof(GaiaStackHeader));
    memset(arr, 0, (capacity * elem_size) + sizeof(GaiaStackHeader));
    arr[0].length = 0;
    arr[0].capacity = capacity;
    arr++;
    return arr;
}

GAIA_API void *gaia_stack_maygrow(void *arr, size_t elem_size) {
    if(gaia_stack_length(arr) == gaia_stack_capacity(arr)) {
        gaia_stack_capacity(arr) = gaia_stack_capacity(arr) * 2;
        GaiaStackHeader *tmp = realloc(gaia_stack_header(arr), (gaia_stack_capacity(arr) * elem_size) + sizeof(GaiaStackHeader));
        tmp++;
        return tmp;
    }
    return arr;
}

//iters
GaiaIter gaia_iter_init(void *begin, void *end, size_t data_size) {
    return (GaiaIter){.begin = begin, .end = end, .size = data_size};
}
