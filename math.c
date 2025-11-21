#include "math.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "variables.h"

// Tokenizer state
typedef struct {
    const char *input;
    const char *current;
} Tokenizer;

static void upcase(char *str) {
    while (*str) {
        *str = toupper(*str);
        str++;
    }
}

static Token next_token(Tokenizer *tz) {
    // Skip whitespace
    while (*tz->current && isspace(*tz->current)) {
        tz->current++;
    }
    
    Token t;
    t.start = tz->current;
    t.length = 0;
    
    if (*tz->current == '\0') {
        t.type = TOK_END;
        return t;
    }
    
    // Single character tokens
    if (*tz->current == '+') { t.type = TOK_PLUS; t.length = 1; tz->current++; return t; }
    if (*tz->current == '-') { t.type = TOK_MINUS; t.length = 1; tz->current++; return t; }
    if (*tz->current == '*') { t.type = TOK_MULTIPLY; t.length = 1; tz->current++; return t; }
    if (*tz->current == '/') { t.type = TOK_DIVIDE; t.length = 1; tz->current++; return t; }
    if (*tz->current == '(') { t.type = TOK_OPEN_PAREN; t.length = 1; tz->current++; return t; }
    if (*tz->current == ')') { t.type = TOK_CLOSE_PAREN; t.length = 1; tz->current++; return t; }
    if (*tz->current == ',') { t.type = TOK_COMMA; t.length = 1; tz->current++; return t; }
    
    // Numbers
    if (isdigit(*tz->current) || (*tz->current == '.' && isdigit(*(tz->current + 1)))) {
        t.type = TOK_NUMBER;
        while (*tz->current && (isdigit(*tz->current) || *tz->current == '.')) {
            tz->current++;
            t.length++;
        }
        return t;
    }
    
    // Words (variables or keywords)
    if (isalpha(*tz->current)) {
        t.type = TOK_WORD;
        while (*tz->current && (isalnum(*tz->current) || *tz->current == '$')) {
            tz->current++;
            t.length++;
        }
        
        // Check for keywords
        char keyword[32];
        strncpy(keyword, t.start, t.length);
        keyword[t.length] = '\0';
        upcase(keyword);
        
        if (strcmp(keyword, "ABS") == 0) t.type = TOK_ABS;
        else if (strcmp(keyword, "SGN") == 0) t.type = TOK_SGN;
        else if (strcmp(keyword, "INT") == 0) t.type = TOK_INT;
        else if (strcmp(keyword, "SQR") == 0) t.type = TOK_SQR;
        else if (strcmp(keyword, "SIN") == 0) t.type = TOK_SIN;
        else if (strcmp(keyword, "COS") == 0) t.type = TOK_COS;
        else if (strcmp(keyword, "TAN") == 0) t.type = TOK_TAN;
        else if (strcmp(keyword, "ATN") == 0) t.type = TOK_ATN;
        else if (strcmp(keyword, "LOG") == 0) t.type = TOK_LOG;
        else if (strcmp(keyword, "EXP") == 0) t.type = TOK_EXP;
        else if (strcmp(keyword, "ROUND") == 0) t.type = TOK_ROUND;
        else if (strcmp(keyword, "MOD") == 0) t.type = TOK_MOD;
        else if (strcmp(keyword, "POWER") == 0) t.type = TOK_POWER;
        else if (strcmp(keyword, "MAX") == 0) t.type = TOK_MAX;
        else if (strcmp(keyword, "MIN") == 0) t.type = TOK_MIN;
        else if (strcmp(keyword, "PI") == 0) t.type = TOK_PI;
        else if (strcmp(keyword, "RND") == 0) t.type = TOK_RND;
        
        return t;
    }
    
    t.type = TOK_ERROR;
    t.length = 1;
    tz->current++;
    return t;
}

// Forward declarations
static float evaluate_expression(Tokenizer *tz, int *error);

static int check_comma(Tokenizer *tz, int *error) {
    const char *temp = tz->current;
    Token t = next_token(tz);
    
    if (t.type == TOK_COMMA) {
        return 1;
    }
    
    tz->current = temp;
    return 0;
}

static float primary(Tokenizer *tz, int *error) {
    Token t = next_token(tz);
    float sign = 1.0f;
    
    // Handle unary +/-
    if (t.type == TOK_MINUS) {
        sign = -1.0f;
        t = next_token(tz);
    } else if (t.type == TOK_PLUS) {
        t = next_token(tz);
    }
    
    // Numbers
    if (t.type == TOK_NUMBER) {
        char num_str[32];
        strncpy(num_str, t.start, t.length);
        num_str[t.length] = '\0';
        return atof(num_str) * sign;
    }
    
    // Variables
    else if (t.type == TOK_WORD) {
        char varname[32];
        strncpy(varname, t.start, t.length);
        varname[t.length] = '\0';
        upcase(varname);
        
        if (varname[strlen(varname) - 1] != '$') {
            const char *val_str = var_get(varname);
            float val = val_str ? atof(val_str) : 0.0f;
            return val * sign;
        }
        
        *error = 1;
        printf("?TYPE MISMATCH\n");
        return 0.0f;
    }
    
    // Parenthesized expression
    else if (t.type == TOK_OPEN_PAREN) {
        float val = evaluate_expression(tz, error);
        if (*error) return 0.0f;
        
        Token close = next_token(tz);
        if (close.type != TOK_CLOSE_PAREN) {
            *error = 1;
            printf("?MISSING CLOSING PAREN\n");
            return 0.0f;
        }
        return val * sign;
    }
    
    // PI constant
    else if (t.type == TOK_PI) {
        return 3.14159265358979323846f * sign;
    }
    
    // Single-argument math functions
    else if (t.type == TOK_ABS || t.type == TOK_SGN || t.type == TOK_INT || t.type == TOK_SQR ||
             t.type == TOK_SIN || t.type == TOK_COS || t.type == TOK_TAN || t.type == TOK_ATN ||
             t.type == TOK_LOG || t.type == TOK_EXP || t.type == TOK_ROUND)
    {
        Token func_type = t;
        
        Token open = next_token(tz);
        if (open.type != TOK_OPEN_PAREN) {
            *error = 1;
            printf("?FUNCTION REQUIRES (\n");
            return 0.0f;
        }
        
        float val = evaluate_expression(tz, error);
        if (*error) return 0.0f;
        
        Token close = next_token(tz);
        if (close.type != TOK_CLOSE_PAREN) {
            *error = 1;
            printf("?MISSING CLOSING PAREN\n");
            return 0.0f;
        }
        
        if (func_type.type == TOK_ABS) return fabsf(val);
        else if (func_type.type == TOK_SGN) {
            if (val > 0.0f) return 1.0f;
            if (val < 0.0f) return -1.0f;
            return 0.0f;
        }
        else if (func_type.type == TOK_INT) return floorf(val);
        else if (func_type.type == TOK_SQR) {
            if (val < 0.0f) {
                *error = 1;
                printf("?SQR OF NEGATIVE\n");
                return 0.0f;
            }
            return sqrtf(val);
        }
        else if (func_type.type == TOK_SIN) return sinf(val);
        else if (func_type.type == TOK_COS) return cosf(val);
        else if (func_type.type == TOK_TAN) return tanf(val);
        else if (func_type.type == TOK_ATN) return atanf(val);
        else if (func_type.type == TOK_LOG) {
            if (val <= 0.0f) {
                *error = 1;
                printf("?LOG OF NON-POSITIVE\n");
                return 0.0f;
            }
            return logf(val);
        }
        else if (func_type.type == TOK_EXP) return expf(val);
        else if (func_type.type == TOK_ROUND) return roundf(val);
    }
    
    // RND function - returns random number from 1 to max (or 0-1 if no parameter)
    else if (t.type == TOK_RND) {
        Token open = next_token(tz);
        if (open.type != TOK_OPEN_PAREN) {
            *error = 1;
            printf("?FUNCTION REQUIRES (\n");
            return 0.0f;
        }
        
        float max_val = evaluate_expression(tz, error);
        if (*error) return 0.0f;
        
        Token close = next_token(tz);
        if (close.type != TOK_CLOSE_PAREN) {
            *error = 1;
            printf("?MISSING CLOSING PAREN\n");
            return 0.0f;
        }
        
        // RND(max) returns random number from 1 to max (inclusive)
        if (max_val < 1.0f) max_val = 1.0f;  // Minimum of 1
        int result = (rand() % (int)max_val) + 1;
        return (float)result * sign;
    }
    
    // Multi-argument functions
    else if (t.type == TOK_MOD || t.type == TOK_POWER || t.type == TOK_MAX || t.type == TOK_MIN) {
        Token func_type = t;
        
        Token open = next_token(tz);
        if (open.type != TOK_OPEN_PAREN) {
            *error = 1;
            printf("?FUNCTION REQUIRES (\n");
            return 0.0f;
        }
        
        float val1 = evaluate_expression(tz, error);
        if (*error) return 0.0f;
        
        if (!check_comma(tz, error)) {
            *error = 1;
            printf("?MISSING COMMA\n");
            return 0.0f;
        }
        
        float val2 = evaluate_expression(tz, error);
        if (*error) return 0.0f;
        
        Token close = next_token(tz);
        if (close.type != TOK_CLOSE_PAREN) {
            *error = 1;
            printf("?MISSING CLOSING PAREN\n");
            return 0.0f;
        }
        
        if (func_type.type == TOK_POWER) return powf(val1, val2);
        else if (func_type.type == TOK_MOD) return fmodf(val1, val2);
        else if (func_type.type == TOK_MAX) return fmaxf(val1, val2);
        else if (func_type.type == TOK_MIN) return fminf(val1, val2);
    }
    
    *error = 1;
    printf("?SYNTAX ERROR\n");
    return 0.0f;
}

static float factor(Tokenizer *tz, int *error) {
    return primary(tz, error);
}

static float term(Tokenizer *tz, int *error) {
    float result = factor(tz, error);
    if (*error) return 0.0f;
    
    while (1) {
        const char *temp = tz->current;
        Token t = next_token(tz);
        
        if (t.type == TOK_MULTIPLY) {
            float right = factor(tz, error);
            if (*error) return 0.0f;
            result *= right;
        } else if (t.type == TOK_DIVIDE) {
            float right = factor(tz, error);
            if (*error) return 0.0f;
            if (right == 0.0f) {
                *error = 1;
                printf("?DIVISION BY ZERO\n");
                return 0.0f;
            }
            result /= right;
        } else {
            tz->current = temp;
            break;
        }
    }
    
    return result;
}

static float evaluate_expression(Tokenizer *tz, int *error) {
    float result = term(tz, error);
    if (*error) return 0.0f;
    
    while (1) {
        const char *temp = tz->current;
        Token t = next_token(tz);
        
        if (t.type == TOK_PLUS) {
            float right = term(tz, error);
            if (*error) return 0.0f;
            result += right;
        } else if (t.type == TOK_MINUS) {
            float right = term(tz, error);
            if (*error) return 0.0f;
            result -= right;
        } else {
            tz->current = temp;
            break;
        }
    }
    
    return result;
}

float evaluate_math_expression(const char *expr, int *error) {
    *error = 0;
    Tokenizer tz;
    tz.input = expr;
    tz.current = expr;
    
    float result = evaluate_expression(&tz, error);
    if (*error) return 0.0f;
    
    Token t = next_token(&tz);
    if (t.type != TOK_END) {
        *error = 1;
        printf("?EXTRA CHARACTERS\n");
        return 0.0f;
    }
    
    return result;
}
