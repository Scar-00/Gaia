#include <Gaia/gaia.h>
#include "../internal.h"

#include <assert.h>
#include <ctype.h>

// #define GAIA_DEBUG

typedef struct GaiaConfFileToken {
    GaiaToken type;
    String value;
}GaiaConfFileToken;

typedef struct GaiaConfFile_t {
    GaiaFile src;
    gaia_array(GaiaTable) tables;
}GaiaConfFile_t;

typedef struct GaiaEntryRequest {
    String requested_entry;
    enum GaiaRequestType {
        REQ_FIELD,
        REQ_SUFFIX,
    }type;
    String table_name;
}GaiaEntryRequest;

//---------------------------------------------//
//      internal function declarations         //
//---------------------------------------------//

gaia_array(GaiaConfFileToken) gaia_conf_file_lex(GaiaConfFile_t *file);
GaiaConfFileToken gaia_conf_file_next_token(GaiaLexer *lexer);
inline char *gaia_conf_token_to_string(GaiaToken type);
GaiaEntryRequest gaia_conf_file_parse_request(String request);

GAIA_API GaiaConfFile_t *gaia_conf_file_parse(GaiaFile file) {
    GaiaConfFile_t *self = calloc(1, sizeof(*self));
    *self = (GaiaConfFile_t){
        .src = file,
        .tables = gaia_array_create(GaiaTable, 2),
    };

    gaia_array(GaiaConfFileToken) tokens = gaia_conf_file_lex(self);

    GaiaAST root = gaia_ast_create(AST_ROOT);

    for(u32 i = 0; i < gaia_array_length(tokens); i++) {
        switch (tokens[i].type) {
        case TOKEN_BRACE_LEFT: {
            i++;
            String value = tokens[i].value;
            GaiaAST ast = gaia_ast_create(AST_TABLE);
            ast.name = value;
            i++;
            gaia_array_pushback(root.nodes, ast);
        }
        break;
        case TOKEN_STRING:
        case TOKEN_ID: {
            String value = tokens[i].value;
            i++;
            if(tokens[i].type == TOKEN_EQUALS) {
                GaiaAST ast = gaia_ast_create(AST_FIELD);
                ast.name = value;
                i++;
                ast.value = tokens[i].value;
                gaia_array_pushback(root.nodes, ast);
            }else {
                // gaia_panic("[%s] needs to have a value after '='\n", value.c_str);
                printf("[%s] needs to have a value after '='\n", value.c_str);
                exit(1);
            }
        }
        break;
        case TOKEN_ANGLE_LEFT: {
            parse_suffix:
            i++;
            if(tokens[i].type == TOKEN_ID) {
                String value = tokens[i].value;
                i++;
                if(tokens[i].type == TOKEN_EQUALS) {
                    GaiaAST ast = gaia_ast_create(AST_SUFFIX);
                    ast.name = value;
                    i++;
                    ast.value = tokens[i].value;
                    gaia_array_pushback(root.nodes, ast);
                }
                i++;
                if(tokens[i].type == TOKEN_COMMA) {
                    goto parse_suffix;
                }
            }
        }
        break;
        default: break;
        }
    }

    #ifdef GAIA_DEBUG
    for(u32 i = 0; i < gaia_array_length(tokens); i++) {
        printf("[%s] --> %s\n", gaia_conf_token_to_string(tokens[i].type), tokens[i].value.c_str);
    }

    printf("================================\n");
    for(u32 i = 0; i < gaia_array_length(root.nodes); i++) {
        switch (root.nodes[i].type) {
        case AST_TABLE:
            printf("[Table] = %s\n", root.nodes[i].name.c_str);
            break;
        case AST_FIELD:
            printf("%s = %s\n", root.nodes[i].name.c_str, root.nodes[i].value.c_str);
            break;
        case AST_SUFFIX:
            printf("<Suffix> %s = %s\n", root.nodes[i].name.c_str, root.nodes[i].value.c_str);
        default: break;
        }
    }
    #endif

    for(u32 i = 0; i < gaia_array_length(root.nodes); i++) {
        if(root.nodes[i].type == AST_TABLE) {
            GaiaTable table = {.name = root.nodes[i].name};
            table.suffixes = gaia_array_create(GaiaPair, 4);
            table.fields = gaia_array_create(GaiaPair, 4);

            i++;
            while(root.nodes[i].type == AST_SUFFIX) {
                gaia_array_pushback(table.suffixes, ((GaiaPair){.key = root.nodes[i].name, root.nodes[i].value}));
                i++;
            }
            while(root.nodes[i].type == AST_FIELD) {
                gaia_array_pushback(table.fields, ((GaiaPair){.key = root.nodes[i].name, root.nodes[i].value}));
                i++;
            }
            i--;
            gaia_array_pushback(self->tables, table);
        }
    }

    #ifdef GAIA_DEBUG
    printf("================================\n");
    for(u32 i = 0; i < gaia_array_length(self->tables); i++) {
        printf("%s", self->tables[i].name.c_str);
        for(u32 j = 0; j < gaia_array_length(self->tables[i].suffixes); j++) {
            printf(": ");
            printf("%s = %s | ", self->tables[i].suffixes[j].key.c_str, self->tables[i].suffixes[j].value.c_str);
        }
        printf("\n");
        for(u32 j = 0; j < gaia_array_length(self->tables[i].fields); j++) {
            printf("%s = %s\n", self->tables[i].fields[j].key.c_str, self->tables[i].fields[j].value.c_str);
        }
    }
    #endif

    gaia_array_destroy(root.nodes);
    gaia_array_destroy(tokens);
    return self;
}

GAIA_API GaiaConfFile_t *gaia_conf_file_parse_c(const char *path) {
    return gaia_conf_file_parse(gaia_file_read(path, "r"));
}

GAIA_API u32 gaia_conf_file_free(GaiaConfFile_t *file) {
    for(u32 i = 0; i < gaia_array_length(file->tables); i++) {
        gaia_array_destroy(file->tables[i].suffixes);
        gaia_array_destroy(file->tables[i].fields);
    }
    gaia_array_destroy(file->tables);
    gaia_file_close(&file->src);
    free(file);
    file = NULL;

    return 0;
}

GAIA_API String gaia_conf_file_get_entry(GaiaConfFile_t *file, const char *entry, ...) {
    va_list args;
    va_start(args, entry);
    String request_full = gaia_string_init_va(entry, args);
    va_end(args);

    GaiaEntryRequest requested_entry = gaia_conf_file_parse_request(request_full);
    for(u32 i = 0; i < gaia_array_length(file->tables); i++) {
        if(gaia_string_cmp(requested_entry.table_name, file->tables[i].name)) {
            switch (requested_entry.type) {
            case REQ_FIELD:
                for(u32 j = 0; j < gaia_array_length(file->tables[i].fields); j++) {
                    if(gaia_string_cmp(requested_entry.requested_entry, file->tables[i].fields[j].key)) {
                        return file->tables[i].fields[j].value;
                    }
                }
                fprintf(stderr, "[%s] has no field %s\n", requested_entry.table_name.c_str, requested_entry.requested_entry.c_str);
                exit(1);
            break;
            case REQ_SUFFIX:
                for(u32 j = 0; j < gaia_array_length(file->tables[i].suffixes); j++) {
                    if(gaia_string_cmp(requested_entry.requested_entry, file->tables[i].suffixes[j].key)) {
                        return file->tables[i].suffixes[j].value;
                    }
                }
                fprintf(stderr, "[%s] has no specifier %s\n", requested_entry.table_name.c_str, requested_entry.requested_entry.c_str);
                exit(1);
            break;
            }
        }
    }
    fprintf(stderr, "%s is not a valid request\n", requested_entry.requested_entry.c_str);
    exit(1);
}

GAIA_API GaiaTable gaia_conf_file_get_table(GaiaConfFile_t *file, const char *table_name) {
    gaia_array_foreach(file->tables, table) {
        if(gaia_string_cmp(table->name, gaia_string_init(table_name))) {
            return *table;
        }
    }
    fprintf(stderr, "%s does not exist\n", table_name);
    exit(1);
}

GAIA_API gaia_array(GaiaTable) gaia_conf_file_get_table_all(GaiaConfFile_t *file) {
    return file->tables;
}

GAIA_API bool gaia_conf_file_has_entry(GaiaConfFile_t *file, const char *entry, ...) {
    va_list args;
    va_start(args, entry);
    String request_full = gaia_string_init_va(entry, args);
    va_end(args);

    GaiaEntryRequest requested_entry = gaia_conf_file_parse_request(request_full);

    gaia_array_foreach(file->tables, table) {
        if(gaia_string_cmp(requested_entry.table_name, file->tables[i].name)) {
            switch(requested_entry.type) {
            case REQ_FIELD: {
                gaia_array_foreach(table->fields, field) {
                    if(gaia_string_cmp(requested_entry.requested_entry, field->key)) {
                        return 1;
                    }
                }
                return 0;
            }
            case REQ_SUFFIX: {
                gaia_array_foreach(table->suffixes, suffix) {
                    if(gaia_string_cmp(requested_entry.requested_entry, suffix->key)) {
                        return 1;
                    }
                }
                return 0;
            }
            }
        }
    }
    return 0;
}

GaiaEntryRequest gaia_conf_file_parse_request(String request) {
    if(gaia_string_contains(request, '/')) {
        String table_name = gaia_string_init("");
        for(u32 i = 0; request.c_str[i] != '/'; i++) {
            gaia_string_char_append(&table_name, request.c_str[i]);
        }

        if(gaia_string_cmp(table_name, gaia_string_init(""))) {
            fprintf(stderr, "%s is not a valid request\n", request.c_str);
            exit(1);
        }

        return (GaiaEntryRequest){.requested_entry = gaia_string_split_at(request, '/'), .type = REQ_FIELD, .table_name = table_name};
    }

    if(gaia_string_contains(request, '|')) {
        String table_name = gaia_string_init("");
        for(u32 i = 0; request.c_str[i] != '|'; i++) {
            gaia_string_char_append(&table_name, request.c_str[i]);
        }

        if(gaia_string_cmp(table_name, gaia_string_init(""))) {
            fprintf(stderr, "%s is not a valid request\n", request.c_str);
            exit(1);
        }

        return (GaiaEntryRequest){.requested_entry = gaia_string_split_at(request, '|'), .type = REQ_SUFFIX, .table_name = table_name};
    }

    fprintf(stderr, "%s is not a valid request\n", request.c_str);
    exit(1);
}

//lexical anaylisis
static GaiaConfFileToken gaia_cong_file_lex_id(GaiaLexer *lexer) {
    GaiaConfFileToken token = {.value = gaia_string_init(""), .type = TOKEN_ID};

    while(isalpha(lexer->curr)) {
        gaia_string_char_append(&token.value, lexer->curr);
        gaia_lexer_advance(lexer);
    }

    return token;
}

static GaiaConfFileToken gaia_cong_file_lex_int(GaiaLexer *lexer) {
    GaiaConfFileToken token = {.value = gaia_string_init(""), .type = TOKEN_INT};

    while(isdigit(lexer->curr)) {
        gaia_string_char_append(&token.value, lexer->curr);
        gaia_lexer_advance(lexer);
    }

    return token;
}

static GaiaConfFileToken gaia_cong_file_lex_string(GaiaLexer *lexer) {
    GaiaConfFileToken token = {.value = gaia_string_init(""), .type = TOKEN_STRING};

    // while (isalpha(lexer->curr) || lexer->curr == 13 || lexer->curr == 10 || lexer->curr == ' ' || lexer->curr == '\t') {
    while (iswascii(lexer->curr) && lexer->curr != '\"') {
        gaia_string_char_append(&token.value, lexer->curr);
        gaia_lexer_advance(lexer);
    }
    gaia_lexer_advance(lexer);
    return token;
}

static inline GaiaConfFileToken gaia_conf_file_token_init(const char *value, GaiaToken type) {
    return (GaiaConfFileToken){.value = gaia_string_init(value), .type = type};
}

inline char *gaia_conf_token_to_string(GaiaToken type) {
    static char *names[] = {
        "ID",
        "BRACE_LEFT",
        "BRACE_RIGHT",
        "ANGLE_LEFT",
        "ANGLE_RIGHT",
        "COMMA",
        "EQUALS",
        "STRING",
        "INT",
        "EOF",
    };

    return names[type];
}

//TODO: add rule that things in quotes are allways trated as one
gaia_array(GaiaConfFileToken) gaia_conf_file_lex(GaiaConfFile_t *file) {
    GaiaLexer lexer = gaia_lexer_init(file->src);
    GaiaConfFileToken *tokens = gaia_array_create(GaiaConfFileToken, 16);

    GaiaConfFileToken token;
    while((token = gaia_conf_file_next_token(&lexer)).type != TOKEN_EOF) {
        gaia_array_pushback(tokens, token);
    }

    return tokens;
}

GaiaConfFileToken gaia_conf_file_next_token(GaiaLexer *lexer) {
    while(lexer->curr != '\0') {
        while(lexer->curr == 13 || lexer->curr == 10 || lexer->curr == ' ' || lexer->curr == '\t')
            gaia_lexer_advance(lexer);

        if(isalpha(lexer->curr))
            return gaia_cong_file_lex_id(lexer);

        if(isdigit(lexer->curr))
            return gaia_cong_file_lex_int(lexer);

        switch (lexer->curr) {
        case '=':
            gaia_lexer_advance(lexer);
            return gaia_conf_file_token_init("=", TOKEN_EQUALS);
            break;
        case '[':
            gaia_lexer_advance(lexer);
            return gaia_conf_file_token_init("[", TOKEN_BRACE_LEFT);
            break;
        case ']':
            gaia_lexer_advance(lexer);
            return gaia_conf_file_token_init("]", TOKEN_BRACE_RIGHT);
            break;
        case '<':
            gaia_lexer_advance(lexer);
            return gaia_conf_file_token_init("<", TOKEN_ANGLE_LEFT);
            break;
        case '>':
            gaia_lexer_advance(lexer);
            return gaia_conf_file_token_init(">", TOKEN_ANGLE_RIGHT);
            break;
        case ',':
            gaia_lexer_advance(lexer);
            return gaia_conf_file_token_init(",", TOKEN_COMMA);
            break;
        case '\"':
            gaia_lexer_advance(lexer);
            return gaia_cong_file_lex_string(lexer);
            break;
        case '\0': break;
        default: 
            printf("[%c] is not a valid token\n", lexer->curr);
            exit(1);
            break;
        }
    }
    return gaia_conf_file_token_init("", TOKEN_EOF);
}