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
#include "pico/stdlib.h"

// Forward declaration of math evaluator (from math.c)
// We can't include math.h due to Token struct name collision
extern float evaluate_math_expression(const char *expr, int *error);

// Global interrupt flag for Ctrl-C handling
volatile int execution_interrupted = 0;

// Check if execution should be interrupted and clear the flag
int should_stop_execution(void) {
    if (execution_interrupted) {
        execution_interrupted = 0;
        return 1;
    }
    return 0;
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

static int evaluate_condition(const char *condition) {
    // Parse condition like "x>5" or "x$=\"hello\""
    char left[256], op[10], right[256];
    
    // Find comparison operator
    if (strchr(condition, '<')) {
        if (strstr(condition, "<=")) {
            sscanf(condition, "%[^<]<=%s", left, right);
            strcpy(op, "<=");
        } else if (strstr(condition, "<>")) {
            sscanf(condition, "%[^<]<>%s", left, right);
            strcpy(op, "<>");
        } else {
            sscanf(condition, "%[^<]<%s", left, right);
            strcpy(op, "<");
        }
    } else if (strchr(condition, '>')) {
        if (strstr(condition, ">=")) {
            sscanf(condition, "%[^>]>=%s", left, right);
            strcpy(op, ">=");
        } else {
            sscanf(condition, "%[^>]>%s", left, right);
            strcpy(op, ">");
        }
    } else if (strchr(condition, '=')) {
        sscanf(condition, "%[^=]=%s", left, right);
        strcpy(op, "=");
    } else {
        return 0;
    }
    
    // Trim spaces from left
    int len = strlen(left);
    while (len > 0 && left[len - 1] == ' ') left[--len] = '\0';
    
    // Trim spaces from right
    len = strlen(right);
    while (len > 0 && right[len - 1] == ' ') right[--len] = '\0';
    
    // Remove quotes from right if present
    if (right[0] == '"') {
        memmove(right, right + 1, strlen(right));
        len = strlen(right);
        if (len > 0 && right[len - 1] == '"') {
            right[len - 1] = '\0';
        }
    }
    
    // Get variable values if they're variables
    const char *left_val = var_get(left);
    const char *right_val = var_get(right);
    
    // If not a variable, use the literal value
    if (left_val == NULL) left_val = left;
    if (right_val == NULL) right_val = right;
    
    return compare(left_val, op, right_val);
}

static void execute_if(Token* tokens, int token_count) {
    if (token_count < 2) return;
    
    const char *condition = tokens[1].value;
    
    if (evaluate_condition(condition)) {
        // Condition is true, execute the command after THEN
        if (token_count >= 4) {
            // tokens[3] contains the command (e.g., "PRINT \"x is big\"")
            // Re-tokenize and execute it
            int cmd_token_count;
            Token* cmd_tokens = tokenize(tokens[3].value, &cmd_token_count);
            execute(cmd_tokens, cmd_token_count, -1);  // Immediate mode
            free_tokens(cmd_tokens);
        }
    }
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
            execute_if(tokens, token_count);
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
                    // Check for Ctrl-C interrupt by reading input without blocking
                    int c = getchar_timeout_us(0);
                    if (c == 0x03) {  // Ctrl+C
                        printf("\nBREAK\n");
                        execution_interrupted = 0;
                        break;
                    }
                    
                    const char *line_text = prog_get_line(run_line);
                    if (line_text) {
                        int tc;
                            Token* toks = tokenize(line_text, &tc);
                        
                        // Special handling for FOR and WHILE loops
                        if (tc > 0 && toks[0].type == TOKEN_FOR) {
                            // Execute FOR to set up the loop, but pass next line as body start
                            int next_line_for_body = prog_next_line(run_line);
                            execute(toks, tc, next_line_for_body);
                            free_tokens(toks);
                            run_line = prog_next_line(run_line);
                            goto continue_run;
                        }
                        if (tc > 0 && toks[0].type == TOKEN_WHILE) {
                            // Save condition and execute WHILE to push onto stack
                            char condition[256];
                            strcpy(condition, toks[1].value);
                            
                            execute(toks, tc, run_line);
                            free_tokens(toks);
                            
                            // Evaluate condition
                            if (!evaluate_condition(condition)) {
                                // Condition false, skip to line after WEND
                                int skip_line = run_line;
                                while ((skip_line = prog_next_line(skip_line)) >= 0) {
                                    const char *skip_text = prog_get_line(skip_line);
                                    if (skip_text) {
                                        int skip_tc;
                                        Token* skip_toks = tokenize(skip_text, &skip_tc);
                                        if (skip_tc > 0 && skip_toks[0].type == TOKEN_WEND) {
                                            free_tokens(skip_toks);
                                            loop_pop();
                                            run_line = prog_next_line(skip_line);
                                            goto continue_run;
                                        }
                                        free_tokens(skip_toks);
                                    }
                                }
                                loop_pop();
                                run_line = prog_next_line(run_line);
                                goto continue_run;
                            }
                            // Condition true, enter loop body
                            run_line = prog_next_line(run_line);
                            goto continue_run;
                        }
                        int next_line = execute(toks, tc, run_line);
                        free_tokens(toks);
                        if (next_line == -2) {
                            // END statement - terminate program execution
                            break;
                        } else if (next_line >= 0) {
                            run_line = next_line;  // Jump to specified line
                        } else {
                            run_line = prog_next_line(run_line);  // Continue sequentially
                        }
                        continue_run:;
                    } else {
                        run_line = prog_next_line(run_line);
                    }
                }
            }
            break;
        case TOKEN_NEW:
            prog_clear();
            printf("Program cleared\n");
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
        case TOKEN_UNKNOWN:
            printf("Unknown command\n");
            break;
        default:
            break;
    }
    
    return -1;  // Continue sequentially
}
