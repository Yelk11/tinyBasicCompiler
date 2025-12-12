#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "parse.h"
#include "lex.h"
#include "token.h"
#include "ast.h"

// ---------------- Helper functions ----------------
static token *peek(lexer *lex) { return lexer_peek_next_token(lex); }
static token *next(lexer *lex) { return next_token(lex); }

static token *expect(lexer *lex, int type)
{
    token *t = next(lex);
    if (!t || t->type != type)
    {
        fprintf(stderr, "Parse error: expected token type %d, got %d\n",
                type, t ? t->type : -1);
        printf("line: %d, col: %d\n", lex->line, lex->col);
        exit(1);
    }
    return t;
}

static int is_keyword(token *t, const char *kw)
{
    return t && t->type == TOKEN_KEYWORD && strcasecmp(t->value, kw) == 0;
}

// program     ::= { line }
ast *parse(lexer *lex)
{
    ast *prog = init_node(PROGRAM, NULL);
    while (peek(lex)->type != TOKEN_EOF)
    {
        ast_add_child(prog, parse_line(lex));

        // consume EOL after each line
        token *t = peek(lex);
        if (t && t->type == TOKEN_EOL)
            next(lex);
    }

    return prog;
}

// line        ::= number statement
ast *parse_line(lexer *lex)
{
    token *lineNum = expect(lex, TOKEN_LINE_NUM);
    ast *lineNode = init_node(LINE, lineNum);

    ast *stmt = parse_statement(lex);
    ast_add_child(lineNode, stmt);
    return lineNode;
}

/*
statement   ::= let-stmt
              | print-stmt
              | input-stmt
              | goto-stmt
              | if-stmt
              | gosub-stmt
              | return-stmt
              | end-stmt
              | rem-stmt
*/
ast *parse_statement(lexer *lex)
{
    token *t = peek(lex);

    if (!t)
    {
        fprintf(stderr, "Unexpected end of input\n");
        exit(1);
    }

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

    if (t->type == TOKEN_IDENTIFIER)
        return parse_let(lex);

    fprintf(stderr, "Unknown statement token '%s'\n", t->value);
    exit(1);
}


// let-stmt    ::= (LET)? var '=' expr
ast *parse_let(lexer *lex)
{
    if (is_keyword(peek(lex), "LET"))
        next(lex);

    token *var = expect(lex, TOKEN_IDENTIFIER);

    token *eq = expect(lex, TOKEN_OPERATOR);
    if (strcmp(eq->value, "=") != 0) {
        fprintf(stderr, "Expected '=' in LET statement\n");
        exit(1);
    }

    ast *exprNode = parse_expression(lex);

    ast *node = init_node(LET_STATEMENT, NULL);
    ast* eq_node = init_node(EXPRESSION, eq);
    ast_add_child(node, eq_node);   // '=' operator node
    ast_add_child(eq_node, init_node(EXPRESSION, var));  // var
    ast_add_child(eq_node, exprNode);                    // RHS expression

    return node;
}


// print-stmt  ::= PRINT print-list
ast *parse_print(lexer *lex)
{
    next(lex); // consume PRINT
    ast *node = init_node(PRINT_STATEMENT, NULL);


   ast_add_child(node, parse_expression(lex));

    while (peek(lex)->type == TOKEN_PUNCTUATION &&
           (strcmp(peek(lex)->value, ",") == 0 ||
            strcmp(peek(lex)->value, ";") == 0))
    {
        next(lex); // consume separator
        ast_add_child(node, parse_expression(lex));
    }

    return node;
}

// ---------------- INPUT statement ----------------
ast *parse_input(lexer *lex)
{
    next(lex);
    token *id = expect(lex, TOKEN_IDENTIFIER);
    return init_node(INPUT_STATEMENT, id);
}

// ---------------- GOTO statement ----------------
ast *parse_goto(lexer *lex)
{
    next(lex);
    token *num = expect(lex, TOKEN_NUMBER);
    return init_node(GO_TO_STATEMENT, num);
}

// if-stmt     ::= IF expr relop expr THEN number
ast *parse_if(lexer *lex)
{
    next(lex); // consume IF
    ast *node = init_node(IF_STATEMENT, NULL);
    ast* expr_left = parse_expression(lex);
    

    token *op = expect(lex, TOKEN_OPERATOR);
    ast* relop = init_node(EXPRESSION, op);
    ast* expr_right = parse_expression(lex);
    


    token *thenTok = expect(lex, TOKEN_KEYWORD);
    if (!is_keyword(thenTok, "THEN"))
    {
        fprintf(stderr, "Expected THEN in IF statement\n");
        exit(1);
    }

    token *num = expect(lex, TOKEN_NUMBER);
    ast_add_child(node, relop);
    ast_add_child(relop, expr_left);
    ast_add_child(relop, expr_right);
    ast_add_child(node, init_node(EXPRESSION, num));
    return node;
}

// ---------------- GOSUB statement ----------------
ast *parse_gosub(lexer *lex)
{
    next(lex);
    token *num = expect(lex, TOKEN_NUMBER);
    return init_node(GO_SUB_STATEMENT, num);
}

// ---------------- RETURN statement ----------------
ast *parse_return_stmt(lexer *lex)
{
    next(lex);
    return init_node(RETURN_STATEMENT, NULL);
}

// ---------------- END statement ----------------
ast *parse_end_stmt(lexer *lex)
{
    next(lex);
    return init_node(END_STATEMENT, NULL);
}

// ---------------- REM statement ----------------
ast *parse_rem_stmt(lexer *lex)
{
    token *rem = next(lex); // consume REM
    ast *node = init_node(STRING_LITERAL, rem);

    // consume rest of line as string tokens until EOL
    while (peek(lex)->type != TOKEN_EOL && peek(lex)->type != TOKEN_EOF)
        next(lex);

    return node;
}

// ---------------- Expression parser ----------------
ast *parse_expression(lexer *lex)
{
    ast *left = parse_term(lex);

    while (peek(lex)->type == TOKEN_OPERATOR &&
           (strcmp(peek(lex)->value, "+") == 0 ||
            strcmp(peek(lex)->value, "-") == 0))
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
           (strcmp(peek(lex)->value, "*") == 0 ||
            strcmp(peek(lex)->value, "/") == 0))
    {
        token *op = next(lex);
        ast *right = parse_factor(lex);

        ast *node = init_node(EXPRESSION, op);
        ast_add_child(node, left);
        ast_add_child(node, right);
    }

    return left;
}

ast *parse_factor(lexer *lex)
{
    token *t = peek(lex);

    if (!t)
    {
        fprintf(stderr, "Unexpected end of input in factor\n");
        exit(1);
    }

    if (t->type == TOKEN_NUMBER || t->type == TOKEN_IDENTIFIER)
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
        next(lex); // consume '('
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
    printf("line: %d, col: %d\n", lex->line, lex->col);
    exit(1);
}
