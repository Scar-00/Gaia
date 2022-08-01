#ifndef _GAIA_IO_H_
#define _GAIA_IO_H_

#include <Gaia/gaia.h>

#include <stdio.h>

typedef struct GaiaFile {
    FILE *file;
    const char *mode;
    const char *path;
    StringStream data;
}GaiaFile;

GAIA_API GaiaFile gaia_file_read(const char *path, const char *mode);
GAIA_API void gaia_file_close(GaiaFile *file);
GAIA_API u32 gaia_file_check(const char *path, u32 mode);
GAIA_API void gaia_file_reopen(GaiaFile *file, const char *mode);

// care slow operation
GAIA_API void gaia_setting_file_set(const char *path);
//writing to file also closes the current file and it can no longet be used
GAIA_API void gaia_setting_write();
GAIA_API u32 gaia_setting_u32_get(const char *name, u32 value_default);
GAIA_API void gaia_setting_u32_set(const char *name, u32 value);
GAIA_API s32 gaia_setting_s32_get(const char *name, s32 value_default);
GAIA_API void gaia_setting_s32_set(const char *name, s32 value);
GAIA_API f32 gaia_setting_f32_get(const char *name, f32 value_default);
GAIA_API void gaia_setting_f32_set(const char *name, f32 value);
GAIA_API String gaia_setting_string_get(const char *name, String value_default);
GAIA_API void gaia_setting_string_set(const char *name, String value);

#endif
