#include "variables.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

#define MAX_VARS 50
#define MAX_VAR_NAME 50
#define MAX_VAR_VALUE 256

typedef struct {
    char name[MAX_VAR_NAME];      // "x" or "x$"
    char value[MAX_VAR_VALUE];    // "10" or "hello"
    int is_string;                // 1 if string, 0 if number
} Variable;

static Variable vars[MAX_VARS];
static int var_count = 0;

void var_init(void) {
    var_count = 0;
    memset(vars, 0, sizeof(vars));
}

static int find_var(const char *name) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(vars[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

void var_set(const char *assignment) {
    // Parse "x=10" or "x$=\"hello\"" or "x=y+5"
    char name[MAX_VAR_NAME];
    char value[MAX_VAR_VALUE];
    int is_string = 0;
    
    // Find the = sign
    const char *eq = strchr(assignment, '=');
    if (!eq) return;
    
    // Extract variable name (trim spaces)
    strncpy(name, assignment, eq - assignment);
    name[eq - assignment] = '\0';
    
    // Trim trailing spaces from name
    int len = strlen(name);
    while (len > 0 && name[len - 1] == ' ') {
        name[--len] = '\0';
    }
    
    // Check if it's a string variable (ends with $)
    if (len > 0 && name[len - 1] == '$') {
        is_string = 1;
    }
    
    // Extract value (skip = and leading spaces)
    const char *val_start = eq + 1;
    while (*val_start == ' ' || *val_start == '\t') {
        val_start++;
    }
    
    // Handle quoted strings
    if (*val_start == '"') {
        val_start++;  // Skip opening quote
        char *dest = value;
        while (*val_start && *val_start != '"') {
            *dest++ = *val_start++;
        }
        *dest = '\0';
        is_string = 1;
    } else {
        // Try to evaluate as expression (variable +/- number or variable)
        strcpy(value, val_start);
        
        // Look for arithmetic operators
        char *plus = strchr(value, '+');
        char *minus = strchr(value, '-');
        char *mult = strchr(value, '*');
        char *divv = strchr(value, '/');
        
        // Find the first operator (excluding leading minus)
        char *op = NULL;
        char op_char = '\0';
        if (plus) { op = plus; op_char = '+'; }
        if (minus && minus != value && (!op || minus < op)) { op = minus; op_char = '-'; }
        if (mult && (!op || mult < op)) { op = mult; op_char = '*'; }
        if (divv && (!op || divv < op)) { op = divv; op_char = '/'; }
        
        if (op) {
            // Parse left operand
            char left_str[256];
            strncpy(left_str, value, op - value);
            left_str[op - value] = '\0';
            
            // Trim spaces
            len = strlen(left_str);
            while (len > 0 && left_str[len - 1] == ' ') left_str[--len] = '\0';
            len = strlen(left_str);
            while (len > 0 && left_str[0] == ' ') {
                memmove(left_str, left_str + 1, strlen(left_str));
                len--;
            }
            
            // Parse right operand
            char right_str[256];
            strcpy(right_str, op + 1);
            
            // Trim spaces
            len = strlen(right_str);
            while (len > 0 && right_str[0] == ' ') {
                memmove(right_str, right_str + 1, strlen(right_str));
                len--;
            }
            len = strlen(right_str);
            while (len > 0 && right_str[len - 1] == ' ') right_str[--len] = '\0';
            
            // Get numeric values
            const char *left_val_str = var_get(left_str);
            if (left_val_str == NULL) left_val_str = left_str;
            int left_val = atoi(left_val_str);
            
            const char *right_val_str = var_get(right_str);
            if (right_val_str == NULL) right_val_str = right_str;
            int right_val = atoi(right_val_str);
            
            // Perform operation
            int result = 0;
            switch (op_char) {
                case '+': result = left_val + right_val; break;
                case '-': result = left_val - right_val; break;
                case '*': result = left_val * right_val; break;
                case '/': result = right_val != 0 ? left_val / right_val : 0; break;
            }
            
            snprintf(value, sizeof(value), "%d", result);
            is_string = 0;
        }
    }
    
    // Find or create variable
    int idx = find_var(name);
    if (idx >= 0) {
        // Update existing
        strcpy(vars[idx].value, value);
        vars[idx].is_string = is_string;
    } else if (var_count < MAX_VARS) {
        // Add new
        strcpy(vars[var_count].name, name);
        strcpy(vars[var_count].value, value);
        vars[var_count].is_string = is_string;
        var_count++;
    }
}

const char* var_get(const char *name) {
    int idx = find_var(name);
    if (idx >= 0) {
        return vars[idx].value;
    }
    return NULL;
}

int var_is_string(const char *name) {
    int idx = find_var(name);
    if (idx >= 0) {
        return vars[idx].is_string;
    }
    return 0;
}

int var_is_number(const char *name) {
    int idx = find_var(name);
    if (idx >= 0) {
        return !vars[idx].is_string;
    }
    return 0;
}
