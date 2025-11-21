#include "execute.h"
#include "variables.h"
#include "program.h"
#include "loops.h"
#include "filesystem.h"
#include "help.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "pico/stdlib.h"

// Forward declaration of main.c functions for EDIT mode
extern void set_edit_mode(int line_num, const char *line_text);

// Forward declaration of math evaluator (from math.c)
// We can't include math.h due to Token struct name collision
extern float evaluate_math_expression(const char *expr, int *error);

// Global interrupt flag for Ctrl-C handling
volatile int execution_interrupted = 0;

// Global seed for RND function (0 means use default)
static unsigned int rnd_seed = 0;

// Check if execution should be interrupted and clear the flag
int should_stop_execution(void) {
    if (execution_interrupted) {
        execution_interrupted = 0;
        return 1;
    }
    return 0;
}

#define EXEC_LINE_CONTINUE 0
#define EXEC_LINE_JUMP 1
#define EXEC_LINE_END 2
#define EXEC_LINE_BREAK 3
#define MAX_STATEMENT_LEN 256

static int evaluate_condition(const char *condition);
static int evaluate_simple_condition(const char *condition);
static void trim_whitespace(char *str);
static char* find_operator(char *str, const char *op);

static bool extract_next_statement(const char **cursor, char *buffer, size_t size) {
    if (!cursor || !*cursor || **cursor == '\0') {
        return false;
    }

    const char *ptr = *cursor;
    const char *start = ptr;
    bool in_quote = false;

    while (*ptr) {
        if (*ptr == '"') {
            in_quote = !in_quote;
        } else if (!in_quote && *ptr == ':') {
            break;
        }
        ptr++;
    }

    const char *end = ptr;

    while (start < end && isspace((unsigned char)*start)) {
        start++;
    }
    while (end > start && isspace((unsigned char)*(end - 1))) {
        end--;
    }

    size_t len = (size_t)(end - start);
    if (len >= size) {
        len = size - 1;
    }
    if (len > 0) {
        memcpy(buffer, start, len);
        buffer[len] = '\0';
    } else {
        buffer[0] = '\0';
    }

    if (*ptr == ':') {
        ptr++;
    }

    while (*ptr == ' ' || *ptr == '\t') {
        ptr++;
    }

    *cursor = ptr;
    return true;
}

static int execute_line_segments(const char *line_text, int current_line, int *next_line_out, bool program_mode) {
    if (next_line_out) {
        *next_line_out = -1;
    }

    const char *cursor = line_text;
    char statement[MAX_STATEMENT_LEN];
    bool skip_remaining_line = false;

    while (extract_next_statement(&cursor, statement, sizeof(statement))) {
        if (statement[0] == '\0') {
            continue;
        }

        int token_count = 0;
        Token* tokens = tokenize(statement, &token_count);
        if (token_count == 0) {
            free_tokens(tokens);
            continue;
        }

        if (skip_remaining_line) {
            free_tokens(tokens);
            continue;
        }

        if (token_count >= 2 && tokens[0].type == TOKEN_IF) {
            bool condition_true = evaluate_condition(tokens[1].value);

            if (condition_true) {
                int result = -1;
                if (token_count >= 4 && strlen(tokens[3].value) > 0) {
                    int then_token_count = 0;
                    Token* then_tokens = tokenize(tokens[3].value, &then_token_count);
                    int then_line = program_mode ? current_line : -1;
                    result = execute(then_tokens, then_token_count, then_line);
                    free_tokens(then_tokens);

                    if (should_stop_execution()) {
                        free_tokens(tokens);
                        if (next_line_out) {
                            *next_line_out = -1;
                        }
                        return EXEC_LINE_BREAK;
                    }

                    if (result == -2) {
                        free_tokens(tokens);
                        return EXEC_LINE_END;
                    }

                    if (result >= 0) {
                        free_tokens(tokens);
                        if (next_line_out) {
                            *next_line_out = result;
                        }
                        return EXEC_LINE_JUMP;
                    }
                }

                free_tokens(tokens);
                continue;
            } else {
                skip_remaining_line = true;
                free_tokens(tokens);
                continue;
            }
        }

        if (program_mode && tokens[0].type == TOKEN_FOR) {
            int next_line_for_body = prog_next_line(current_line);
            execute(tokens, token_count, next_line_for_body);
            free_tokens(tokens);
            continue;
        }

        if (program_mode && tokens[0].type == TOKEN_WHILE) {
            char condition[256] = {0};
            if (token_count >= 2) {
                strncpy(condition, tokens[1].value, sizeof(condition) - 1);
            }

            execute(tokens, token_count, current_line);
            free_tokens(tokens);

            if (!evaluate_condition(condition)) {
                int skip_line = current_line;
                while ((skip_line = prog_next_line(skip_line)) >= 0) {
                    const char *skip_text = prog_get_line(skip_line);
                    if (skip_text) {
                        int skip_tc = 0;
                        Token* skip_tokens = tokenize(skip_text, &skip_tc);
                        if (skip_tc > 0 && skip_tokens[0].type == TOKEN_WEND) {
                            free_tokens(skip_tokens);
                            loop_pop();
                            if (next_line_out) {
                                *next_line_out = prog_next_line(skip_line);
                            }
                            return EXEC_LINE_JUMP;
                        }
                        free_tokens(skip_tokens);
                    }
                }
                loop_pop();
                if (next_line_out) {
                    *next_line_out = prog_next_line(current_line);
                }
                return EXEC_LINE_JUMP;
            }

            if (next_line_out) {
                *next_line_out = prog_next_line(current_line);
            }
            return EXEC_LINE_JUMP;
        }

        int exec_line = program_mode ? current_line : -1;
        int result = execute(tokens, token_count, exec_line);
        free_tokens(tokens);

        if (should_stop_execution()) {
            if (next_line_out) {
                *next_line_out = -1;
            }
            return EXEC_LINE_BREAK;
        }

        if (result == -2) {
            if (next_line_out) {
                *next_line_out = -1;
            }
            return EXEC_LINE_END;
        } else if (result >= 0) {
            if (next_line_out) {
                *next_line_out = result;
            }
            return EXEC_LINE_JUMP;
        }
    }

    return EXEC_LINE_CONTINUE;
}

// Helper to check if a string is a number (integer or float)
static int is_number(const char *str) {
    if (!str || *str == '\0') return 0;
    char *endptr;
    strtof(str, &endptr);
    // Accept if we parsed something and reached end of string
    // Also handle cases with just a sign or decimal point
    return *endptr == '\0' && endptr != str;
}

// Helper to compare values
static int compare(const char *left, const char *op, const char *right) {
    // Try to convert to numbers
    int left_num = atoi(left);
    int right_num = atoi(right);
    
    if (strcmp(op, "=") == 0 || strcmp(op, "==") == 0) {
        return strcmp(left, right) == 0;
    } else if (strcmp(op, "<") == 0) {
        return left_num < right_num;
    } else if (strcmp(op, ">") == 0) {
        return left_num > right_num;
    } else if (strcmp(op, "<=") == 0) {
        return left_num <= right_num;
    } else if (strcmp(op, ">=") == 0) {
        return left_num >= right_num;
    } else if (strcmp(op, "<>") == 0) {
        return strcmp(left, right) != 0;
    }
    return 0;
}

static void execute_print(Token* tokens, int token_count) {
    if (token_count < 2) return;
    
    // Process all print arguments (tokens 1 through token_count-1)
    for (int i = 1; i < token_count; i++) {
        const char *arg = tokens[i].value;
        
        // Check if it's a string literal (was originally quoted)
        if (tokens[i].is_string_literal) {
            printf("%s", arg);
        }
        // Check if it's a string variable (ends with $)
        else if (strlen(arg) > 0 && arg[strlen(arg) - 1] == '$') {
            const char *val = var_get(arg);
            if (val != NULL) {
                printf("%s", val);
            } else {
                printf("?UNDEFINED VARIABLE: %s", arg);
            }
        } 
        // Check if it's a number literal
        else if (is_number(arg)) {
            printf("%s", arg);
        }
        // Check if it's a variable name (single word, no quotes, no spaces, no operators)
        else if (strlen(arg) > 0 && arg[0] != '"' && !strchr(arg, ' ') && !strchr(arg, '+') && !strchr(arg, '-') && !strchr(arg, '*') && !strchr(arg, '/') && !strchr(arg, '(') && !strchr(arg, ')')) {
            const char *val = var_get(arg);
            if (val != NULL) {
                printf("%s", val);
            } else {
                printf("?UNDEFINED VARIABLE: %s", arg);
            }
        }
        // Check if it contains mathematical operators - evaluate as expression
        else if (strchr(arg, '+') || strchr(arg, '-') || strchr(arg, '*') || strchr(arg, '/') || strchr(arg, '(') || strchr(arg, ')')) {
            int error = 0;
            float result = evaluate_math_expression(arg, &error);
            if (error) {
                printf("?MATH ERROR");
            } else {
                // Print as integer if it's a whole number, otherwise as float
                if (result == (int)result) {
                    printf("%d", (int)result);
                } else {
                    printf("%g", result);
                }
            }
        }
        // Otherwise it's a string literal (from quotes or spaces)
        else {
            printf("%s", arg);
        }
        
        // If this token doesn't have a semicolon, print newline after it
        // Otherwise continue on same line
        if (!tokens[i].has_semicolon && i == token_count - 1) {
            printf("\n");
        }
    }
}

static void execute_let(Token* tokens, int token_count) {
    if (token_count < 2) return;
    var_set(tokens[1].value);
}

static void trim_whitespace(char *str) {
    if (!str) return;
    char *start = str;
    while (*start && isspace((unsigned char)*start)) {
        start++;
    }
    char *end = start + strlen(start);
    while (end > start && isspace((unsigned char)*(end - 1))) {
        end--;
    }
    size_t len = (size_t)(end - start);
    if (start != str) {
        memmove(str, start, len);
    }
    str[len] = '\0';
}

static char* find_operator(char *str, const char *op) {
    if (!str || !op) return NULL;
    size_t op_len = strlen(op);
    if (op_len == 0) return NULL;

    int in_quote = 0;
    for (char *p = str; *p; ++p) {
        if (*p == '"') {
            in_quote = !in_quote;
            continue;
        }
        if (!in_quote && strncmp(p, op, op_len) == 0) {
            return p;
        }
    }
    return NULL;
}

static int evaluate_simple_condition(const char *condition) {
    char buffer[256];
    strncpy(buffer, condition, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';

    trim_whitespace(buffer);

    char op[3] = {0};
    char *op_pos = NULL;
    const char *right_part = NULL;

    if ((op_pos = find_operator(buffer, "<=")) != NULL) {
        strcpy(op, "<=");
        right_part = op_pos + 2;
    } else if ((op_pos = find_operator(buffer, ">=")) != NULL) {
        strcpy(op, ">=");
        right_part = op_pos + 2;
    } else if ((op_pos = find_operator(buffer, "<>")) != NULL) {
        strcpy(op, "<>");
        right_part = op_pos + 2;
    } else if ((op_pos = find_operator(buffer, "<")) != NULL) {
        strcpy(op, "<");
        right_part = op_pos + 1;
    } else if ((op_pos = find_operator(buffer, ">")) != NULL) {
        strcpy(op, ">");
        right_part = op_pos + 1;
    } else if ((op_pos = find_operator(buffer, "=")) != NULL) {
        strcpy(op, "=");
        right_part = op_pos + 1;
    } else {
        return 0;
    }

    *op_pos = '\0';

    char left[256];
    char right[256];
    strncpy(left, buffer, sizeof(left) - 1);
    left[sizeof(left) - 1] = '\0';
    strncpy(right, right_part, sizeof(right) - 1);
    right[sizeof(right) - 1] = '\0';

    trim_whitespace(left);
    trim_whitespace(right);

    if (right[0] == '"') {
        size_t len = strlen(right);
        memmove(right, right + 1, len);
        len = strlen(right);
        if (len > 0 && right[len - 1] == '"') {
            right[len - 1] = '\0';
        }
    }

    const char *left_val = var_get(left);
    const char *right_val = var_get(right);

    if (left_val == NULL) left_val = left;
    if (right_val == NULL) right_val = right;

    return compare(left_val, op, right_val);
}

static int evaluate_condition(const char *condition) {
    if (!condition) return 0;

    char buffer[256];
    strncpy(buffer, condition, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    trim_whitespace(buffer);

    int len = strlen(buffer);
    int in_quote = 0;

    // Search for OR (lowest precedence)
    for (int i = 0; i < len; i++) {
        char c = buffer[i];
        if (c == '"') {
            in_quote = !in_quote;
            continue;
        }
        if (in_quote) continue;

        if ((i == 0 || isspace((unsigned char)buffer[i - 1])) &&
            (i + 2 <= len) &&
            (toupper((unsigned char)buffer[i]) == 'O') &&
            (toupper((unsigned char)buffer[i + 1]) == 'R') &&
            (i + 2 == len || isspace((unsigned char)buffer[i + 2]))) {
            buffer[i] = '\0';
            const char *right = buffer + i + 2;
            return evaluate_condition(buffer) || evaluate_condition(right);
        }
    }

    // Search for AND
    in_quote = 0;
    for (int i = 0; i < len; i++) {
        char c = buffer[i];
        if (c == '"') {
            in_quote = !in_quote;
            continue;
        }
        if (in_quote) continue;

        if ((i == 0 || isspace((unsigned char)buffer[i - 1])) &&
            (i + 3 <= len) &&
            (toupper((unsigned char)buffer[i]) == 'A') &&
            (toupper((unsigned char)buffer[i + 1]) == 'N') &&
            (toupper((unsigned char)buffer[i + 2]) == 'D') &&
            (i + 3 == len || isspace((unsigned char)buffer[i + 3]))) {
            buffer[i] = '\0';
            const char *right = buffer + i + 3;
            return evaluate_condition(buffer) && evaluate_condition(right);
        }
    }

    return evaluate_simple_condition(buffer);
}

static int execute_if(Token* tokens, int token_count, int line_num) {
    if (token_count < 2) return -1;
    
    const char *condition = tokens[1].value;
    
    if (evaluate_condition(condition)) {
        // Condition is true, execute the command after THEN
        if (token_count >= 4) {
            // tokens[3] contains the command (e.g., "PRINT \"x is big\"" or "GOTO 100")
            // Re-tokenize and execute it
            int cmd_token_count;
            Token* cmd_tokens = tokenize(tokens[3].value, &cmd_token_count);
            int result = execute(cmd_tokens, cmd_token_count, line_num);
            free_tokens(cmd_tokens);
            return result;  // Return any jump result (GOTO, etc)
        }
        return -1;
    }
    return -1;
}

static void execute_input(Token* tokens, int token_count) {
    if (token_count < 3) {
        printf("INPUT requires a variable\n");
        return;
    }
    
    const char *prompt = tokens[1].value;
    const char *var_name = tokens[2].value;
    
    // Determine if this is a numeric variable (doesn't end with $)
    bool is_string_var = (strlen(var_name) > 0 && var_name[strlen(var_name) - 1] == '$');
    
    while (true) {  // Loop until valid input is received
        // Print the prompt
        printf("%s", prompt);
        fflush(stdout);
        
        // Read user input
        char input_line[256];
        size_t len = 0;
        
        while (true) {
            int c = getchar_timeout_us(0);
            if (c == PICO_ERROR_TIMEOUT) continue;
            
            // Check for Ctrl+C (0x03) - interrupt execution
            if (c == 0x03) {
                printf("\n^C\n");
                fflush(stdout);
                execution_interrupted = 1;  // Set global interrupt flag
                // Exit with empty string to abort INPUT
                input_line[0] = '\0';
                len = 0;
                // Set variable to 0 or "" and return to abort gracefully
                char assign_str[512];
                if (is_string_var) {
                    snprintf(assign_str, sizeof(assign_str), "%s=\"\"", var_name);
                } else {
                    snprintf(assign_str, sizeof(assign_str), "%s=0", var_name);
                }
                var_set(assign_str);
                return;  // Return to calling code
            }
            
            // Handle Enter key: 0x0D (CR) or 0x0A (LF)
            if (c == 0x0D || c == 0x0A || c == '\r' || c == '\n') {
                putchar('\n');
                input_line[len] = '\0';
                break;
            }
            
            if (c == 0x08 || c == 0x7F) {  // Backspace
                if (len > 0) {
                    len--;
                    putchar('\b'); putchar(' '); putchar('\b');
                    fflush(stdout);
                }
                continue;
            }
            
            if (c >= 32 && c <= 126) {  // Printable ASCII
                if (len < 255) {
                    input_line[len++] = (char)c;
                    putchar(c);
                    fflush(stdout);
                }
            }
        }
        
        // Trim leading and trailing spaces
        char trimmed[256];
        int start = 0, end = len - 1;
        while (start < len && input_line[start] == ' ') start++;
        while (end >= start && input_line[end] == ' ') end--;
        
        int trim_len = 0;
        for (int i = start; i <= end; i++) {
            trimmed[trim_len++] = input_line[i];
        }
        trimmed[trim_len] = '\0';
        
        // Check if input is empty
        if (trim_len == 0) {
            // For testing purposes, allow empty input
            // Empty = 0 for numbers, empty string for strings
            char assign_str[512];
            if (is_string_var) {
                snprintf(assign_str, sizeof(assign_str), "%s=\"\"", var_name);
            } else {
                snprintf(assign_str, sizeof(assign_str), "%s=0", var_name);
            }
            var_set(assign_str);
            return;
        }
        
        // For numeric variables, validate that input is a number
        if (!is_string_var) {
            bool valid = true;
            int i = 0;
            
            // Allow leading minus sign
            if (trimmed[i] == '-') i++;
            
            // Must have at least one digit
            if (trimmed[i] == '\0') {
                valid = false;
            } else {
                // Check all remaining characters are digits
                while (trimmed[i] != '\0') {
                    if (trimmed[i] < '0' || trimmed[i] > '9') {
                        valid = false;
                        break;
                    }
                    i++;
                }
            }
            
            if (!valid) {
                printf("?SN ERROR\n");
                continue;  // Re-prompt
            }
        }
        
        // Valid input received, set the variable
        char assignment[512];
        snprintf(assignment, sizeof(assignment), "%s=%s", var_name, trimmed);
        var_set(assignment);
        break;  // Exit the retry loop
    }
}

static void execute_help(Token* tokens, int token_count) {
    // HELP with no argument shows all categories
    if (token_count < 2 || strlen(tokens[1].value) == 0) {
        help_list_categories();
        return;
    }
    
    // HELP <topic> shows help for that topic and appends code to program
    const char *topic = tokens[1].value;
    const HelpEntry *entry = help_get(topic);
    
    if (entry == NULL) {
        printf("?UNKNOWN HELP TOPIC: %s\n", topic);
        return;
    }
    
    // Print the help entry
    help_print(entry);
    
    // Append the code lines to the program
    help_append_to_program(entry);
    
    printf("Code snippet added to program.\n");
    printf("Type LIST to see your program.\n");
}

int execute(Token* tokens, int token_count, int line_num) {
    if (token_count == 0) return -1;
    
    // Direct Mode Commands - only allowed when line_num == -1 (immediate mode)
    if (line_num >= 0) {  // Program Mode - reject direct commands
        switch (tokens[0].type) {
            case TOKEN_LIST:
            case TOKEN_RUN:
            case TOKEN_NEW:
            case TOKEN_SAVE:
            case TOKEN_LOAD:
            case TOKEN_DIR:
            case TOKEN_RM:
            case TOKEN_DELETE:
            case TOKEN_FORMAT:
            case TOKEN_CD:
            case TOKEN_PWD:
            case TOKEN_MKDIR:
            case TOKEN_RMDIR:
            case TOKEN_DRIVES:
            case TOKEN_CLS:
            case TOKEN_HELP:
                printf("?ILLEGAL DIRECT COMMAND\n");
                return -1;
            default:
                break;
        }
    }
    
    switch (tokens[0].type) {
        case TOKEN_PRINT:
            execute_print(tokens, token_count);
            break;
        case TOKEN_LET:
            execute_let(tokens, token_count);
            break;
        case TOKEN_IF:
            {
                int jump_result = execute_if(tokens, token_count, line_num);
                if (jump_result == -2) {
                    return -2;
                }
                if (jump_result >= 0) {
                    return jump_result;  // Propagate GOTO/GOSUB result
                }
            }
            break;
        case TOKEN_INPUT:
            execute_input(tokens, token_count);
            break;
        case TOKEN_REM:
            // Comments do nothing
            break;
        case TOKEN_FOR:
            // FOR i=1 TO 10
            // tokens[1].value has the full expression "i=1"
            // tokens[2] is TO
            // tokens[3].value is the end value
            if (token_count >= 4) {
                char var_name[50];
                int start_val, end_val;
                
                // Parse "i=1" from tokens[1].value
                const char *eq = strchr(tokens[1].value, '=');
                if (eq) {
                    strncpy(var_name, tokens[1].value, eq - tokens[1].value);
                    var_name[eq - tokens[1].value] = '\0';
                    // Trim trailing spaces from variable name
                    int vnlen = strlen(var_name);
                    while (vnlen > 0 && (var_name[vnlen - 1] == ' ' || var_name[vnlen - 1] == '\t')) {
                        var_name[--vnlen] = '\0';
                    }
                    start_val = atoi(eq + 1);
                    end_val = atoi(tokens[3].value);
                    
                    loop_push_for(var_name, start_val, end_val, line_num);
                }
            }
            break;
        case TOKEN_NEXT:
            // NEXT or NEXT i
            {
                int jump_line = loop_for_next();
                if (jump_line >= 0) {
                    return jump_line;  // Jump back to FOR line
                }
            }
            break;
        case TOKEN_WHILE:
            // WHILE condition
            // tokens[1].value has the condition
            if (token_count >= 2) {
                loop_push_while(line_num);
            }
            break;
        case TOKEN_WEND:
            // Re-evaluate WHILE condition and jump or pop
            {
                int while_line = loop_get_start_line();
                if (while_line >= 0) {
                    // Get the WHILE line and evaluate its condition
                    const char *while_text = prog_get_line(while_line);
                    if (while_text) {
                        int tc;
                        Token* while_toks = tokenize(while_text, &tc);
                        if (tc >= 2 && while_toks[0].type == TOKEN_WHILE) {
                            // Evaluate the condition in while_toks[1].value
                            if (evaluate_condition(while_toks[1].value)) {
                                free_tokens(while_toks);
                                return while_line;  // Jump back to WHILE
                            }
                        }
                        free_tokens(while_toks);
                    }
                }
                loop_pop();  // Exit the WHILE loop
            }
            break;
        case TOKEN_LIST:
            prog_list();
            break;
        case TOKEN_RUN:
            {
                int run_line = prog_first_line();
                while (run_line >= 0) {
                    // Check for Ctrl-C interrupt
                    if (should_stop_execution()) {
                        printf("\nBREAK\n");
                        break;
                    }

                    // Also check for Ctrl-C by reading input without blocking
                    int c = getchar_timeout_us(0);
                    if (c == 0x03) {  // Ctrl+C
                        printf("\nBREAK\n");
                        execution_interrupted = 0;
                        break;
                    }

                    const char *line_text = prog_get_line(run_line);
                    if (line_text) {
                        int next_line = -1;
                        int action = execute_line_segments(line_text, run_line, &next_line, true);

                        if (action == EXEC_LINE_END) {
                            break;
                        } else if (action == EXEC_LINE_BREAK) {
                            break;
                        } else if (action == EXEC_LINE_JUMP) {
                            run_line = next_line;
                            continue;
                        }
                    }

                    run_line = prog_next_line(run_line);
                }
            }
            break;
        case TOKEN_NEW:
            prog_clear();
            printf("Program cleared\n");
            break;
        case TOKEN_AUTO:
            // AUTO command: enter automatic line numbering mode
            if (token_count >= 2) {
                int start_line = atoi(tokens[1].value);
                prog_set_auto_line_number(start_line);
            } else {
                prog_set_auto_line_number(10);
            }
            printf("AUTO mode: Enter lines (CTRL+C to exit)\n");
            // This will be handled in main.c
            break;
        case TOKEN_SNIPPET:
            // SNIPPET command: add code snippet from help to the program
            if (token_count >= 2) {
                const HelpEntry *entry = help_get(tokens[1].value);
                if (entry) {
                    help_append_to_program(entry);
                    printf("Code snippet added to program.\n");
                } else {
                    printf("?SNIPPET NOT FOUND: %s\n", tokens[1].value);
                }
            } else {
                printf("?SNIPPET requires a command name\n");
            }
            break;
        case TOKEN_RENUM:
            // RENUM command: renumber all program lines
            if (token_count >= 2) {
                int step = atoi(tokens[1].value);
                if (step <= 0) {
                    printf("?INVALID STEP VALUE\n");
                } else {
                    prog_renumber(step);
                    printf("Program renumbered\n");
                }
            } else {
                prog_renumber(10);
                printf("Program renumbered\n");
            }
            break;
        case TOKEN_EDIT:
            // EDIT command: bring up a line for editing
            if (token_count >= 2) {
                int line_num = atoi(tokens[1].value);
                const char *line_text = prog_get_line(line_num);
                if (line_text) {
                    set_edit_mode(line_num, line_text);
                } else {
                    printf("?LINE NOT FOUND\n");
                }
            } else {
                printf("?EDIT requires a line number\n");
            }
            break;
        case TOKEN_RANDOMIZE:
            // RANDOMIZE command: seed the random number generator
            if (token_count >= 2) {
                // RANDOMIZE with specific seed
                rnd_seed = atoi(tokens[1].value);
            } else {
                // RANDOMIZE without argument: use timer as seed
                rnd_seed = (unsigned int)time_us_64();
            }
            srand(rnd_seed);
            printf("Random seed initialized\n");
            break;
        case TOKEN_SAVE:
            if (token_count >= 2) {
                fs_save(tokens[1].value);
            } else {
                printf("?FILENAME REQUIRED\n");
            }
            break;
        case TOKEN_LOAD:
            if (token_count >= 2) {
                fs_load(tokens[1].value);
            } else {
                printf("?FILENAME REQUIRED\n");
            }
            break;
        case TOKEN_DIR:
            if (token_count >= 2) {
                fs_dir(tokens[1].value);
            } else {
                fs_dir(NULL);
            }
            break;
        case TOKEN_RM:
        case TOKEN_DELETE:
            if (token_count >= 2) {
                fs_rm(tokens[1].value);
            } else {
                printf("?FILENAME REQUIRED\n");
            }
            break;
        case TOKEN_FORMAT:
            if (token_count >= 3 && strcmp(tokens[2].value, "YES") == 0) {
                // Parse drive number from "0:" or "1:"
                uint8_t drive = tokens[1].value[0] - '0';
                fs_format(drive);
            } else {
                printf("?FORMAT requires drive and YES confirmation\n");
                printf("Example: FORMAT \"0:\" YES\n");
            }
            break;
        case TOKEN_CD:
            if (token_count >= 2) {
                fs_cd(tokens[1].value);
            } else {
                printf("?PATH REQUIRED\n");
            }
            break;
        case TOKEN_PWD:
            printf("%s\n", fs_get_path());
            break;
        case TOKEN_MKDIR:
            if (token_count >= 2) {
                fs_mkdir(tokens[1].value);
            } else {
                printf("?DIRECTORY NAME REQUIRED\n");
            }
            break;
        case TOKEN_RMDIR:
            if (token_count >= 2) {
                fs_rmdir(tokens[1].value);
            } else {
                printf("?DIRECTORY NAME REQUIRED\n");
            }
            break;
        case TOKEN_DRIVES:
            fs_drives();
            break;
        case TOKEN_CLS:
            // Clear screen using ANSI escape sequence
            // ESC[2J = clear screen, ESC[H = move cursor to home (top-left)
            printf("\x1B[2J\x1B[H");
            fflush(stdout);
            break;
        case TOKEN_GOSUB: {
            // GOSUB target_line
            if (token_count < 2) {
                printf("?GOSUB REQUIRES LINE NUMBER\n");
                break;
            }
            
            int target_line = atoi(tokens[1].value);
            if (target_line <= 0) {
                printf("?INVALID LINE NUMBER\n");
                break;
            }
            
            // Find next line number for return address
            int return_addr = prog_next_line(line_num);
            
            if (return_addr <= 0) {
                printf("?NO RETURN ADDRESS\n");
                break;
            }
            
            // Push return address and jump to target
            gosub_push_return(return_addr);
            return target_line;  // Jump to GOSUB target
            break;
        }
        case TOKEN_ON: {
            // ON expression GOTO line1, line2, line3, ...
            // ON expression GOSUB line1, line2, line3, ...
            if (token_count < 4) {
                printf("?ON REQUIRES EXPRESSION, GOTO/GOSUB, AND LINE NUMBERS\n");
                break;
            }
            
                 // Evaluate the expression
                 int error = 0;
                 float result = evaluate_math_expression(tokens[1].value, &error);
            
            if (error) {
                printf("?INVALID EXPRESSION IN ON\n");
                break;
            }
            
            int index = (int)result;  // Convert to integer for indexing (1-based)
            
            // Parse comma-separated line numbers from tokens[3].value
            char line_numbers[256];
            strcpy(line_numbers, tokens[3].value);
            
            int current_line = 0;
            int line_count = 0;
            int target_line = -1;
            
            // Parse the comma-separated line numbers
            char *ptr = line_numbers;
            char *end;
            
            while (*ptr && line_count < 100) {  // Safety limit
                // Skip whitespace
                while (*ptr == ' ' || *ptr == '\t') ptr++;
                
                // Parse number
                int line_num = (int)strtol(ptr, &end, 10);
                line_count++;
                
                // If this is the selected index (1-based), store it
                if (line_count == index) {
                    target_line = line_num;
                }
                
                // Move to next comma or end
                ptr = end;
                while (*ptr == ' ' || *ptr == '\t') ptr++;
                if (*ptr == ',') {
                    ptr++;
                } else if (*ptr == '\0') {
                    break;
                }
            }
            
            // If index out of range, just continue
            if (target_line == -1) {
                printf("?OUT OF RANGE IN ON\n");
                break;
            }
            
            // Validate target line exists
            if (prog_get_line(target_line) == NULL) {
                printf("?UNDEF'D STATEMENT %d\n", target_line);
                break;
            }
            
            // Check if it's GOTO or GOSUB
            if (tokens[2].type == TOKEN_GOSUB) {
                // Push return address for GOSUB
                // The return address should be the line after the current ON statement
                int return_addr = prog_next_line(line_num);
                if (return_addr <= 0) {
                    printf("?NO RETURN ADDRESS\n");
                    break;
                }
                gosub_push_return(return_addr);
                return target_line;  // Jump to GOSUB target
            } else {
                // GOTO
                return target_line;  // Jump to target line
            }
            break;
        }
        case TOKEN_GOTO: {
            // GOTO - unconditional jump to target line
            if (token_count < 2) {
                printf("?GOTO REQUIRES LINE NUMBER\n");
                break;
            }
            
            int target_line = atoi(tokens[1].value);
            
            // Validate target line exists
            if (prog_get_line(target_line) == NULL) {
                printf("?UNDEF'D STATEMENT %d\n", target_line);
                break;
            }
            
            return target_line;  // Jump to target line
            break;
        }
        case TOKEN_RETURN: {
            // RETURN - pop return address and jump back
            if (!gosub_has_return()) {
                printf("?RETURN WITHOUT GOSUB\n");
                break;
            }
            
            int return_addr = gosub_pop_return();
            return return_addr;  // Jump back to return address
            break;
        }
        case TOKEN_END: {
            // END - terminate program execution
            return -2;  // Special return code to signal end of program
            break;
        }
        case TOKEN_NOTE: {
            // NOTE filename text - save text to filename.txt
            if (token_count < 2) {
                printf("?NOTE REQUIRES FILENAME\n");
                break;
            }
            
            // Build filename with .txt extension
            char filename[128];
            snprintf(filename, sizeof(filename), "%s.txt", tokens[1].value);
            
            // Get text content (token[2] if exists, otherwise empty)
            const char *text = (token_count >= 3) ? tokens[2].value : "";
            
            // Save to file
            fs_write_note(filename, text);
            break;
        }
        case TOKEN_HELP:
            execute_help(tokens, token_count);
            break;
        case TOKEN_BEEP: {
            // BEEP - sound the speaker
            printf("\a");  // Bell character for terminal beep
            break;
        }
        case TOKEN_CLEAR: {
            // CLEAR - clears variables (not program)
            var_clear_all();
            printf("Variables cleared\n");
            break;
        }
        case TOKEN_DIM: {
            // DIM array(size) or array(rows, cols)
            if (token_count < 2) {
                printf("?DIM REQUIRES ARRAY NAME\n");
                break;
            }
            // Array support would require array_declare function
            printf("(DIM arrays not yet implemented)\n");
            break;
        }
        case TOKEN_DATA: {
            // DATA statement - store data values for READ
            // This would need a data storage mechanism
            printf("(DATA statements not yet implemented)\n");
            break;
        }
        case TOKEN_READ: {
            // READ variable - read next data value
            printf("(READ not yet implemented)\n");
            break;
        }
        case TOKEN_RESTORE: {
            // RESTORE - reset DATA pointer to beginning
            printf("(RESTORE not yet implemented)\n");
            break;
        }
        case TOKEN_OPEN: {
            // OPEN "file.txt" FOR OUTPUT AS 1
            if (token_count < 2) {
                printf("?OPEN REQUIRES FILE SPECIFICATION\n");
                break;
            }
            printf("(File I/O not yet implemented)\n");
            break;
        }
        case TOKEN_CLOSE: {
            // CLOSE file_number
            if (token_count < 2) {
                printf("(CLOSE - file I/O not yet implemented)\n");
                break;
            }
            printf("(File I/O not yet implemented)\n");
            break;
        }
        case TOKEN_PRINT_HASH: {
            // PRINT# file_number, data
            if (token_count < 2) {
                printf("(PRINT# - file I/O not yet implemented)\n");
                break;
            }
            printf("(File I/O not yet implemented)\n");
            break;
        }
        case TOKEN_INPUT_HASH: {
            // INPUT# file_number, variable
            if (token_count < 2) {
                printf("(INPUT# - file I/O not yet implemented)\n");
                break;
            }
            printf("(File I/O not yet implemented)\n");
            break;
        }
        case TOKEN_UNKNOWN: {
            // Try to evaluate as an expression if it looks like one
            if (token_count > 0 && strlen(tokens[0].value) > 0) {
                int error = 0;
                float result = evaluate_math_expression(tokens[0].value, &error);
                
                if (!error) {
                    // Successfully evaluated as expression
                    if (result == (int)result) {
                        printf("%d\n", (int)result);
                    } else {
                        printf("%g\n", result);
                    }
                } else {
                    // Not a valid expression or command
                    printf("?Unknown command or invalid expression: %s\n", tokens[0].value);
                }
            } else {
                printf("?Unknown command\n");
            }
            break;
        }
        default:
            break;
    }
    
    return -1;  // Continue sequentially
}

void execute_line_direct(const char *line_text) {
    execute_line_segments(line_text, -1, NULL, false);
}
