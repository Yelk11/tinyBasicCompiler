#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "lex.h"
#include "token.h"

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

lexer *init_lexer(char *src)
{
    lexer *lexer = calloc(1, sizeof(struct lexer));
    lexer->src = src;
    lexer->pos = 0;
    lexer->line = 1;
    lexer->col  = 0;
    return lexer;
}

char lexer_peek(lexer *lex)
{
    return lex->src[lex->pos];
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
    while (lex->src[lex->pos] == 13 ||
           lex->src[lex->pos] == 10 ||
           lex->src[lex->pos] == ' '  ||
           lex->src[lex->pos] == '\t' ||
           lex->src[lex->pos] == '\n')
    {
        advance_lexer(lex);
    }
}

void skip_comment(lexer* lex)
{
    // Detect "REM"
    int start = lex->pos;

    if (toupper(lex->src[start])   == 'R' &&
        toupper(lex->src[start+1]) == 'E' &&
        toupper(lex->src[start+2]) == 'M')
    {
        // consume REM
        advance_lexer(lex);
        advance_lexer(lex);
        advance_lexer(lex);

        // skip until end of line
        while (lexer_peek(lex) != '\n' && lexer_peek(lex) != '\0') {
            advance_lexer(lex);
        }
    }
}

token *next_token(lexer *lex)
{
    skip_whitespace(lex);
    skip_comment(lex);
    skip_whitespace(lex);

    char c = lex->src[lex->pos];

    if (c == '\0')
        return init_token(NULL, TOKEN_EOF);

    // Line num
    if (isdigit(c))
    {
        if (lex->col == 0)
            return lexer_parse_line_num(lex);
        else
            return lexer_parse_number(lex);
    }
    // identifier / keyword
    else if (isalpha(c))
    {
        return lexer_parse_identifier(lex);
    }
    // operators
    else if (c=='+' || c=='-' || c=='*' || c=='/' ||
             c=='=' || c=='<' || c=='>' )
    {
        return lexer_parse_operator(lex);
    }
    // punctuation
    else if (c=='(' || c==')' || c==',' || c==';')
    {
        return lexer_parse_punctuation(lex);
    }

    printf("ERROR: Unexpected character '%c' at line %d col %d\n",
           c, lex->line, lex->col);
    advance_lexer(lex);
    return NULL;
}

// -------------------- PARSER HELPERS --------------------

token *lexer_parse_string(lexer *lex)
{
    char *value = calloc(1, 1);
    advance_lexer(lex); // skip opening "

    while (lex->src[lex->pos] != '"' && lex->src[lex->pos] != '\0')
    {
        size_t len = strlen(value);
        value = realloc(value, len + 2);
        value[len] = lex->src[lex->pos];
        value[len+1] = 0;
        advance_lexer(lex);
    }

    advance_lexer(lex); // skip closing "
    return init_token(value, TOKEN_STRING);
}

token *lexer_parse_line_num(lexer *lex)
{
    char *value = calloc(1,1);

    while (isdigit(lex->src[lex->pos]))
    {
        size_t len = strlen(value);
        value = realloc(value, len + 2);
        value[len] = lex->src[lex->pos];
        value[len+1] = 0;
        advance_lexer(lex);
    }

    return init_token(value, TOKEN_LINE_NUM);
}

token *lexer_parse_identifier(lexer *lex)
{
    char *value = calloc(1,1);

    while (isalnum(lex->src[lex->pos]))
    {
        size_t len = strlen(value);
        value = realloc(value, len + 2);
        value[len] = lex->src[lex->pos];
        value[len+1] = 0;
        advance_lexer(lex);
    }

    // detect keywords (LET, PRINT, IF, GOTO, etc)
    if (is_keyword(value))
        return init_token(value, TOKEN_KEYWORD);

    return init_token(value, TOKEN_IDENTIFIER);
}

token *lexer_parse_number(lexer *lex)
{
    char *value = calloc(1,1);

    while (isdigit(lex->src[lex->pos]))
    {
        size_t len = strlen(value);
        value = realloc(value, len + 2);
        value[len] = lex->src[lex->pos];
        value[len+1] = 0;
        advance_lexer(lex);
    }

    return init_token(value, TOKEN_NUMBER);
}

token *lexer_parse_operator(lexer *lex)
{
    char op = advance_lexer(lex);
    char str[2] = { op, 0 };
    return init_token(strdup(str), TOKEN_OPERATOR);
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
