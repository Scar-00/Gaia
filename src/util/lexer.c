#include "../internal.h"

GaiaLexer gaia_lexer_init(GaiaFile file) {
    return (GaiaLexer){
        .src = file,
        .curr = file.data.c_str[0],
        .pos = 0,
    };
}

void gaia_lexer_advance(GaiaLexer *self) {
    self->pos++;
    self->curr = self->src.data.c_str[self->pos];
}