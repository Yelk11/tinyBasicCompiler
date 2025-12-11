#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "parse.h"
#include "lex.h"
#include "token.h"
#include "ast.h"

// --- helper functions ---
token *peek(lexer *lex) { return lexer_peek_next_token(lex); }
token *next(lexer *lex) { return next_token(lex); }

token *expect(lexer *lex, int type)
{
    token *t = next_token(lex); // consume the next token from the lexer
    if (!t || t->type != type)
    { // check if the token exists and is the expected type
        fprintf(stderr, "Parse error: expected token type %d, got %d\n",
                type, t ? t->type : -1);
        exit(1); // abort parsing if the token is not what we expect
    }
    return t; // return the token if it's correct
}

int is_keyword(token *t, const char *kw)
{
    return t->type == TOKEN_KEYWORD && strcmp(t->value, kw) == 0;
}

// --- forward declarations ---

// --- entry point ---
ast *parse(lexer *lex)
{
    return parse_program(lex);
}

// --- program ::= { line } ---
ast *parse_program(lexer *lex)
{
    ast *prog = init_node(PROGRAM, NULL);

    while (peek(lex)->type != TOKEN_EOF)
    {
        ast *ln = parse_line(lex);
        ast_add_child(prog, ln);

        if (peek(lex)->type == TOKEN_EOL)
            next(lex);
    }

    return prog;
}

// --- line ::= LINE_NUM statement ---
ast *parse_line(lexer *lex)
{
    // Consume the line number token
    token *lineTok = expect(lex, TOKEN_LINE_NUM);

    // Create a LINE node with the line number as its token
    ast *lineNode = init_node(LINE, lineTok);

    // Parse the statement that follows
    ast *stmt = parse_statement(lex);
    ast_add_child(lineNode, stmt);

    return lineNode;
}

// --- statement dispatcher ---
ast *parse_statement(lexer *lex)
{
    token *t = peek(lex);

    if (t->type == TOKEN_KEYWORD)
    {
        if (is_keyword(t, "LET"))
            return parse_let(lex);
        if (is_keyword(t, "PRINT"))
            return parse_print(lex);
        if (is_keyword(t, "INPUT"))
            return parse_input(lex);
        if (is_keyword(t, "GOTO"))
            return parse_goto(lex);
        if (is_keyword(t, "IF"))
            return parse_if(lex);
        if (is_keyword(t, "GOSUB"))
            return parse_gosub(lex);
        if (is_keyword(t, "RETURN"))
            return parse_return_stmt(lex);
        if (is_keyword(t, "END"))
            return parse_end_stmt(lex);
        if (is_keyword(t, "REM"))
            return parse_rem_stmt(lex);
    }

    // implicit LET if line starts with identifier
    if (t->type == TOKEN_IDENTIFIER)
        return parse_let(lex);

    fprintf(stderr, "Unknown statement token '%s'\n", t->value);
    exit(1);
}

// --- LET ::= ("LET")? IDENT "=" expr ---
ast *parse_let(lexer *lex)
{
    token *t = peek(lex);
    if (is_keyword(t, "LET"))
        next(lex);

    token *var = expect(lex, TOKEN_IDENTIFIER);
    token *eq = expect(lex, TOKEN_OPERATOR);
    if (strcmp(eq->value, "=") != 0)
    {
        fprintf(stderr, "Expected '=' in LET statement\n");
        exit(1);
    }

    ast *node = init_node(LET_STATEMENT, NULL);
    ast_add_child(node, init_node(EXPRESSION, var));
    ast_add_child(node, parse_expression(lex));
    return node;
}

// --- PRINT ::= "PRINT" expr-or-string {","|";" expr-or-string} ---
ast *parse_print(lexer *lex)
{
    next(lex); // consume PRINT
    ast *node = init_node(PRINT_STATEMENT, NULL);
    ast_add_child(node, parse_expression(lex));

    while (peek(lex)->type == TOKEN_PUNCTUATION &&
           (strcmp(peek(lex)->value, ",") == 0 || strcmp(peek(lex)->value, ";") == 0))
    {
        next(lex); // consume separator
        ast_add_child(node, parse_expression(lex));
    }

    return node;
}

// --- INPUT ::= "INPUT" IDENT ---
ast *parse_input(lexer *lex)
{
    next(lex);
    token *id = expect(lex, TOKEN_IDENTIFIER);
    return init_node(INPUT_STATEMENT, id);
}

// --- GOTO ::= "GOTO" NUMBER ---
ast *parse_goto(lexer *lex)
{
    next(lex);
    token *num = expect(lex, TOKEN_NUMBER);
    return init_node(GO_TO_STATEMENT, num);
}

// --- IF ::= "IF" expr OP expr THEN NUMBER ---
ast *parse_if(lexer *lex)
{
    next(lex); // IF
    ast *node = init_node(IF_STATEMENT, NULL);

    ast_add_child(node, parse_expression(lex));

    token *op = expect(lex, TOKEN_OPERATOR);
    ast_add_child(node, init_node(EXPRESSION, op));

    ast_add_child(node, parse_expression(lex));

    token *thenTok = expect(lex, TOKEN_KEYWORD);
    if (!is_keyword(thenTok, "THEN"))
    {
        fprintf(stderr, "Expected THEN in IF statement\n");
        exit(1);
    }

    token *num = expect(lex, TOKEN_NUMBER);
    ast_add_child(node, init_node(EXPRESSION, num));

    return node;
}

// --- GOSUB ::= "GOSUB" NUMBER ---
ast *parse_gosub(lexer *lex)
{
    next(lex);
    token *num = expect(lex, TOKEN_NUMBER);
    return init_node(GO_SUB_STATEMENT, num);
}

// --- RETURN ::= "RETURN" ---
ast *parse_return_stmt(lexer *lex)
{
    next(lex);
    return init_node(RETURN_STATEMENT, NULL);
}

// --- END ::= "END" ---
ast *parse_end_stmt(lexer *lex)
{
    next(lex);
    return init_node(END_STATEMENT, NULL);
}

// --- REM ::= "REM" <rest of line> ---
ast *parse_rem_stmt(lexer *lex)
{
    token *rem = next(lex); // consume REM
    ast *node = init_node(STRING_LITERAL, rem);

    // consume rest of line as string tokens until EOL
    while (peek(lex)->type != TOKEN_EOL && peek(lex)->type != TOKEN_EOF)
        next(lex);

    return node;
}

// --- Expressions ---
ast *parse_expression(lexer *lex)
{
    ast *left = parse_term(lex);

    while (peek(lex)->type == TOKEN_OPERATOR &&
           (strcmp(peek(lex)->value, "+") == 0 || strcmp(peek(lex)->value, "-") == 0))
    {
        token *op = next(lex);
        ast *right = parse_term(lex);

        ast *node = init_node(EXPRESSION, op);
        ast_add_child(node, left);
        ast_add_child(node, right);
        left = node;
    }

    return left;
}

ast *parse_term(lexer *lex)
{
    ast *left = parse_factor(lex);

    while (peek(lex)->type == TOKEN_OPERATOR &&
           (strcmp(peek(lex)->value, "*") == 0 || strcmp(peek(lex)->value, "/") == 0))
    {
        token *op = next(lex);
        ast *right = parse_factor(lex);

        ast *node = init_node(EXPRESSION, op);
        ast_add_child(node, left);
        ast_add_child(node, right);
        left = node;
    }

    return left;
}

ast *parse_factor(lexer *lex)
{
    token *t = peek(lex);

    if (t->type == TOKEN_NUMBER)
    {
        next(lex);
        return init_node(EXPRESSION, t);
    }
    if (t->type == TOKEN_IDENTIFIER)
    {
        next(lex);
        return init_node(EXPRESSION, t);
    }
    if (t->type == TOKEN_STRING)
    {
        next(lex);
        return init_node(STRING_LITERAL, t);
    }

    if (t->type == TOKEN_PUNCTUATION && strcmp(t->value, "(") == 0)
    {
        next(lex);
        ast *e = parse_expression(lex);
        token *rp = expect(lex, TOKEN_PUNCTUATION);
        if (strcmp(rp->value, ")") != 0)
        {
            fprintf(stderr, "Expected ')'\n");
            exit(1);
        }
        return e;
    }

    fprintf(stderr, "Unexpected token '%s' in factor\n", t->value);
    exit(1);
}
