#ifndef PARSE_H
#define PARSE_H




#include "lex.h"
#include "ast.h"
#include "token.h"
ast* parse(lexer* lex);
ast* parse_program(lexer* lex);
ast* parse_line(lexer* lex);
ast* parse_statement(lexer* lex);
ast* parse_let(lexer* lex);
ast* parse_print(lexer* lex);
ast* parse_input(lexer* lex);
ast* parse_goto(lexer* lex);
ast* parse_if(lexer* lex);
ast* parse_gosub(lexer* lex);
ast* parse_return_stmt(lexer* lex);
ast* parse_end_stmt(lexer* lex);
ast* parse_rem_stmt(lexer* lex);
ast* parse_expression(lexer* lex);
ast* parse_term(lexer* lex);
ast* parse_factor(lexer* lex);

#endif