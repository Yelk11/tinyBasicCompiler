#ifndef AST_H
#define AST_H

#include "token.h"




enum node_type
{
    PROGRAM,
    LINE,
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
    struct ast** child;
    int child_num;
    int child_cap;
    token* tok;
    enum node_type type;
}ast;

ast* init_node(enum node_type type, token* tok);

void ast_add_child(ast* parent, ast* child);

const char* node_type_to_string(enum node_type type);

void print_ast(ast* node, int indent);



#endif