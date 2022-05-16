#include <Gaia/gaia.h>
#include <io.h>

GAIA_API GaiaFile gaia_file_read(const char *path, const char *mode) {
    GaiaFile file = {.path = path, .mode = mode};
    fopen_s(&file.file, file.path, file.mode);

    size_t file_size = -1;

    if(file.file == NULL)
        fprintf(stderr, "Failed to open file: %s\n", file.path);

    fseek(file.file, 0, SEEK_END);
    file_size = ftell(file.file);

    if(file_size < 0)
        fprintf(stderr, "Failed to get file size: %s\n", file.path);

    rewind(file.file);
    char *file_data = calloc(file_size + 1, sizeof(char));
    fread(file_data, sizeof(char), file_size, file.file);
    if(!file_data)
        fprintf(stderr, "Failed to read file: %s\n", file.path);

    file.data = gaia_stringstream_init(file_data);

    return file;
}

GAIA_API u32 gaia_file_check(const char *path, u32 mode) {
    return _access_s(path, mode);
}

GAIA_API void gaia_file_close(GaiaFile *file) {
    gaia_stringstream_destroy(&file->data);

    if(fclose(file->file)) {
        fprintf(stderr, "Could not close file: %s\n", file->path);
    }
}