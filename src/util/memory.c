#include "../internal.h"
#include <Gaia/gaia.h>

#include <assert.h>

GaiaArena *arena;

GAIA_API int gaia_arena_init() {
    arena = calloc(1, sizeof(*arena));
    
    *arena = (GaiaArena){0};

    if (!arena)
        return -1;
    arena->ptrs_capacity = 10;
    arena->ptrs = calloc(arena->ptrs_capacity, sizeof(GaiaPtr));
    if (!arena->ptrs)
        return -1;
    atexit(gaia_arena_destroy);
    at_quick_exit(gaia_arena_destroy);
    return 1;
}

GAIA_API void gaia_arena_set_debug(bool state) {
    arena->debug = state;
}

void gaia_arena_destroy() {
    assert(arena && "arena was not initialized");
    gaia_arena_print();
    free(arena->ptrs);
    free(arena);
}

void gaia_arena_ptr_push(GaiaPtr ptr) {
    assert(arena && "arena was not initialized");
    if(arena->allocations == arena->ptrs_capacity) {
        arena->ptrs_capacity += 10;
        arena->ptrs = realloc(arena->ptrs, arena->ptrs_capacity * sizeof(GaiaPtr));
    }
    arena->ptrs[arena->allocations] = ptr;
}

void gaia_arena_ptr_pop(void *ptr) {
    assert(arena && "arena was not initialized");
    for(size_t i = 0; i < arena->allocations; i++) {
        if(arena->ptrs[i].raw == ptr) {
            arena->ptrs[i] = arena->ptrs[arena->allocations - 1];
            arena->allocations--;
            return;
        }
    }
}

GAIA_API void gaia_arena_memory_free(void *_block, const char *file, int line) {
    assert(arena && "arena was not initialized");
    printf("freed memory [%p] at %s:%d\n", _block, file, line);
    gaia_arena_ptr_pop(_block);
    free(_block);
}

GAIA_API void *gaia_arena_memory_alloc(size_t _size, const char *file, int line) {
    assert(arena && "arena was not initialized");
    void *block = calloc(1, _size);
    printf("allocated memory [%p] at %s:%d\n", block, file, line);
    gaia_arena_ptr_push((GaiaPtr){block, file, line});
    arena->allocations++;
    arena->allocated_memory += _size;
    return block;
}

GAIA_API void gaia_arena_print() {
    assert(arena && "arena was not initialized");
    printf("remaining allocations --> %llu\n", arena->allocations);
    for(size_t i = 0; i < arena->allocations; i++)
        printf("[%p] allocated at %s:%d\n", arena->ptrs[i].raw, arena->ptrs[i].file, arena->ptrs[i].line);

    if(arena->allocations == 0) {
        printf("all allocated memory has been freed correctly\n");
    }else {
        printf("total number of bytes allocated: %llu\n", arena->allocated_memory);
    }
}
