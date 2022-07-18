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

typedef enum GaiaToken {
    TOKEN_ID,
    TOKEN_BRACE_LEFT,
    TOKEN_BRACE_RIGHT,
    TOKEN_ANGLE_LEFT,
    TOKEN_ANGLE_RIGHT,
    TOKEN_COMMA,
    TOKEN_EQUALS,
    TOKEN_STRING,
    TOKEN_INT,
    TOKEN_EOF,
}GaiaToken;

typedef struct GaiaLexer {
    GaiaFile src;
    char curr;
    u64 pos;
}GaiaLexer;

GaiaLexer gaia_lexer_init(GaiaFile file);
void gaia_lexer_advance(GaiaLexer *self);

typedef enum GaiaType{
    AST_ROOT,
    AST_FIELD,
    AST_TABLE, 
    AST_SUFFIX, //-> [name] [type, ..., ...]
}GaiaType;

typedef struct GaiaAST {
    GaiaType type;
    String name;
    String value;
    gaia_array(struct GaiaAST) nodes;
}GaiaAST;

GaiaAST gaia_ast_create(GaiaType type);

typedef struct GaiaPair {
    String key;
    String value;
}GaiaPair;

typedef struct GaiaTable {
    String name;
    gaia_array(GaiaPair) suffixes;
    gaia_array(GaiaPair) fields;
}GaiaTable;

#endif
