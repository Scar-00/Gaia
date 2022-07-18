#ifndef _GAIA_ARRAY_H_
#define _GAIA_ARRAY_H_

#include <Gaia/gaia.h>

typedef struct GaiaArrayHeader {
    size_t length;
    size_t capacity;
}GaiaArrayHeader;

#define gaia_array_header(a) ((GaiaArrayHeader *) (a) - 1)
#define gaia_array_length(a) (gaia_array_header(a)->length)
#define gaia_array_capacity(a) (gaia_array_header(a)->capacity)
#define gaia_array_interator(a) (gaia_array_header(a)->inter)

//FIXME: wrap freeing the array/stack in a function to avoid conflictis with memory manager
#define gaia_array(type) type *
#define gaia_array_create(type, size) gaia_array_int_create(size, sizeof(type))
#define gaia_array_destroy(arr) (free(gaia_array_header(arr)), arr = NULL);
#define gaia_array_free gaia_array_destroy
#define gaia_array_pushback(arr, value) ((arr) = gaia_array_maygrow(arr, sizeof(*arr)), (arr)[gaia_array_header(arr)->length++] = (value))
#define gaia_array_pop(arr) (gaia_array_length(arr)--, (arr)[gaia_array_header(arr)->length])
#define gaia_array_put(arr, index, value) ((arr) = gaia_array_maygrow(arr, sizeof(*arr)), (arr)[index] = (value))
//FIXME: maybe loop over the capacity of the array instead of the current length <- lenght may not cover the whole array
#define gaia_array_loop(arr, iter) for(size_t iter = 0; iter < gaia_array_capacity(arr); iter++)
#define gaia_array_foreach(arr, item)\
    __typeof__((*arr)) *item;\
    for(size_t i = 0; i < gaia_array_capacity(arr) && (item = &arr[i]) != (void *)__INT64_MAX__; i++)

GAIA_API void *gaia_array_int_create(u32 capacity, size_t elem_size);
GAIA_API void *gaia_array_maygrow(void *arr, size_t elem_size);

#endif