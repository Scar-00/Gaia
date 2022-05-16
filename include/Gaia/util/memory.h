#ifndef _GAIA_MEMORY_H_
#define _GAIA_MEMORY_H_

#include "../types.h"
#include <Gaia/gaia.h>

#include <stdlib.h>

GAIA_API int gaia_arena_init();
GAIA_API void gaia_arena_memory_free(void *_block, const char *file, int line);
GAIA_API void *gaia_arena_memory_alloc(size_t _size, const char *file, int line);
GAIA_API void gaia_arena_print();

#ifdef GAIA_MEM_DEBUG
#define free(block) gaia_arena_memory_free(block, __FILE__, __LINE__)
#define malloc(size) gaia_arena_memory_alloc(size, __FILE__, __LINE__)
#define calloc(count, size) gaia_arena_memory_alloc(count * size, __FILE__, __LINE__)
#endif

#endif