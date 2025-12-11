#ifndef LEX_H
#define LEX_H

#include "token.h"

typedef struct lexer{
    char* src;
    int pos;
    int line;
    int col;
}lexer;

lexer* init_lexer(char* src);

token* next_token(lexer* lex);

char lexer_peek(lexer* lex);

token *lexer_peek_next_token(lexer *lex);

char advance_lexer(lexer* lex);

token* lexer_parse_string(lexer* lex);

token* lexer_parse_line_num(lexer* lex);

token* lexer_parse_keyword(lexer* lex);

token* lexer_parse_identifier(lexer* lex);

token* lexer_parse_number(lexer* lex);

token* lexer_parse_operator(lexer* lex);

token* lexer_parse_punctuation(lexer* lex);

token* lexer_parse_end_of_line(lexer* lex);


#endif