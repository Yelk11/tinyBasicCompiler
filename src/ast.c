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
    return n;
}

void ast_add_child(ast* parent, ast* child)
{
    if (!parent || !child)
        return;

    if (!parent->child) {
        parent->child = child;
        return;
    }

    ast* c = parent->child;
    while (c->sibling)
        c = c->sibling;

    c->sibling = child;
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

    // indent
    for (int i = 0; i < indent; i++)
        printf("  ");

    // print node type
    printf("%s", node_type_to_string(node->type));

    // print token if present
    if (node->tok && node->tok->value)
        printf(" (%s)", node->tok->value);

    printf("\n");

    // print children
    print_ast(node->child, indent + 1);

    // print siblings at same indent
    print_ast(node->sibling, indent);
}
