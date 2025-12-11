#include "lex.h"
#include "ast.h"
#include "token.h"
#include "parse.h"

#include <stdlib.h>

int expect(lexer* lex, int type)
{
    token* tok = next_token(lex);
    return tok->type == type;
}


ast* parse(lexer* lex)
{
    return parse_program(lex);
}



// program     ::= { line }
ast* parse_program(lexer* lex)
{
    ast* n = calloc(1, sizeof(ast));
    n->type = PROGRAM;
    n->child_num = 0;
    token* tok;

    do{
        
        ast_add_child(n, parse_line(lex));
        tok = lexer_peek_next_token(lex);

    }while(tok->type != TOKEN_EOF);
    

    return n;
}


// line        ::= number statement
ast* parse_line(lexer* lex)
{
    if(expect(lex, TOKEN_LINE_NUM))
    {
        return parse_statement(lex);
    }
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
ast* parse_statement(lexer* lex)
{

    
}

/*
let-stmt    ::= (LET)? var '=' expr
*/
ast* parse_let_statement(lexer* lex)
{

    
}
// print-stmt  ::= PRINT print-list
ast* parse_print_statement(lexer* lex)
{

    
}
// print-list  ::= print-item { (';' | ',') print-item }
ast* parse_print_list(lexer* lex)
{

    
}
// print-item  ::= expr | string
ast* parse_print_item(lexer* lex)
{

    
}

// input-stmt  ::= INPUT var
ast* parse_input_statement(lexer* lex)
{

    
}

// goto-stmt   ::= GOTO number
ast* parse_goto_statement(lexer* lex)
{

    
}

// if-stmt     ::= IF expr relop expr THEN number
ast* parse_if_statement(lexer* lex)
{

    
}

// gosub-stmt  ::= GOSUB number
ast* parse_gosub_statement(lexer* lex)
{

    
}

// return-stmt ::= RETURN
ast* parse_return_statement(lexer* lex)
{

    
}

// end-stmt    ::= END
ast* parse_end_statement(lexer* lex)
{

    
}

// rem-stmt    ::= REM { any-char }
ast* parse_rem_statement(lexer* lex)
{

    
}

// expr        ::= term { ('+' | '-') term }
ast* parse_expression(lexer* lex)
{

    
}

// term        ::= factor { ('*' | '/') factor }
ast* parse_term(lexer* lex)
{

    
}
// factor      ::= number | var | '(' expr ')'
ast* parse_factor(lexer* lex)
{

    
}
// relop       ::= '=' | '<>' | '<' | '>' | '<=' | '>='
ast* parse_relop(lexer* lex)
{

    
}
// var         ::= 'A'..'Z'
ast* parse_var(lexer* lex)
{

    
}
// number      ::= digit { digit }
ast* parse_number(lexer* lex)
{

    
}
// string      ::= '"' { char } '"'
ast* parse_string(lexer* lex)
{

    
}
