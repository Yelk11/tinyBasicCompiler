#include "token.h"
#include <stdlib.h>

token *init_token(char *value, int type)
{
    token *token = calloc(1, sizeof(struct token));
    token->value = value;
    token->type = type;
    return token;
}

char *type_to_string(int token)
{
    switch (token)
    {

    case TOKEN_LINE_NUM:
        return "TOKEN_LINE_NUM";
    case TOKEN_KEYWORD:
        return "TOKEN_KEYWORD";
    case TOKEN_IDENTIFIER:
        return "TOKEN_IDENTIFIER";
    case TOKEN_NUMBER:
        return "TOKEN_NUMBER";
    case TOKEN_OPERATOR:
        return "TOKEN_OPERATOR";
    case TOKEN_PUNCTUATION:
        return "TOKEN_PUNCTUATION";
    case TOKEN_STRING:
        return "TOKEN_STRING";
    case TOKEN_EOL:
        return "TOKEN_EOL";

    }
    return "nope";
}


