#include "../internal.h"

void *stack_global = NULL;

//Array funktions
GAIA_API void *gaia_array_int_create(u32 capacity, size_t elem_size) {
    GaiaArrayHeader *arr = malloc((capacity * elem_size) + sizeof(GaiaArrayHeader));
    memset(arr, 0, (capacity * elem_size) + sizeof(GaiaArrayHeader));
    arr[0].length = 0;
    arr[0].capacity = capacity;
    arr++;
    return arr;
}

GAIA_API void *gaia_array_maygrow(void *arr, size_t elem_size) {
    if(gaia_array_length(arr) == gaia_array_capacity(arr)) {
        gaia_array_capacity(arr) = gaia_array_capacity(arr) * 2;
        GaiaArrayHeader *tmp = realloc(gaia_array_header(arr), (gaia_array_capacity(arr) * elem_size) + sizeof(GaiaArrayHeader));
        tmp++;
        return tmp;
    }
    return arr;
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