#include "program.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_LINES 200
#define MAX_LINE_LENGTH 256

typedef struct {
    int line_num;
    char text[MAX_LINE_LENGTH];
} ProgramLine;

static ProgramLine program[MAX_LINES];
static int line_count = 0;
static int auto_line_num = 10;  // Current AUTO line number (default 10)

void prog_init(void) {
    line_count = 0;
    memset(program, 0, sizeof(program));
}

void prog_clear(void) {
    prog_init();
}

int prog_has_line_number(const char *line, int *line_num) {
    // Skip leading spaces
    while (*line == ' ' || *line == '\t') {
        line++;
    }
    
    // Check if line starts with a digit
    if (!isdigit(*line)) {
        return 0;
    }
    
    // Extract the line number
    *line_num = atoi(line);
    return 1;
}

void prog_store_line(const char *line) {
    int line_num;
    
    // Extract line number
    if (!prog_has_line_number(line, &line_num)) {
        return;
    }
    
    // Skip the line number to get the command
    const char *cmd = line;
    while (isdigit(*cmd)) {
        cmd++;
    }
    while (*cmd == ' ' || *cmd == '\t') {
        cmd++;
    }
    
    // Find if this line number already exists
    int idx = -1;
    for (int i = 0; i < line_count; i++) {
        if (program[i].line_num == line_num) {
            idx = i;
            break;
        }
    }
    
    // If command is empty, delete the line
    if (*cmd == '\0') {
        if (idx >= 0) {
            // Delete by shifting lines down
            for (int i = idx; i < line_count - 1; i++) {
                program[i] = program[i + 1];
            }
            line_count--;
        }
        return;
    }
    
    // If line exists, replace it
    if (idx >= 0) {
        strcpy(program[idx].text, cmd);
        return;
    }
    
    // Add new line if there's space
    if (line_count < MAX_LINES) {
        program[line_count].line_num = line_num;
        strcpy(program[line_count].text, cmd);
        line_count++;
        
        // Sort by line number (simple bubble sort)
        for (int i = 0; i < line_count - 1; i++) {
            for (int j = 0; j < line_count - i - 1; j++) {
                if (program[j].line_num > program[j + 1].line_num) {
                    ProgramLine temp = program[j];
                    program[j] = program[j + 1];
                    program[j + 1] = temp;
                }
            }
        }
    }
}

const char* prog_get_line(int line_num) {
    for (int i = 0; i < line_count; i++) {
        if (program[i].line_num == line_num) {
            return program[i].text;
        }
    }
    return NULL;
}

int prog_first_line(void) {
    if (line_count > 0) {
        return program[0].line_num;
    }
    return -1;
}

int prog_next_line(int current_line) {
    for (int i = 0; i < line_count; i++) {
        if (program[i].line_num == current_line) {
            if (i + 1 < line_count) {
                return program[i + 1].line_num;
            }
            break;
        }
    }
    return -1;
}

void prog_list(void) {
    for (int i = 0; i < line_count; i++) {
        printf("%d %s\n", program[i].line_num, program[i].text);
    }
}

int prog_get_memory_used(void) {
    int used = 0;
    for (int i = 0; i < line_count; i++) {
        // Estimate: line number (4 bytes) + text content (strlen) + overhead (8 bytes)
        used += 4 + strlen(program[i].text) + 8;
    }
    return used;
}

int prog_get_auto_line_number(void) {
    return auto_line_num;
}

void prog_set_auto_line_number(int line_num) {
    auto_line_num = line_num;
}

void prog_increment_auto_line_number(int step) {
    auto_line_num += step;
}

void prog_renumber(int step) {
    if (line_count == 0) {
        return;
    }
    
    // Create mapping of old line numbers to new line numbers
    int old_lines[MAX_LINES];
    int new_lines[MAX_LINES];
    int new_line_num = step;
    
    // Build the mapping
    for (int i = 0; i < line_count; i++) {
        old_lines[i] = program[i].line_num;
        new_lines[i] = new_line_num;
        new_line_num += step;
    }
    
    // Update GOTO and GOSUB references in all lines
    for (int i = 0; i < line_count; i++) {
        char new_text[MAX_LINE_LENGTH];
        strcpy(new_text, program[i].text);
        
        // Find and replace GOTO and GOSUB line numbers
        char *cmd = new_text;
        
        // Look for GOTO or GOSUB
        while (*cmd) {
            // Skip whitespace
            while (*cmd == ' ' || *cmd == '\t') cmd++;
            
            // Check for GOTO
            if (strncmp(cmd, "GOTO", 4) == 0) {
                cmd += 4;
                while (*cmd == ' ' || *cmd == '\t') cmd++;
                
                // Parse the line number
                if (isdigit(*cmd)) {
                    int old_target = atoi(cmd);
                    
                    // Find the mapping for this line number
                    int new_target = -1;
                    for (int j = 0; j < line_count; j++) {
                        if (old_lines[j] == old_target) {
                            new_target = new_lines[j];
                            break;
                        }
                    }
                    
                    if (new_target != -1) {
                        // Replace the line number
                        char before[256];
                        char after[256];
                        int cmd_start = cmd - new_text;
                        
                        // Copy everything before the line number
                        strncpy(before, new_text, cmd_start);
                        before[cmd_start] = '\0';
                        
                        // Skip old line number
                        while (*cmd && isdigit(*cmd)) cmd++;
                        
                        // Copy everything after
                        strcpy(after, cmd);
                        
                        // Rebuild the line
                        snprintf(new_text, MAX_LINE_LENGTH, "%s%d%s", before, new_target, after);
                        cmd = new_text + cmd_start;
                    }
                }
            }
            // Check for GOSUB
            else if (strncmp(cmd, "GOSUB", 5) == 0) {
                cmd += 5;
                while (*cmd == ' ' || *cmd == '\t') cmd++;
                
                // Parse the line number
                if (isdigit(*cmd)) {
                    int old_target = atoi(cmd);
                    
                    // Find the mapping for this line number
                    int new_target = -1;
                    for (int j = 0; j < line_count; j++) {
                        if (old_lines[j] == old_target) {
                            new_target = new_lines[j];
                            break;
                        }
                    }
                    
                    if (new_target != -1) {
                        // Replace the line number
                        char before[256];
                        char after[256];
                        int cmd_start = cmd - new_text;
                        
                        // Copy everything before the line number
                        strncpy(before, new_text, cmd_start);
                        before[cmd_start] = '\0';
                        
                        // Skip old line number
                        while (*cmd && isdigit(*cmd)) cmd++;
                        
                        // Copy everything after
                        strcpy(after, cmd);
                        
                        // Rebuild the line
                        snprintf(new_text, MAX_LINE_LENGTH, "%s%d%s", before, new_target, after);
                        cmd = new_text + cmd_start;
                    }
                }
            }
            else {
                // Move to next token
                while (*cmd && *cmd != ' ' && *cmd != '\t') cmd++;
            }
        }
        
        // Update the line with modified text
        strcpy(program[i].text, new_text);
        program[i].line_num = new_lines[i];
    }
}

