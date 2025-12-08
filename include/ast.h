#ifndef AST_H
#define AST_H

#include "token.h"




enum node_type
{
    PROGRAM,
    LET_STATEMENT,
    PRINT_STATEMENT,
    IF_STATEMENT,
    GO_TO_STATEMENT,
    GO_SUB_STATEMENT,
    RETURN_STATEMENT,
    END_STATEMENT,
    INPUT_STATEMENT,
    EXPRESSION,
    STRING_LITERAL
};
typedef struct ast{
    struct ast* right;
    struct ast* left;
    token* tok;
    enum node_type type;
}ast;

ast *init_node(enum node_type type, token* token);

int expect(token* tok);


#endif