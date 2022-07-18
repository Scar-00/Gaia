#ifndef _GAIA_PARSE_H_
#define _GAIA_PARSE_H_

#include <Gaia/gaia.h>

typedef struct GaiaConfFile_t *GaiaConfFile; 

GAIA_API GaiaConfFile gaia_conf_file_parse(GaiaFile file);
GAIA_API GaiaConfFile gaia_conf_file_parse_c(const char *path);
GAIA_API u32 gaia_conf_file_free(GaiaConfFile file);
GAIA_API String gaia_conf_file_get_entry(GaiaConfFile file, const char *entry, ...);
GAIA_API bool gaia_conf_file_has_entry(GaiaConfFile file, const char *entry, ...);

#endif
