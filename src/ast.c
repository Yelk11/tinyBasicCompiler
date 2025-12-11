#include <stdio.h>
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



// Utility: convert node type enum to string
const char* node_type_to_string(enum node_type type) 
{
    switch(type) {
        case PROGRAM: return "PROGRAM";
        case LINE: return "LINE";
        case LET_STATEMENT: return "LET";
        case PRINT_STATEMENT: return "PRINT";
        case IF_STATEMENT: return "IF";
        case GO_TO_STATEMENT: return "GOTO";
        case GO_SUB_STATEMENT: return "GOSUB";
        case RETURN_STATEMENT: return "RETURN";
        case END_STATEMENT: return "END";
        case INPUT_STATEMENT: return "INPUT";
        case EXPRESSION: return "EXPR";
        case STRING_LITERAL: return "STRING";
        default: return "UNKNOWN";
    }
}

// Recursive function to print AST
void print_ast(ast* node, int indent)
{
    if (!node) return;

    for (int i = 0; i < indent; i++) printf("  "); // indentation

    printf("%s", node_type_to_string(node->type));

    if (node->tok && node->tok->value)
        printf(" [%s]", node->tok->value);

    printf("\n");

    for (int i = 0; i < node->child_num; i++) {
        print_ast(node->child[i], indent + 1);
    }
}
