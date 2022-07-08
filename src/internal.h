#ifndef _GAIA_INTERNAL_H_
#define _GAIA_INTERNAL_H_

#include <Gaia/gaia.h>

//stdlibs
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct GaiaPtr {
    void *raw;
    const char *file;
    int line;
}GaiaPtr;

typedef struct GaiaArena {
    size_t allocations;
    size_t allocated_memory;
    GaiaPtr *ptrs;
    size_t ptrs_capacity;
    bool debug;
}GaiaArena;

void gaia_arena_destroy();
void gaia_arena_ptr_push(GaiaPtr ptr);
//FIXME: remame to gaia_arena_ptr_drop
void gaia_arena_ptr_pop(void *ptr);

typedef struct GaiaEntry {
    String name;
    String value;
}GaiaEntry;

//FIXME: refactor using gaia array/stack
typedef struct GaiaSaveFile {
    GaiaFile file;
    GaiaEntry *entries;
    size_t entries_count;
    size_t entries_size;
}GaiaSaveFile;

GaiaSaveFile *gaia_save_file_init(GaiaFile file);
void gaia_save_file_lex();
inline GaiaEntry gaia_entry_init(String name, String value);
inline void gaia_save_file_entry_add(GaiaEntry entry);
GaiaEntry *gaia_save_file_entry_get(String name);

typedef enum TOMLTypes{
    TOML_VALUE,
    TOML_STRING,
    TOML_INT,
    TOML_FLOAT,
    TOML_BOOL,
}TOMLTypes;



#endif
