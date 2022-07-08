#ifndef _GAIA_STACK_H_
#define _GAIA_STACK_H_

#include <Gaia/gaia.h>

typedef struct GaiaStackHeader {
    size_t length;
    size_t capacity;
}GaiaStackHeader;

#define gaia_stack_header(stack) ((GaiaStackHeader *) (stack) - 1)
#define gaia_stack_length(stack) (gaia_stack_header(stack)->length)
#define gaia_stack_capacity(stack) (gaia_stack_header(stack)->capacity)

#define gaia_stack(type) type *
#define gaia_stack_create(type, size) gaia_stack_int_create(size, sizeof(type))
#define gaia_stack_destroy(stack) (free(gaia_stack_header(stack), stack = NULL))
#define gaia_stack_free gaia_stack_destroy
#define gaia_stack_push(stack, value) ((stack) = gaia_stack_maygrow(stack, sizeof(*stack)), (stack)[gaia_stack_header(stack)->length++] = (value))
#define gaia_stack_pop(stack) ((stack)[gaia_stack_header(stack)->length], gaia_stack_length(stack)--)
#define gaia_stack_peek(stack)

GAIA_API void *gaia_stack_int_create(u32 capacity, size_t elem_size);
GAIA_API void *gaia_stack_maygrow(void *arr, size_t elem_size);


#endif