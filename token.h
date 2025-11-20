#ifndef TOKEN_H
#define TOKEN_H

// Token types we support
typedef enum {
    TOKEN_PRINT,
    TOKEN_LET,
    TOKEN_IF,
    TOKEN_THEN,
    TOKEN_INPUT,
    TOKEN_REM,
    TOKEN_LIST,
    TOKEN_RUN,
    TOKEN_NEW,
    TOKEN_FOR,
    TOKEN_TO,
    TOKEN_NEXT,
    TOKEN_WHILE,
    TOKEN_WEND,
    TOKEN_SAVE,
    TOKEN_LOAD,
    TOKEN_DIR,
    TOKEN_RM,
    TOKEN_FORMAT,
    TOKEN_CD,
    TOKEN_PWD,
    TOKEN_MKDIR,
    TOKEN_RMDIR,
    TOKEN_DRIVES,
    TOKEN_CLS,
    TOKEN_GOSUB,
    TOKEN_RETURN,
    TOKEN_GOTO,
    TOKEN_END,
    TOKEN_UNKNOWN,
    TOKEN_EOF,
} TokenType;

// A single token
typedef struct {
    TokenType type;
    char value[256];  // The text/data for this token
    int has_semicolon;  // 1 if followed by semicolon (suppress newline)
    int is_string_literal;  // 1 if this was a quoted string
} Token;

// Parse a line and return the tokens
Token* tokenize(const char *line, int *token_count);

// Free allocated token memory
void free_tokens(Token *tokens);

#endif
