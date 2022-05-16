#include <Gaia/gaia.h>
#include "../internal.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

GaiaSaveFile *file_current = NULL;

//FIXME: use maybe hashmap or something for entry indexing way to slow to loop for entry

GaiaSaveFile *gaia_save_file_init(GaiaFile file) {
    GaiaSaveFile *self = malloc(sizeof(GaiaSaveFile));
    memset(self, 0, sizeof(GaiaSaveFile));
    self->file = file;
    self->entries_count = 0;
    self->entries_size = 10;
    self->entries = malloc(self->entries_size * sizeof(GaiaEntry));
    return self;
}

void gaia_save_file_lex() {
    assert(file_current && "no save file set");
    const char *data = file_current->file.data.c_str;
    String name = gaia_string_init("name");
    String value = gaia_string_init("value");
    for(s32 i = 0; i < (s32)file_current->file.data.length; i++) {
        if(data[i - 1] == '[') {
            name = gaia_string_init("");
            while(data[i] != ']') {
                gaia_string_char_append(&name, data[i]);
                i++;
            }
        }

        if(data[i - 1] == '<') {
            value = gaia_string_init("");
            while(data[i] != '>') {
                gaia_string_char_append(&value, data[i]);
                i++;
            }
            gaia_save_file_entry_add(gaia_entry_init(name, value));
        }
    }
}

inline GaiaEntry gaia_entry_init(String name, String value) {
    return (GaiaEntry){.name = name, .value = value};
}

inline void gaia_save_file_entry_add(GaiaEntry entry) {
    if(file_current->entries_count == file_current->entries_size) {
        file_current->entries_size += 10;
        realloc(file_current->entries, file_current->entries_size * sizeof(GaiaEntry));
    }

    file_current->entries[file_current->entries_count++] = entry;
}

GaiaEntry *gaia_save_file_entry_get(String name) {
    for(u32 i = 0; i < file_current->entries_count; i++) {
        if(gaia_string_cmp(file_current->entries[i].name, name)) {
            return &file_current->entries[i];
        }
    }
    return NULL;
}

GAIA_API void gaia_setting_file_set(const char *path) {
    atexit(gaia_setting_write);
    if(file_current) {
        gaia_file_close(&file_current->file);
        free(file_current->entries);
        free(file_current);
        file_current = NULL;
    }
    file_current = gaia_save_file_init(gaia_file_read(path, "r+"));
    gaia_save_file_lex();
}

GAIA_API void gaia_setting_write() {
    const char *path = file_current->file.path;

    gaia_file_close(&file_current->file);
    file_current->file = gaia_file_read(path, "w+");

    for(u32 i = 0; i < file_current->entries_count; i++) {
        fprintf(file_current->file.file, "[%s] = <%s>\n", file_current->entries[i].name.c_str, file_current->entries[i].value.c_str);
    }

    gaia_file_close(&file_current->file);
    free(file_current->entries);
    free(file_current);
    file_current = NULL;
}

GAIA_API u32 gaia_setting_u32_get(const char *name, u32 value_default) {
    assert(file_current && "No save file set call 'gaia_setting_file_set' to set a file");
    String name_string = gaia_string_init(name);
    for(u32 i = 0; i < file_current->entries_count; i++) {
        if(gaia_string_cmp(file_current->entries[i].name, name_string)) {
            return (u32)atoi(file_current->entries[i].value.c_str);
        }
    }
    gaia_save_file_entry_add(gaia_entry_init(name_string, gaia_string_init("%u", value_default)));
    return value_default;
}

GAIA_API void gaia_setting_u32_set(const char *name, u32 value) {
    assert(file_current && "No save file set call 'gaia_setting_file_set' to set a file");
    GaiaEntry *entry = gaia_save_file_entry_get(gaia_string_init(name));
    if(entry) {
        entry->value = gaia_string_init("%u", value);
    }else {
        fprintf(stderr, "No entry with name [%s] found", name);
    }
}

GAIA_API s32 gaia_setting_s32_get(const char *name, s32 value_default) {
    assert(file_current && "No save file set call 'gaia_setting_file_set' to set a file");
    String name_string = gaia_string_init(name);
    for(u32 i = 0; i < file_current->entries_count; i++) {
        if(gaia_string_cmp(file_current->entries[i].name, name_string)) {
            return (s32)atoi(file_current->entries[i].value.c_str);
        }
    }
    gaia_save_file_entry_add(gaia_entry_init(name_string, gaia_string_init("%d", value_default)));
    return value_default;
}

GAIA_API void gaia_setting_s32_set(const char *name, s32 value) {
    assert(file_current && "No save file set call 'gaia_setting_file_set' to set a file");
    GaiaEntry *entry = gaia_save_file_entry_get(gaia_string_init(name));
    if(entry) {
        entry->value = gaia_string_init("%d", value);
    }else {
        fprintf(stderr, "No entry with name [%s] found", name);
    }
}

GAIA_API f32 gaia_setting_f32_get(const char *name, f32 value_default) {
    assert(file_current && "No save file set call 'gaia_setting_file_set' to set a file");
    String name_string = gaia_string_init(name);
    for(u32 i = 0; i < file_current->entries_count; i++) {
        if(gaia_string_cmp(file_current->entries[i].name, name_string)) {
            return (f32)atof(file_current->entries[i].value.c_str);
        }
    }
    gaia_save_file_entry_add(gaia_entry_init(name_string, gaia_string_init("%f", value_default)));
    return value_default;
}

GAIA_API void gaia_setting_f32_set(const char *name, f32 value) {
    assert(file_current && "No save file set call 'gaia_setting_file_set' to set a file");
    GaiaEntry *entry = gaia_save_file_entry_get(gaia_string_init(name));
    if(entry) {
        entry->value = gaia_string_init("%f", value);
    }else {
        fprintf(stderr, "No entry with name [%s] found", name);
    }
}

GAIA_API String gaia_setting_string_get(const char *name, String value_default) {
    assert(file_current && "No save file set call 'gaia_setting_file_set' to set a file");
    String name_string = gaia_string_init(name);
    for(u32 i = 0; i < file_current->entries_count; i++) {
        if(gaia_string_cmp(file_current->entries[i].name, name_string)) {
            return file_current->entries[i].value;
        }
    }
    gaia_save_file_entry_add(gaia_entry_init(name_string, value_default));
    return value_default;
}

GAIA_API void gaia_setting_string_set(const char *name, String value) {
    assert(file_current && "No save file set call 'gaia_setting_file_set' to set a file");
    GaiaEntry *entry = gaia_save_file_entry_get(gaia_string_init(name));
    if(entry) {
        entry->value = value;
    }else {
        fprintf(stderr, "No entry with name [%s] found", name);
    }
}