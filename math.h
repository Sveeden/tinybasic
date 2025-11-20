#ifndef MATH_H
#define MATH_H

#include <stdint.h>

// Token types for math expression parsing
typedef enum {
    TOK_NUMBER = 0,
    TOK_WORD,
    TOK_PLUS,
    TOK_MINUS,
    TOK_MULTIPLY,
    TOK_DIVIDE,
    TOK_OPEN_PAREN,
    TOK_CLOSE_PAREN,
    TOK_COMMA,
    TOK_END,
    TOK_ERROR,
    // Math functions
    TOK_ABS,
    TOK_SGN,
    TOK_INT,
    TOK_SQR,
    TOK_SIN,
    TOK_COS,
    TOK_TAN,
    TOK_ATN,
    TOK_LOG,
    TOK_EXP,
    TOK_ROUND,
    TOK_MOD,
    TOK_POWER,
    TOK_MAX,
    TOK_MIN,
    TOK_PI,
} TokenType;

typedef struct {
    TokenType type;
    const char *start;
    int length;
} Token;

// Evaluate a mathematical expression and return result
float evaluate_math_expression(const char *expr, int *error);

#endif // MATH_H
