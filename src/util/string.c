#include <Gaia/gaia.h>
#include <assert.h>

GAIA_API String gaia_string_init(const char *format, ...) {
    va_list args;
    va_start(args, format);
    String string = gaia_string_init_va(format, args);
    va_end(args);
    return string;
}

GAIA_API String gaia_string_init_va(const char *format, va_list args) {
    String string = {0};
    string.length = vsnprintf(NULL, 0, format, args);

    //TODO: check if +1 is needed
    assert(string.length + 1 < 64 && "String to long");

    vsprintf_s(string.c_str, string.length + 1, format, args);

    return string;
}

GAIA_API u32 gaia_string_cmp(const String str0, const String str1) {
    if(str0.length != str1.length)
        return 0;

    for(u32 i = 0; i < str0.length; i++) {
        if(str0.c_str[i] != str1.c_str[i])
            return 0;
    }

    return 1;
}

GAIA_API void gaia_string_char_append(String *string, char c) {
    assert(string->length <= 62 && "cannot append character to max size 'String'");
    strcat_s(string->c_str, string->length + 2, (char[]) {c, '\0'});
    string->length++;
}

GAIA_API void gaia_string_append(String *dest, String *src) {
    size_t size_new = dest->length + src->length; 
    assert(size_new <= 64 && "cannot append 'String' to max size 'String'");
    strcat_s(dest->c_str, size_new, src->c_str);
    dest->length = size_new;
}

GAIA_API StringStream gaia_stringstream_init(const char *format, ...) {
    va_list args;
    va_start(args, format);
    StringStream stream = gaia_stringstream_init_va(format, args);
    va_end(args);
    return stream;
}

GAIA_API StringStream gaia_stringstream_init_va(const char *format, va_list args) {
    StringStream stream = {
        .length = vsnprintf(NULL, 0, format, args),
    };

    stream.c_str = malloc(stream.length + 1);

    vsprintf_s(stream.c_str, stream.length + 1, format, args);

    return stream;
}

GAIA_API void gaia_stringstream_destroy(StringStream *stream) {
    free(stream->c_str);
    stream->length = -1;
}

GAIA_API u32 gaia_stringstream_cmp(const StringStream stream0, const StringStream stream1) {
    if(stream0.length != stream1.length)
        return 0;

    for(u32 i = 0; i < stream0.length; i++) {
        if(stream0.c_str[i] != stream1.c_str[i])
            return 0;
    }

    return 1;
}

GAIA_API void gaia_stringstream_char_append(StringStream *stream, char c) {
    assert(0 && "FIXME");
    stream->c_str = realloc(stream->c_str, stream->length + 1);
    stream->length++;
    strcat_s(stream->c_str, stream->length, &c);
}

GAIA_API void gaia_stringstream_append(StringStream *dest, StringStream *src) {
    assert(0 && "Not implemented");
}





GAIA_API String gaia_stringstream_to_string(StringStream stream) {
    assert(stream.length <= 64 && "cannot convert 'StringStream' to 'String' that is longer then 64 characters");
    return gaia_string_init(stream.c_str);
}

GAIA_API StringStream gaia_string_to_stringstream(String string) {
    return gaia_stringstream_init(string.c_str);
}