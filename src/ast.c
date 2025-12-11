#include <stdlib.h>
#include <string.h>
#include "ast.h"

#define INITIAL_CHILD_CAP 4

ast* init_node(enum node_type type, token* tok)
{
    ast* n = calloc(1, sizeof(ast));
    n->type = type;
    n->tok = tok;
    n->child_num = 0;
    n->child_cap = INITIAL_CHILD_CAP;
    n->child = calloc(n->child_cap, sizeof(ast*));
    return n;
}

void ast_add_child(ast* parent, ast* child)
{
    if (parent->child_num >= parent->child_cap) {
        parent->child_cap *= 2;
        parent->child = realloc(parent->child,
                                parent->child_cap * sizeof(ast*));
        if (!parent->child) {
            exit(1);
        }
    }

    parent->child[parent->child_num++] = child;
}
