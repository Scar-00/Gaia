#ifndef _GAIA_STRING_H_
#define _GAIA_STRING_H_

#include "Gaia/util/string.h"
#include <Gaia/gaia.h>

#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//string type for strings shorter then 64 characters
typedef struct String_t {
    char c_str[64];
    size_t length;
}String;

GAIA_API String gaia_string_init(const char *format, ...);
GAIA_API String gaia_string_init_va(const char *format, va_list args);
GAIA_API u32 gaia_string_cmp(const String str0, const String str1); 
GAIA_API void gaia_string_char_append(String *string, char c);
GAIA_API void gaia_string_append(String *dest, String *src);
GAIA_API String gaia_string_split_at(String str, char c);
GAIA_API bool gaia_string_contains(String str, char c); 

//string type for strings longer then 64 characters
typedef struct StringStream_t {
    char *c_str;
    size_t length;
}StringStream;

GAIA_API StringStream gaia_stringstream_init(const char *format, ...);
GAIA_API StringStream gaia_stringstream_init_va(const char *format, va_list args);
GAIA_API void gaia_stringstream_destroy(StringStream *stream);
GAIA_API u32 gaia_stringstream_cmp(const StringStream stream0, const StringStream stream1);
GAIA_API void gaia_stringstream_char_append(StringStream *stream, char c);
GAIA_API void gaia_stringstream_append(StringStream *dest, StringStream *src);
GAIA_API StringStream gaia_stringstream_split_at(StringStream stream, char c);
GAIA_API bool gaia_stringstream_contains(StringStream stream, char c);


GAIA_API String gaia_stringstream_to_string(StringStream stream);
GAIA_API StringStream gaia_string_to_stringstream(String string);

/*
typedef union GaiaString {
    struct {
        size_t length;
        size_t capacity;
        void *c_str;
    };
    struct {
        size_t length;
        char c_str[16];
    };
}GaiaString;
*/

#endif
