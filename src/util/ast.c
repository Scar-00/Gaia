#include <Gaia/gaia.h>
#include "../internal.h"

GaiaAST gaia_ast_create(GaiaType type) {
    GaiaAST ast = {.type = type};
    
    if(ast.type == AST_ROOT)
        ast.nodes = gaia_array_create(GaiaAST, 4);
    
    return ast;
}