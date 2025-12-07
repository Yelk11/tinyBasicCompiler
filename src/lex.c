#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "lex.h"
#include "token.h"

lexer *init_lexer(char *src)
{
    lexer *lexer = calloc(1, sizeof(struct lexer));
    lexer->src = src;
    lexer->pos = 0;
    return lexer;
}

void skip_whitespace(lexer *lex)
{
    while (lex->src[lex->pos] == 13 ||
           lex->src[lex->pos] == 10 ||
           lex->src[lex->pos] == ' ' ||
           lex->src[lex->pos] == '\t' ||
           lex->src[lex->pos] == '\n')
    {

        advance_lexer(lex);
    }
}

token *next_token(lexer *lex)
{
    skip_whitespace(lex);
    char c = lex->src[lex->pos];

    if (isdigit(c) != 0)
    {
        if (lex->col == 0)
        {
            return lexer_parse_line_num(lex);
        }
        else
        {
            return lexer_parse_number(lex);
        }
    }
    else if (isalpha(c))
    {
        return lexer_parse_identifier(lex);
    }
    else if(c=='+'|| c=='-'|| c== '*'|| c== '/'|| c== '=' || c== '<'|| c== '>')
    {
        return lexer_parse_operator(lex);
    }
    else if(c=='(' || c==')' || c== ',' || c== ';')
    {
        return lexer_parse_punctuation(lex);
    }
    else{
        printf("ERROR: Unexpected charector at line %d col %d\n", lex->line, lex->col);
    }
}

char lexer_peek(lexer *lex)
{
}

char advance_lexer(lexer *lex)
{
}

token *lexer_parse_string(lexer *lex)
{
}

token *lexer_parse_line_num(lexer *lex)
{
}

token *lexer_parse_keyword(lexer *lex)
{
}

token *lexer_parse_identifier(lexer *lex)
{
}

token *lexer_parse_number(lexer *lex)
{
}

token *lexer_parse_operator(lexer *lex)
{
}

token *lexer_parse_punctuation(lexer *lex)
{
}

token *lexer_parse_end_of_line(lexer *lex)
{
}
