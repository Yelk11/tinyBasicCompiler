#ifndef PARSE_H
#define PARSE_H




#include "lex.h"
#include "ast.h"
#include "token.h"


int expect(lexer* lex, int type);


ast* parse(lexer* lex);


// program     ::= { line }
ast* parse_program(lexer* lex);


// line        ::= number statement
ast* parse_line(lexer* lex);

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
ast* parse_statement(lexer* lex);
/*
let-stmt    ::= (LET)? var '=' expr
*/
ast* parse_let_statement(lexer* lex);
// print-stmt  ::= PRINT print-list
ast* parse_print_statement(lexer* lex);
// print-list  ::= print-item { (';' | ',') print-item }
ast* parse_print_list(lexer* lex);
// print-item  ::= expr | string
ast* parse_print_item(lexer* lex);
// input-stmt  ::= INPUT var
ast* parse_input_statement(lexer* lex);
// goto-stmt   ::= GOTO number
ast* parse_goto_statement(lexer* lex);
// if-stmt     ::= IF expr relop expr THEN number
ast* parse_if_statement(lexer* lex);
// gosub-stmt  ::= GOSUB number
ast* parse_gosub_statement(lexer* lex);
// return-stmt ::= RETURN
ast* parse_return_statement(lexer* lex);
// end-stmt    ::= END
ast* parse_end_statement(lexer* lex);
// rem-stmt    ::= REM { any-char }
ast* parse_rem_statement(lexer* lex);
// expr        ::= term { ('+' | '-') term }
ast* parse_expression(lexer* lex);
// term        ::= factor { ('*' | '/') factor }
ast* parse_term(lexer* lex);
// factor      ::= number | var | '(' expr ')'
ast* parse_factor(lexer* lex);
// relop       ::= '=' | '<>' | '<' | '>' | '<=' | '>='
ast* parse_relop(lexer* lex);
// var         ::= 'A'..'Z'
ast* parse_var(lexer* lex);
// number      ::= digit { digit }
ast* parse_number(lexer* lex);
// string      ::= '"' { char } '"'
ast* parse_string(lexer* lex);


#endif