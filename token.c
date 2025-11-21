#include "token.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

// Convert a string to uppercase
static void to_upper(char *str) {
    while (*str) {
        *str = toupper(*str);
        str++;
    }
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

Token* tokenize(const char *line, int *token_count) {
    Token* tokens = malloc(sizeof(Token) * 10);
    *token_count = 0;
    
    // Initialize tokens
    memset(tokens, 0, sizeof(Token) * 10);
    
    // Skip leading whitespace
    while (*line == ' ' || *line == '\t') {
        line++;
    }
    
    // Make a working copy so we can convert to uppercase
    char command[256];
    strcpy(command, line);
    to_upper(command);
    
    // Check for PRINT
    if (strncmp(command, "PRINT", 5) == 0) {
        tokens[0].type = TOKEN_PRINT;
        strcpy(tokens[0].value, "PRINT");
        *token_count = 1;
        
        // Get the argument (what to print)
        line += 5;  // Skip "PRINT"
        while (*line == ' ' || *line == '\t') {
            line++;  // Skip spaces
        }
        
        // Parse print items - separated by spaces or semicolons
        while (*line != '\0' && *token_count < 10) {
            // Handle quoted strings: "hello"
            if (*line == '"') {
                line++;  // Skip opening quote
                char *dest = tokens[*token_count].value;
                int found_closing = 0;
                while (*line) {
                    if (*line == '"') {
                        found_closing = 1;
                        line++;  // Skip closing quote
                        break;
                    }
                    *dest++ = *line++;
                }
                *dest = '\0';  // Null terminate
                
                // Check if we found the closing quote
                if (!found_closing) {
                    tokens[0].type = TOKEN_UNKNOWN;
                    strcpy(tokens[0].value, "SYNTAX ERROR: missing closing quote");
                    *token_count = 1;
                    return tokens;
                }
                
                tokens[*token_count].type = TOKEN_PRINT;
                tokens[*token_count].is_string_literal = 1;  // Mark as string literal
                tokens[*token_count].has_semicolon = 0;  // Reset semicolon flag
                (*token_count)++;
                
                // Check for semicolon after quoted string
                while (*line == ' ' || *line == '\t') {
                    line++;
                }
                if (*line == ';') {
                    tokens[*token_count - 1].has_semicolon = 1;  // Mark previous token as having semicolon
                    line++;
                    while (*line == ' ' || *line == '\t') {
                        line++;
                    }
                }
            } else if (*line == ';') {
                // Skip semicolon and spaces
                line++;
                while (*line == ' ' || *line == '\t') {
                    line++;
                }
                continue;
            } else {
                // Unquoted variable or expression - read until space or semicolon or end
                char *dest = tokens[*token_count].value;
                while (*line && *line != ';' && *line != ' ' && *line != '\t') {
                    *dest++ = *line++;
                }
                *dest = '\0';
                
                if (strlen(tokens[*token_count].value) > 0) {
                    tokens[*token_count].type = TOKEN_PRINT;
                    tokens[*token_count].has_semicolon = 0;  // Reset semicolon flag
                    (*token_count)++;
                    
                    // Check for semicolon after unquoted item
                    while (*line == ' ' || *line == '\t') {
                        line++;
                    }
                    if (*line == ';') {
                        tokens[*token_count - 1].has_semicolon = 1;  // Mark previous token as having semicolon
                        line++;
                        while (*line == ' ' || *line == '\t') {
                            line++;
                        }
                    }
                }
            }
        }
    }
    // Check for LET
    else if (strncmp(command, "LET", 3) == 0) {
        tokens[0].type = TOKEN_LET;
        strcpy(tokens[0].value, "LET");
        *token_count = 1;
        
        // Get variable assignment: LET x=10 or LET x$="hello"
        line += 3;  // Skip "LET"
        while (*line == ' ' || *line == '\t') {
            line++;  // Skip spaces
        }
        
        // Store the rest as the assignment
        if (*line != '\0') {
            strcpy(tokens[1].value, line);
            tokens[1].type = TOKEN_LET;
            *token_count = 2;
        }
    }
    // Check for IF
    else if (strncmp(command, "IF", 2) == 0) {
        tokens[0].type = TOKEN_IF;
        strcpy(tokens[0].value, "IF");
        *token_count = 1;
        
        // Get the condition
        line += 2;  // Skip "IF"
        while (*line == ' ' || *line == '\t') {
            line++;
        }
        
        // Store condition until we find THEN
        char condition[256];
        char *cond_ptr = condition;
        while (*line && strncmp(line, "THEN", 4) != 0 && strncmp(line, "then", 4) != 0) {
            *cond_ptr++ = *line++;
        }
        *cond_ptr = '\0';
        
        // Trim trailing spaces from condition
        while (cond_ptr > condition && *(cond_ptr - 1) == ' ') {
            *(--cond_ptr) = '\0';
        }
        
        strcpy(tokens[1].value, condition);
        tokens[1].type = TOKEN_IF;
        *token_count = 2;
        
        // Check for THEN
        while (*line == ' ' || *line == '\t') {
            line++;
        }
        if (strncmp(line, "THEN", 4) == 0 || strncmp(line, "then", 4) == 0) {
            tokens[2].type = TOKEN_THEN;
            strcpy(tokens[2].value, "THEN");
            line += 4;
            while (*line == ' ' || *line == '\t') {
                line++;
            }
            
            // Store command after THEN
            if (*line != '\0') {
                strcpy(tokens[3].value, line);
                tokens[3].type = TOKEN_PRINT;  // Could be any command
                *token_count = 4;
            } else {
                *token_count = 3;
            }
        }
    }
    // Check for INPUT
    else if (strncmp(command, "INPUT", 5) == 0) {
        tokens[0].type = TOKEN_INPUT;
        strcpy(tokens[0].value, "INPUT");
        *token_count = 1;
        
        // Get INPUT arguments
        line += 5;  // Skip "INPUT"
        while (*line == ' ' || *line == '\t') {
            line++;  // Skip spaces
        }
        
        // Check if there's a quoted prompt: INPUT "msg";var
        char prompt[256] = "? ";
        char var_name[256] = "";
        
        if (*line == '"') {
            // Has a prompt
            line++;  // Skip opening quote
            char *dest = prompt;
            while (*line && *line != '"') {
                *dest++ = *line++;
            }
            *dest = '\0';  // Null terminate prompt
            
            if (*line == '"') line++;  // Skip closing quote
            
            // Skip spaces and find semicolon
            while (*line == ' ' || *line == '\t') {
                line++;
            }
            
            if (*line == ';' || *line == ',') {
                line++;  // Skip semicolon
                while (*line == ' ' || *line == '\t') {
                    line++;
                }
                // Get variable name
                strcpy(var_name, line);
            }
        } else {
            // No prompt, just variable name
            strcpy(var_name, line);
        }

        trim_whitespace(var_name);
        
        // Store prompt in token[1] and variable name in token[2]
        strcpy(tokens[1].value, prompt);
        tokens[1].type = TOKEN_INPUT;
        
        strcpy(tokens[2].value, var_name);
        tokens[2].type = TOKEN_INPUT;
        
        *token_count = 3;
    }
    // Check for REM (comment - ignore rest of line)
    else if (strncmp(command, "REM", 3) == 0) {
        tokens[0].type = TOKEN_REM;
        strcpy(tokens[0].value, "REM");
        *token_count = 1;
    }
    // Check for LIST
    else if (strncmp(command, "LIST", 4) == 0) {
        tokens[0].type = TOKEN_LIST;
        strcpy(tokens[0].value, "LIST");
        *token_count = 1;
    }
    // Check for RUN
    else if (strncmp(command, "RUN", 3) == 0) {
        tokens[0].type = TOKEN_RUN;
        strcpy(tokens[0].value, "RUN");
        *token_count = 1;
    }
    // Check for NEW
    else if (strncmp(command, "NEW", 3) == 0) {
        tokens[0].type = TOKEN_NEW;
        strcpy(tokens[0].value, "NEW");
        *token_count = 1;
    }
    // Check for FOR
    else if (strncmp(command, "FOR", 3) == 0) {
        tokens[0].type = TOKEN_FOR;
        strcpy(tokens[0].value, "FOR");
        line += 3;
        while (*line == ' ' || *line == '\t') line++;

        // Parse format: var=start TO end
        char *p = (char *)line;
        char left[256] = {0};
        char right[256] = {0};
        char *to_pos = NULL;
        while (*p) {
            if ((toupper(*p) == 'T') && (toupper(*(p+1)) == 'O') && (p == line || *(p-1) == ' ' || *(p-1) == '\t')) {
                to_pos = p;
                break;
            }
            p++;
        }
        if (to_pos) {
            // left part is from line to to_pos - 1
            size_t left_len = to_pos - line;
            strncpy(left, line, left_len);
            left[left_len] = '\0';
            // right part is after TO
            p = to_pos + 2; // skip 'TO'
            while (*p == ' ' || *p == '\t') p++;
            strncpy(right, p, sizeof(right)-1);

            // Trim trailing spaces from left
            char *end = left + strlen(left) - 1;
            while (end >= left && (*end == ' ' || *end == '\t')) {
                *end-- = '\0';
            }

            // Store in tokens
            strcpy(tokens[1].value, left);
            tokens[1].type = TOKEN_FOR;
            strcpy(tokens[2].value, "TO");
            tokens[2].type = TOKEN_FOR;
            strcpy(tokens[3].value, right);
            tokens[3].type = TOKEN_FOR;
            *token_count = 4;
        } else {
            // No TO found, store entire rest as single token
            strcpy(tokens[1].value, line);
            tokens[1].type = TOKEN_FOR;
            *token_count = 2;
        }
    }
    // Check for NEXT
    else if (strncmp(command, "NEXT", 4) == 0) {
        tokens[0].type = TOKEN_NEXT;
        strcpy(tokens[0].value, "NEXT");
        line += 4;
        while (*line == ' ' || *line == '\t') line++;
        
        // Store variable name (optional)
        if (*line != '\0') {
            strcpy(tokens[1].value, line);
            tokens[1].type = TOKEN_NEXT;
            *token_count = 2;
        } else {
            *token_count = 1;
        }
    }
    // Check for WHILE
    else if (strncmp(command, "WHILE", 5) == 0) {
        tokens[0].type = TOKEN_WHILE;
        strcpy(tokens[0].value, "WHILE");
        line += 5;
        while (*line == ' ' || *line == '\t') line++;
        
        // Store condition
        strcpy(tokens[1].value, line);
        tokens[1].type = TOKEN_WHILE;
        *token_count = 2;
    }
    // Check for WEND
    else if (strncmp(command, "WEND", 4) == 0) {
        tokens[0].type = TOKEN_WEND;
        strcpy(tokens[0].value, "WEND");
        *token_count = 1;
    }
    // Check for END
    else if (strncmp(command, "END", 3) == 0) {
        tokens[0].type = TOKEN_END;
        strcpy(tokens[0].value, "END");
        *token_count = 1;
    }
    // Check for NOTE
    else if (strncmp(command, "NOTE", 4) == 0) {
        tokens[0].type = TOKEN_NOTE;
        strcpy(tokens[0].value, "NOTE");
        *token_count = 1;
        
        // Get filename and text: NOTE filename text text text
        line += 4;  // Skip "NOTE"
        while (*line == ' ' || *line == '\t') line++;
        
        // Extract filename (first word)
        char *dest = tokens[1].value;
        while (*line && *line != ' ' && *line != '\t') {
            *dest++ = *line++;
        }
        *dest = '\0';
        
        if (strlen(tokens[1].value) > 0) {
            tokens[1].type = TOKEN_NOTE;
            *token_count = 2;
            
            // Skip spaces before text
            while (*line == ' ' || *line == '\t') line++;
            
            // Get remaining text
            if (*line != '\0') {
                strcpy(tokens[2].value, line);
                tokens[2].type = TOKEN_NOTE;
                *token_count = 3;
            }
        }
    }
    // Check for SAVE
    else if (strncmp(command, "SAVE", 4) == 0) {
        tokens[0].type = TOKEN_SAVE;
        strcpy(tokens[0].value, "SAVE");
        line += 4;
        while (*line == ' ' || *line == '\t') line++;
        
        // Get filename (quoted or unquoted)
        if (*line != '\0') {
            if (*line == '"') {
                // Quoted filename
                line++;
                char *dest = tokens[1].value;
                while (*line && *line != '"') {
                    *dest++ = *line++;
                }
                *dest = '\0';
            } else {
                // Unquoted filename
                strcpy(tokens[1].value, line);
            }
            tokens[1].type = TOKEN_SAVE;
            *token_count = 2;
        } else {
            *token_count = 1;
        }
    }
    // Check for LOAD
    else if (strncmp(command, "LOAD", 4) == 0) {
        tokens[0].type = TOKEN_LOAD;
        strcpy(tokens[0].value, "LOAD");
        line += 4;
        while (*line == ' ' || *line == '\t') line++;
        
        // Get filename (quoted or unquoted)
        if (*line != '\0') {
            if (*line == '"') {
                // Quoted filename
                line++;
                char *dest = tokens[1].value;
                while (*line && *line != '"') {
                    *dest++ = *line++;
                }
                *dest = '\0';
            } else {
                // Unquoted filename
                strcpy(tokens[1].value, line);
            }
            tokens[1].type = TOKEN_LOAD;
            *token_count = 2;
        } else {
            *token_count = 1;
        }
    }
    // Check for DIR
    else if (strncmp(command, "DIR", 3) == 0) {
        tokens[0].type = TOKEN_DIR;
        strcpy(tokens[0].value, "DIR");
        line += 3;
        while (*line == ' ' || *line == '\t') line++;
        
        if (*line != '\0') {
            if (*line == '"') {
                line++;
                char *dest = tokens[1].value;
                while (*line && *line != '"') {
                    *dest++ = *line++;
                }
                *dest = '\0';
            } else {
                strcpy(tokens[1].value, line);
            }
            tokens[1].type = TOKEN_DIR;
            *token_count = 2;
        } else {
            *token_count = 1;
        }
    }
    // Check for RMDIR (must come BEFORE RM check, since "RM" is a substring of "RMDIR")
    else if (strncmp(command, "RMDIR", 5) == 0) {
        tokens[0].type = TOKEN_RMDIR;
        strcpy(tokens[0].value, "RMDIR");
        line += 5;
        while (*line == ' ' || *line == '\t') line++;
        
        if (*line != '\0') {
            if (*line == '"') {
                line++;
                char *dest = tokens[1].value;
                while (*line && *line != '"') {
                    *dest++ = *line++;
                }
                *dest = '\0';
            } else {
                strcpy(tokens[1].value, line);
            }
            tokens[1].type = TOKEN_RMDIR;
            *token_count = 2;
        } else {
            *token_count = 1;
        }
    }
    // Check for RM (after RMDIR check)
    else if (strncmp(command, "RM", 2) == 0) {
        tokens[0].type = TOKEN_RM;
        strcpy(tokens[0].value, "RM");
        line += 2;
        while (*line == ' ' || *line == '\t') line++;
        
        if (*line != '\0') {
            if (*line == '"') {
                line++;
                char *dest = tokens[1].value;
                while (*line && *line != '"') {
                    *dest++ = *line++;
                }
                *dest = '\0';
            } else {
                strcpy(tokens[1].value, line);
            }
            tokens[1].type = TOKEN_RM;
            *token_count = 2;
        } else {
            *token_count = 1;
        }
    }
    // Check for DELETE (alias for RM)
    else if (strncmp(command, "DELETE", 6) == 0) {
        tokens[0].type = TOKEN_DELETE;
        strcpy(tokens[0].value, "DELETE");
        line += 6;
        while (*line == ' ' || *line == '\t') line++;
        
        if (*line != '\0') {
            if (*line == '"') {
                line++;
                char *dest = tokens[1].value;
                while (*line && *line != '"') {
                    *dest++ = *line++;
                }
                *dest = '\0';
            } else {
                strcpy(tokens[1].value, line);
            }
            tokens[1].type = TOKEN_DELETE;
            *token_count = 2;
        } else {
            *token_count = 1;
        }
    }
    // Check for FORMAT
    else if (strncmp(command, "FORMAT", 6) == 0) {
        tokens[0].type = TOKEN_FORMAT;
        strcpy(tokens[0].value, "FORMAT");
        line += 6;
        while (*line == ' ' || *line == '\t') line++;
        
        // Get drive ("0:" or "1:")
        if (*line != '\0') {
            if (*line == '"') {
                line++;
                char *dest = tokens[1].value;
                while (*line && *line != '"') {
                    *dest++ = *line++;
                }
                *dest = '\0';
            } else {
                // Parse drive and YES
                char *space = strchr(line, ' ');
                if (space) {
                    strncpy(tokens[1].value, line, space - line);
                    tokens[1].value[space - line] = '\0';
                    tokens[1].type = TOKEN_FORMAT;
                    
                    // Get YES confirmation
                    line = space + 1;
                    while (*line == ' ' || *line == '\t') line++;
                    strcpy(tokens[2].value, line);
                    tokens[2].type = TOKEN_FORMAT;
                    *token_count = 3;
                } else {
                    strcpy(tokens[1].value, line);
                    tokens[1].type = TOKEN_FORMAT;
                    *token_count = 2;
                }
                return tokens;
            }
            tokens[1].type = TOKEN_FORMAT;
            *token_count = 2;
        } else {
            *token_count = 1;
        }
    }
    // Check for CD
    else if (strncmp(command, "CD", 2) == 0) {
        tokens[0].type = TOKEN_CD;
        strcpy(tokens[0].value, "CD");
        line += 2;
        while (*line == ' ' || *line == '\t') line++;
        
        if (*line != '\0') {
            if (*line == '"') {
                line++;
                char *dest = tokens[1].value;
                while (*line && *line != '"') {
                    *dest++ = *line++;
                }
                *dest = '\0';
            } else {
                strcpy(tokens[1].value, line);
            }
            tokens[1].type = TOKEN_CD;
            *token_count = 2;
        } else {
            *token_count = 1;
        }
    }
    // Check for PWD
    else if (strncmp(command, "PWD", 3) == 0) {
        tokens[0].type = TOKEN_PWD;
        strcpy(tokens[0].value, "PWD");
        *token_count = 1;
    }
    // Check for MKDIR
    else if (strncmp(command, "MKDIR", 5) == 0) {
        tokens[0].type = TOKEN_MKDIR;
        strcpy(tokens[0].value, "MKDIR");
        line += 5;
        while (*line == ' ' || *line == '\t') line++;
        
        if (*line != '\0') {
            if (*line == '"') {
                line++;
                char *dest = tokens[1].value;
                while (*line && *line != '"') {
                    *dest++ = *line++;
                }
                *dest = '\0';
            } else {
                strcpy(tokens[1].value, line);
            }
            tokens[1].type = TOKEN_MKDIR;
            *token_count = 2;
        } else {
            *token_count = 1;
        }
    }
    // Check for DRIVES
    else if (strncmp(command, "DRIVES", 6) == 0) {
        tokens[0].type = TOKEN_DRIVES;
        strcpy(tokens[0].value, "DRIVES");
        *token_count = 1;
    }
    // Check for CLS (clear screen)
    else if (strncmp(command, "CLS", 3) == 0) {
        tokens[0].type = TOKEN_CLS;
        strcpy(tokens[0].value, "CLS");
        *token_count = 1;
    }
    // Check for GOSUB
    else if (strncmp(command, "GOSUB", 5) == 0) {
        tokens[0].type = TOKEN_GOSUB;
        strcpy(tokens[0].value, "GOSUB");
        *token_count = 1;
        
        // Get target line number
        line += 5;  // Skip "GOSUB"
        while (*line == ' ' || *line == '\t') line++;
        
        char *dest = tokens[1].value;
        while (*line && *line != '\0') {
            *dest++ = *line++;
        }
        *dest = '\0';
        
        if (strlen(tokens[1].value) > 0) {
            tokens[1].type = TOKEN_GOSUB;
            *token_count = 2;
        }
    }
    // Check for ON...GOTO / ON...GOSUB
    else if (strncmp(command, "ON", 2) == 0) {
        tokens[0].type = TOKEN_ON;
        strcpy(tokens[0].value, "ON");
        line += 2;  // Skip "ON"
        while (*line == ' ' || *line == '\t') line++;
        
        // Get the expression (e.g., "n" or "x+1")
        char *dest = tokens[1].value;
        while (*line && *line != '\0' && !isspace(*line)) {
            *dest++ = *line++;
        }
        *dest = '\0';
        tokens[1].type = TOKEN_ON;
        
        // Skip whitespace
        while (*line == ' ' || *line == '\t') line++;
        
        // Check for GOTO or GOSUB
        char cmd_upper[256];
        strcpy(cmd_upper, line);
        to_upper(cmd_upper);
        
        if (strncmp(cmd_upper, "GOTO", 4) == 0) {
            line += 4;  // Skip "GOTO"
            tokens[2].type = TOKEN_GOTO;
            strcpy(tokens[2].value, "GOTO");
        } else if (strncmp(cmd_upper, "GOSUB", 5) == 0) {
            line += 5;  // Skip "GOSUB"
            tokens[2].type = TOKEN_GOSUB;
            strcpy(tokens[2].value, "GOSUB");
        } else {
            tokens[0].type = TOKEN_UNKNOWN;
            strcpy(tokens[0].value, "ON requires GOTO or GOSUB");
            *token_count = 1;
            return tokens;
        }
        
        // Skip whitespace
        while (*line == ' ' || *line == '\t') line++;
        
        // Get the line numbers (comma-separated)
        dest = tokens[3].value;
        while (*line && *line != '\0') {
            *dest++ = *line++;
        }
        *dest = '\0';
        tokens[3].type = TOKEN_ON;
        
        *token_count = 4;
    }
    // Check for GOTO
    else if (strncmp(command, "GOTO", 4) == 0) {
        tokens[0].type = TOKEN_GOTO;
        strcpy(tokens[0].value, "GOTO");
        *token_count = 1;
        
        // Get target line number
        line += 4;  // Skip "GOTO"
        while (*line == ' ' || *line == '\t') line++;
        
        char *dest = tokens[1].value;
        while (*line && *line != '\0') {
            *dest++ = *line++;
        }
        *dest = '\0';
        
        if (strlen(tokens[1].value) > 0) {
            tokens[1].type = TOKEN_GOTO;
            *token_count = 2;
        }
    }
    // Check for RETURN
    else if (strncmp(command, "RETURN", 6) == 0) {
        tokens[0].type = TOKEN_RETURN;
        strcpy(tokens[0].value, "RETURN");
        *token_count = 1;
    }
    // Check for AUTO
    else if (strncmp(command, "AUTO", 4) == 0) {
        tokens[0].type = TOKEN_AUTO;
        strcpy(tokens[0].value, "AUTO");
        line += 4;  // Skip "AUTO"
        while (*line == ' ' || *line == '\t') line++;
        
        // Get optional starting line number (default 10)
        if (*line != '\0' && isdigit(*line)) {
            char *dest = tokens[1].value;
            while (*line && isdigit(*line)) {
                *dest++ = *line++;
            }
            *dest = '\0';
            tokens[1].type = TOKEN_AUTO;
            *token_count = 2;
        } else {
            strcpy(tokens[1].value, "10");
            tokens[1].type = TOKEN_AUTO;
            *token_count = 2;
        }
    }
    // Check for RENUM
    else if (strncmp(command, "RENUM", 5) == 0) {
        tokens[0].type = TOKEN_RENUM;
        strcpy(tokens[0].value, "RENUM");
        line += 5;  // Skip "RENUM"
        while (*line == ' ' || *line == '\t') line++;
        
        // Get optional step parameter (default 10)
        if (*line != '\0' && isdigit(*line)) {
            char *dest = tokens[1].value;
            while (*line && isdigit(*line)) {
                *dest++ = *line++;
            }
            *dest = '\0';
            tokens[1].type = TOKEN_RENUM;
            *token_count = 2;
        } else {
            strcpy(tokens[1].value, "10");
            tokens[1].type = TOKEN_RENUM;
            *token_count = 2;
        }
    }
    // Check for EDIT
    else if (strncmp(command, "EDIT", 4) == 0) {
        tokens[0].type = TOKEN_EDIT;
        strcpy(tokens[0].value, "EDIT");
        line += 4;  // Skip "EDIT"
        while (*line == ' ' || *line == '\t') line++;
        
        // Get the line number to edit
        if (*line != '\0' && isdigit(*line)) {
            char *dest = tokens[1].value;
            while (*line && isdigit(*line)) {
                *dest++ = *line++;
            }
            *dest = '\0';
            tokens[1].type = TOKEN_EDIT;
            *token_count = 2;
        } else {
            tokens[0].type = TOKEN_UNKNOWN;
            strcpy(tokens[0].value, "EDIT requires a line number");
            *token_count = 1;
        }
    }
    // Check for SNIPPET
    else if (strncmp(command, "SNIPPET", 7) == 0) {
        tokens[0].type = TOKEN_SNIPPET;
        strcpy(tokens[0].value, "SNIPPET");
        line += 7;  // Skip "SNIPPET"
        while (*line == ' ' || *line == '\t') line++;
        
        // Get the topic name (required)
        if (*line != '\0') {
            char *dest = tokens[1].value;
            while (*line && *line != ' ' && *line != '\t') {
                *dest++ = *line++;
            }
            *dest = '\0';
            tokens[1].type = TOKEN_SNIPPET;
            *token_count = 2;
        } else {
            tokens[0].type = TOKEN_UNKNOWN;
            strcpy(tokens[0].value, "SNIPPET requires a command name");
            *token_count = 1;
        }
    }
    // Check for RANDOMIZE
    else if (strncmp(command, "RANDOMIZE", 9) == 0) {
        tokens[0].type = TOKEN_RANDOMIZE;
        strcpy(tokens[0].value, "RANDOMIZE");
        line += 9;  // Skip "RANDOMIZE"
        while (*line == ' ' || *line == '\t') line++;
        
        // Get optional seed value
        if (*line != '\0' && isdigit(*line)) {
            char *dest = tokens[1].value;
            while (*line && isdigit(*line)) {
                *dest++ = *line++;
            }
            *dest = '\0';
            tokens[1].type = TOKEN_RANDOMIZE;
            *token_count = 2;
        } else {
            *token_count = 1;
        }
    }
    // Check for BEEP
    else if (strncmp(command, "BEEP", 4) == 0) {
        tokens[0].type = TOKEN_BEEP;
        strcpy(tokens[0].value, "BEEP");
        *token_count = 1;
    }
    // Check for CLEAR
    else if (strncmp(command, "CLEAR", 5) == 0) {
        tokens[0].type = TOKEN_CLEAR;
        strcpy(tokens[0].value, "CLEAR");
        *token_count = 1;
    }
    // Check for DIM
    else if (strncmp(command, "DIM", 3) == 0) {
        tokens[0].type = TOKEN_DIM;
        strcpy(tokens[0].value, "DIM");
        line += 3;  // Skip "DIM"
        while (*line == ' ' || *line == '\t') line++;
        
        // Get the array declaration
        if (*line != '\0') {
            strcpy(tokens[1].value, line);
            tokens[1].type = TOKEN_DIM;
            *token_count = 2;
        } else {
            tokens[0].type = TOKEN_UNKNOWN;
            strcpy(tokens[0].value, "DIM requires array name");
            *token_count = 1;
        }
    }
    // Check for DATA
    else if (strncmp(command, "DATA", 4) == 0) {
        tokens[0].type = TOKEN_DATA;
        strcpy(tokens[0].value, "DATA");
        line += 4;  // Skip "DATA"
        while (*line == ' ' || *line == '\t') line++;
        
        // Get the data values
        if (*line != '\0') {
            strcpy(tokens[1].value, line);
            tokens[1].type = TOKEN_DATA;
            *token_count = 2;
        } else {
            *token_count = 1;
        }
    }
    // Check for READ
    else if (strncmp(command, "READ", 4) == 0) {
        tokens[0].type = TOKEN_READ;
        strcpy(tokens[0].value, "READ");
        line += 4;  // Skip "READ"
        while (*line == ' ' || *line == '\t') line++;
        
        // Get the variable names
        if (*line != '\0') {
            strcpy(tokens[1].value, line);
            tokens[1].type = TOKEN_READ;
            *token_count = 2;
        } else {
            *token_count = 1;
        }
    }
    // Check for RESTORE
    else if (strncmp(command, "RESTORE", 7) == 0) {
        tokens[0].type = TOKEN_RESTORE;
        strcpy(tokens[0].value, "RESTORE");
        *token_count = 1;
    }
    // Check for OPEN
    else if (strncmp(command, "OPEN", 4) == 0) {
        tokens[0].type = TOKEN_OPEN;
        strcpy(tokens[0].value, "OPEN");
        line += 4;  // Skip "OPEN"
        while (*line == ' ' || *line == '\t') line++;
        
        // Get the OPEN parameters
        if (*line != '\0') {
            strcpy(tokens[1].value, line);
            tokens[1].type = TOKEN_OPEN;
            *token_count = 2;
        } else {
            *token_count = 1;
        }
    }
    // Check for CLOSE
    else if (strncmp(command, "CLOSE", 5) == 0) {
        tokens[0].type = TOKEN_CLOSE;
        strcpy(tokens[0].value, "CLOSE");
        line += 5;  // Skip "CLOSE"
        while (*line == ' ' || *line == '\t') line++;
        
        // Get the file number
        if (*line != '\0') {
            strcpy(tokens[1].value, line);
            tokens[1].type = TOKEN_CLOSE;
            *token_count = 2;
        } else {
            *token_count = 1;
        }
    }
    // Check for PRINT#
    else if (strncmp(command, "PRINT#", 6) == 0) {
        tokens[0].type = TOKEN_PRINT_HASH;
        strcpy(tokens[0].value, "PRINT#");
        line += 6;  // Skip "PRINT#"
        while (*line == ' ' || *line == '\t') line++;
        
        // Get file number and data
        if (*line != '\0') {
            strcpy(tokens[1].value, line);
            tokens[1].type = TOKEN_PRINT_HASH;
            *token_count = 2;
        } else {
            *token_count = 1;
        }
    }
    // Check for INPUT#
    else if (strncmp(command, "INPUT#", 6) == 0) {
        tokens[0].type = TOKEN_INPUT_HASH;
        strcpy(tokens[0].value, "INPUT#");
        line += 6;  // Skip "INPUT#"
        while (*line == ' ' || *line == '\t') line++;
        
        // Get file number and variable
        if (*line != '\0') {
            strcpy(tokens[1].value, line);
            tokens[1].type = TOKEN_INPUT_HASH;
            *token_count = 2;
        } else {
            *token_count = 1;
        }
    }
    // Check for HELP
    else if (strncmp(command, "HELP", 4) == 0) {
        tokens[0].type = TOKEN_HELP;
        strcpy(tokens[0].value, "HELP");
        line += 4;  // Skip "HELP"
        while (*line == ' ' || *line == '\t') line++;
        
        // Get optional topic name
        if (*line != '\0') {
            char *dest = tokens[1].value;
            while (*line && *line != ' ' && *line != '\t') {
                *dest++ = *line++;
            }
            *dest = '\0';
            tokens[1].type = TOKEN_HELP;
            *token_count = 2;
        } else {
            *token_count = 1;
        }
    }
    // Check for implicit LET (e.g., "x=10" without LET keyword)
    else if (strchr(line, '=')) {
        tokens[0].type = TOKEN_LET;
        strcpy(tokens[0].value, "LET");
        strcpy(tokens[1].value, line);
        tokens[1].type = TOKEN_LET;
        *token_count = 2;
    }
    else {
        tokens[0].type = TOKEN_UNKNOWN;
        strcpy(tokens[0].value, line);
        *token_count = 1;
    }
    
    return tokens;
}

void free_tokens(Token *tokens) {
    free(tokens);
}

