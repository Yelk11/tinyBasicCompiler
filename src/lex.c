#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "lex.h"
#include "token.h"

// -------------------- Keyword check --------------------
static int is_keyword(const char *s)
{
    if (!s) return 0;

    static const char *kw[] = {
        "LET", "PRINT", "IF", "THEN", "GOTO",
        "GOSUB", "RETURN", "END", "INPUT", "REM",
        NULL
    };

    for (int i = 0; kw[i]; ++i) {
        if (strcasecmp(s, kw[i]) == 0) return 1;
    }

    return 0;
}

// -------------------- Lexer init / helpers --------------------
lexer *init_lexer(char *src)
{
    lexer *lex = calloc(1, sizeof(lexer));
    lex->src = src;
    lex->pos = 0;
    lex->line = 1;
    lex->col = 0;
    return lex;
}

char lexer_peek(lexer *lex) { return lex->src[lex->pos]; }

token *lexer_peek_next_token(lexer *lex)
{
    lexer temp = *lex;
    return next_token(&temp);
}

char advance_lexer(lexer *lex)
{
    char c = lex->src[lex->pos++];
    if (c == '\n') {
        lex->line++;
        lex->col = 0;
    } else {
        lex->col++;
    }
    return c;
}

void skip_whitespace(lexer *lex)
{
    while (lex->src[lex->pos] == ' ' ||
           lex->src[lex->pos] == '\t' ||
           lex->src[lex->pos] == '\r')
    {
        advance_lexer(lex);
    }
}



// -------------------- Tokenizer --------------------
token *next_token(lexer *lex)
{
    skip_whitespace(lex);


    char c = lexer_peek(lex);

    if (c == '\0') 
    {
        return init_token(NULL, TOKEN_EOF);
    }

    if (isdigit(c)) 
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
    if (isalpha(c)) 
    {
        return lexer_parse_identifier(lex);
    }
    switch(c)
    {
        case '\n':
            advance_lexer(lex);
            return init_token("\n", TOKEN_EOL);
        case '+':
            advance_lexer(lex);
            return init_token("+", TOKEN_OPERATOR);
        case '-':
            advance_lexer(lex);
            return init_token("-", TOKEN_OPERATOR);
        case '*':
            advance_lexer(lex);
            return init_token("*", TOKEN_OPERATOR);
        case '/':
            advance_lexer(lex);
            return init_token("/", TOKEN_OPERATOR);
        case '=':
            advance_lexer(lex);
            if(lexer_peek(lex) == '=')
            {
                advance_lexer(lex);
                return init_token("==", TOKEN_OPERATOR);
            }
            else
            {
                return init_token("=", TOKEN_OPERATOR);
            }
        case '<':
            advance_lexer(lex);
            if(lexer_peek(lex) == '>')
            {
                advance_lexer(lex);
                return init_token("<>", TOKEN_OPERATOR);
            }
            else if(lexer_peek(lex) == '=')
            {
                advance_lexer(lex);
                return init_token("<=", TOKEN_OPERATOR);
            }
            else
            {
                return init_token("<", TOKEN_OPERATOR);
            }
        case '>':
            advance_lexer(lex);
            if(lexer_peek(lex) == '=')
            {
                advance_lexer(lex);
                return init_token(">=", TOKEN_OPERATOR);
            }
            else
            {
                return init_token(">", TOKEN_OPERATOR);
            }
        case '(': 
        case ')':
        case ',':
        case ';':
            return lexer_parse_punctuation(lex);
        case '"':
            return lexer_parse_string(lex);
            
    }

    fprintf(stderr, "ERROR: Unexpected character '%c' at line %d col %d\n",
            c, lex->line, lex->col);
    advance_lexer(lex);
    return NULL;
}

// -------------------- Token parsers --------------------
token *lexer_parse_string(lexer *lex)
{
    advance_lexer(lex); // skip opening "
    size_t cap = 16;
    size_t len = 0;
    char *buf = malloc(cap);
    buf[0] = '\0';

    while (lexer_peek(lex) != '"' && lexer_peek(lex) != '\0')
    {
        if (len+1 >= cap) {
            cap *= 2;
            buf = realloc(buf, cap);
        }
        buf[len++] = advance_lexer(lex);
        buf[len] = '\0';
    }
    advance_lexer(lex); // skip closing "
    return init_token(buf, TOKEN_STRING);
}

token *lexer_parse_line_num(lexer *lex)
{
    size_t cap = 16;
    size_t len = 0;
    char *buf = malloc(cap);

    while (isdigit(lexer_peek(lex))) {
        if (len+1 >= cap) buf = realloc(buf, cap*2);
        buf[len++] = advance_lexer(lex);
        buf[len] = '\0';
    }

    return init_token(buf, TOKEN_LINE_NUM);
}

token *lexer_parse_identifier(lexer *lex)
{
    size_t cap = 16;
    size_t len = 0;
    char *buf = malloc(cap);

    while (isalnum(lexer_peek(lex))) {
        if (len+1 >= cap) buf = realloc(buf, cap*2);
        buf[len++] = advance_lexer(lex);
        buf[len] = '\0';
    }

    if (is_keyword(buf)) return init_token(buf, TOKEN_KEYWORD);
    return init_token(buf, TOKEN_IDENTIFIER);
}

token *lexer_parse_number(lexer *lex)
{
    size_t cap = 16;
    size_t len = 0;
    char *buf = malloc(cap);

    while (isdigit(lexer_peek(lex))) {
        if (len+1 >= cap) buf = realloc(buf, cap*2);
        buf[len++] = advance_lexer(lex);
        buf[len] = '\0';
    }

    return init_token(buf, TOKEN_NUMBER);
}



token *lexer_parse_punctuation(lexer *lex)
{
    char p = advance_lexer(lex);
    char str[2] = { p, 0 };
    return init_token(strdup(str), TOKEN_PUNCTUATION);
}

token *lexer_parse_end_of_line(lexer *lex)
{
    advance_lexer(lex);
    return init_token(NULL, TOKEN_EOL);
}
