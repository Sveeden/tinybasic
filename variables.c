#include "variables.h"
#include "math.h"
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

// Helper to trim trailing zeros and unnecessary decimal point from float strings
static void trim_float_string(char *str) {
    if (!strchr(str, '.')) return;  // Not a float string
    
    // Find the end of the string
    int len = strlen(str);
    
    // Trim trailing zeros
    while (len > 0 && str[len - 1] == '0') {
        str[--len] = '\0';
    }
    
    // If the string ends with '.', remove it too
    if (len > 0 && str[len - 1] == '.') {
        str[--len] = '\0';
    }
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
    
    // Uppercase variable name for consistency
    for (int i = 0; i < len; i++) {
        name[i] = toupper(name[i]);
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
    } else if (is_string) {
        // String variable - store the value as-is (no math evaluation for strings)
        strcpy(value, val_start);
    } else {
        // Numeric variable - use math expression evaluator
        int error = 0;
        float result = evaluate_math_expression(val_start, &error);
        
        if (!error) {
            // Successfully evaluated as math expression
            // Print as integer if it's a whole number, otherwise as float
            if (result == (int)result) {
                snprintf(value, sizeof(value), "%d", (int)result);
            } else {
                snprintf(value, sizeof(value), "%g", result);
                trim_float_string(value);  // Remove trailing zeros
            }
            is_string = 0;
        } else {
            // If evaluation failed, treat as string or just copy the value
            strcpy(value, val_start);
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
    // Uppercase name for consistent lookup
    char upper_name[MAX_VAR_NAME];
    strncpy(upper_name, name, MAX_VAR_NAME - 1);
    for (int i = 0; upper_name[i]; i++) {
        upper_name[i] = toupper(upper_name[i]);
    }
    int idx = find_var(upper_name);
    if (idx >= 0) {
        return vars[idx].value;
    }
    return NULL;
}

int var_is_string(const char *name) {
    // Uppercase name for consistent lookup
    char upper_name[MAX_VAR_NAME];
    strncpy(upper_name, name, MAX_VAR_NAME - 1);
    for (int i = 0; upper_name[i]; i++) {
        upper_name[i] = toupper(upper_name[i]);
    }
    int idx = find_var(upper_name);
    if (idx >= 0) {
        return vars[idx].is_string;
    }
    return 0;
}

int var_is_number(const char *name) {
    // Uppercase name for consistent lookup
    char upper_name[MAX_VAR_NAME];
    strncpy(upper_name, name, MAX_VAR_NAME - 1);
    for (int i = 0; upper_name[i]; i++) {
        upper_name[i] = toupper(upper_name[i]);
    }
    int idx = find_var(upper_name);
    if (idx >= 0) {
        return !vars[idx].is_string;
    }
    return 0;
}
