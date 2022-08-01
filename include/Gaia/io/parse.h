#ifndef _GAIA_PARSE_H_
#define _GAIA_PARSE_H_

#include <Gaia/gaia.h>

typedef struct GaiaConfFile_t *GaiaConfFile; 

typedef struct GaiaPair {
    String key;
    String value;
}GaiaPair;

typedef struct GaiaTable {
    String name;
    gaia_array(GaiaPair) suffixes;
    gaia_array(GaiaPair) fields;
}GaiaTable;

GAIA_API GaiaConfFile gaia_conf_file_parse(GaiaFile file);
GAIA_API GaiaConfFile gaia_conf_file_parse_c(const char *path);
GAIA_API u32 gaia_conf_file_free(GaiaConfFile file);
GAIA_API String gaia_conf_file_get_entry(GaiaConfFile file, const char *entry, ...);
GAIA_API GaiaTable gaia_conf_file_get_table(GaiaConfFile file, const char *entry);
GAIA_API gaia_array(GaiaTable) gaia_conf_file_get_table_all(GaiaConfFile file);
GAIA_API bool gaia_conf_file_has_entry(GaiaConfFile file, const char *entry, ...);

#endif