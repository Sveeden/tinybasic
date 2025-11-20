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
        
        // Parse print items separated by semicolons
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
                (*token_count)++;
            } else {
                // Unquoted expression (read until semicolon or end)
                char *dest = tokens[*token_count].value;
                while (*line && *line != ';') {
                    *dest++ = *line++;
                }
                *dest = '\0';
                
                // Trim trailing spaces
                dest--;
                while (dest >= tokens[*token_count].value && (*dest == ' ' || *dest == '\t')) {
                    *dest-- = '\0';
                }
                
                if (strlen(tokens[*token_count].value) > 0) {
                    tokens[*token_count].type = TOKEN_PRINT;
                    (*token_count)++;
                }
            }
            
            // Skip spaces after the item
            while (*line == ' ' || *line == '\t') {
                line++;
            }
            
            // Check for semicolon
            if (*line == ';') {
                line++;  // Skip semicolon
                // Mark that we have a semicolon (no newline after this item)
                tokens[*token_count - 1].type = TOKEN_PRINT;  // Still PRINT but we'll track semicolons differently
                
                while (*line == ' ' || *line == '\t') {
                    line++;
                }
            } else {
                // No semicolon, we're done
                break;
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
        char var_name[256];
        
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
            
            if (*line == ';') {
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
    // Check for RM
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
    // Check for RMDIR
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
