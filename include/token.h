#ifndef TOKEN_H
#define TOKEN_H
/*
| Type        | Examples                                            | Notes                                 |
| ----------- | --------------------------------------------------- | ------------------------------------- |
| Line num    | `10`, `20`                                          | Marks program order; integer          |
| Keyword     | `LET`, `PRINT`, `IF`, etc.                          | Case-insensitive                      |
| Identifier  | `A`–`Z`                                             | Single-letter variables only          |
| Number      | `5`, `123`, `-7`                                    | Integer only                          |
| Operator    | `+`, `-`, `*`, `/`, `=`, `<`, `>`, `<=`, `>=`, `<>` | Used in expressions and IF conditions |
| Punctuation | `(`, `)`, `,`, `;`                                  | For grouping and PRINT formatting     |
| String      | `"HELLO"`                                           | Only used in PRINT or REM             |
| End-of-line | Implicit at each program line                       | Helps parser detect end of statement  |
*/

enum token_type
{
    TOKEN_LINE_NUM,
    TOKEN_KEYWORD,
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_PUNCTUATION,
    TOKEN_STRING,
    TOKEN_EOL,
    TOKEN_EOF
}token_type;

typedef struct token
{
    char *value;
    enum token_type type;
} token;

token *init_token(char *value, int type);
char *type_to_string(int token);

#endif