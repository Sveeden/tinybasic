#include "pico/stdlib.h"
#include "pico/stdio_usb.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "token.h"
#include "execute.h"
#include "variables.h"
#include "program.h"
#include "loops.h"
#include "filesystem.h"

#define LINE_MAX 256
#define VERSION "1.0.0"
#define HISTORY_SIZE 100

// Command history buffer
static char history[HISTORY_SIZE][LINE_MAX];
static int history_count = 0;
static int history_index = -1;  // -1 means current input, else index into history

// Variables for EDIT mode
static int edit_line_num = -1;  // -1 means not in edit mode
static char edit_line_text[LINE_MAX];  // The line being edited

// Helper function to add command to history
static void add_to_history(const char *cmd) {
    if (history_count < HISTORY_SIZE) {
        strcpy(history[history_count], cmd);
        history_count++;
    } else {
        // Shift history and add new command
        for (int i = 0; i < HISTORY_SIZE - 1; i++) {
            strcpy(history[i], history[i + 1]);
        }
        strcpy(history[HISTORY_SIZE - 1], cmd);
    }
}

// Set EDIT mode - called by execute.c when EDIT command is used
void set_edit_mode(int line_num, const char *line_text) {
    edit_line_num = line_num;
    strcpy(edit_line_text, line_text);
}

// Check if we're in EDIT mode
int is_edit_mode(void) {
    return (edit_line_num != -1);
}

// Get the line number being edited
int get_edit_line_num(void) {
    return edit_line_num;
}

// Get the line text being edited
const char* get_edit_line_text(void) {
    return edit_line_text;
}

// Clear EDIT mode
void clear_edit_mode(void) {
    edit_line_num = -1;
}

int main() {
    stdio_init_all();
    var_init();
    prog_init();
    loop_init();
    
    // Wait for user to connect serial terminal
    sleep_ms(200);  // Brief delay for USB to initialize
    while (!stdio_usb_connected()) {
        sleep_ms(100);
    }
    sleep_ms(500);  // Give terminal time to be ready

    // Print cool ASCII banner with version
    printf("\n"); fflush(stdout); sleep_ms(50);
    printf("***************************************************************\n"); fflush(stdout); sleep_ms(50);
    printf("*                                                             *\n"); fflush(stdout); sleep_ms(50);
    printf("*                 OBI-88 BASIC INTERPRETER                    *\n"); fflush(stdout); sleep_ms(50);
    printf("*                       Version 1.0                           *\n"); fflush(stdout); sleep_ms(50);
    printf("*                                                             *\n"); fflush(stdout); sleep_ms(50);
    printf("*            Ready to type. Ready to create.                  *\n"); fflush(stdout); sleep_ms(50);
    printf("*                                                             *\n"); fflush(stdout); sleep_ms(50);
    printf("***************************************************************\n"); fflush(stdout); sleep_ms(50);
    printf("\n"); fflush(stdout); sleep_ms(100);

    // Initialize filesystem
    printf("Initializing filesystem...\n"); fflush(stdout);
    if (fs_init() == 0) {
        // Always start at root directory
        fs_cd("/");
        printf("Drive 0: ready at %s\n", fs_get_path()); fflush(stdout);
    } else {
        printf("Warning: Filesystem init failed\n"); fflush(stdout);
    }
    
    // Show memory info
    int drive_used_kb = prog_get_memory_used() / 1024;
    int drive_free_kb = 1015 - drive_used_kb;
    if (drive_free_kb < 0) drive_free_kb = 0;
    printf("Drive Memory: %d/%d KB\n", drive_used_kb, 1015); fflush(stdout);
    printf("RAM: 0/520 KB\n"); fflush(stdout);
    printf("Ready\n"); fflush(stdout);
    printf("\n"); fflush(stdout); sleep_ms(100);
    
    char line[LINE_MAX];
    int auto_mode = 0;  // Flag to track if we're in AUTO mode
    
    while (true) {
        // Check if we're in EDIT mode
        if (is_edit_mode()) {
            int edit_num = get_edit_line_num();
            const char *edit_text = get_edit_line_text();
            
            // Display the line being edited
            printf("%d %s", edit_num, edit_text);
            fflush(stdout);
            
            // Pre-load the line content into the input buffer
            strcpy(line, edit_text);
            size_t len = strlen(line);
            int cursor_pos = len;  // Cursor at end of line
            
            // Read modifications
            char current_input[LINE_MAX];
            current_input[0] = '\0';
            int history_idx = -1;
            
            while (true) {
                int c = getchar_timeout_us(0);
                if (c == PICO_ERROR_TIMEOUT) continue;
                
                // Check for Ctrl+C
                if (c == 0x03) {
                    printf("\n^C\n");
                    clear_edit_mode();
                    fflush(stdout);
                    line[0] = '\0';
                    len = 0;
                    break;
                }
                
                // Handle Enter key
                if (c == 0x0D || c == 0x0A || c == '\r' || c == '\n') {
                    putchar('\n');
                    line[len] = '\0';
                    clear_edit_mode();
                    break;
                }
                
                // Handle arrow keys (ESC sequences)
                if (c == 0x1B) {  // ESC character
                    int next1 = getchar_timeout_us(0);
                    if (next1 == PICO_ERROR_TIMEOUT) continue;
                    
                    if (next1 == '[') {  // CSI sequence
                        int next2 = getchar_timeout_us(0);
                        if (next2 == PICO_ERROR_TIMEOUT) continue;
                        
                        if (next2 == 'C') {  // Right arrow - move cursor right
                            if (cursor_pos < len) {
                                cursor_pos++;
                                putchar(line[cursor_pos - 1]);
                                fflush(stdout);
                            }
                            continue;
                        }
                        else if (next2 == 'D') {  // Left arrow - move cursor left
                            if (cursor_pos > 0) {
                                putchar('\b');
                                cursor_pos--;
                                fflush(stdout);
                            }
                            continue;
                        }
                    }
                    continue;
                }
                
                if (c == 0x08 || c == 0x7F) {  // Backspace
                    if (len > 0 && cursor_pos > 0) {
                        for (int i = cursor_pos - 1; i < len - 1; i++) {
                            line[i] = line[i + 1];
                        }
                        len--;
                        cursor_pos--;
                        
                        putchar('\b');
                        for (int i = cursor_pos; i < len; i++) {
                            putchar(line[i]);
                        }
                        putchar(' ');
                        for (int i = cursor_pos; i <= len; i++) {
                            putchar('\b');
                        }
                        fflush(stdout);
                    }
                    continue;
                }
                
                if (c >= 32 && c <= 126) {  // Printable ASCII
                    if (len < LINE_MAX - 1) {
                        for (int i = len; i > cursor_pos; i--) {
                            line[i] = line[i - 1];
                        }
                        line[cursor_pos] = (char)c;
                        len++;
                        
                        for (int i = cursor_pos; i < len; i++) {
                            putchar(line[i]);
                        }
                        cursor_pos++;
                        
                        for (int i = cursor_pos; i < len; i++) {
                            putchar('\b');
                        }
                        fflush(stdout);
                    }
                }
            }
            
            // Process the edited line
            if (len > 0) {
                // Reconstruct the full line with line number
                char full_line[LINE_MAX];
                snprintf(full_line, LINE_MAX, "%d %s", edit_num, line);
                prog_store_line(full_line);
                printf("Line updated\n");
            }
            continue;  // Back to main prompt
        }
        
        // Show prompt based on mode
        if (auto_mode) {
            printf("%d ", prog_get_auto_line_number());
        } else {
            printf("> ");
        }
        fflush(stdout);
        
        // Read a line with history and arrow key support
        size_t len = 0;
        int cursor_pos = 0;  // Position of cursor in line
        char current_input[LINE_MAX];
        current_input[0] = '\0';
        
        while (true) {
            int c = getchar_timeout_us(0);
            if (c == PICO_ERROR_TIMEOUT) continue;
            
            // Check for Ctrl+C (0x03)
            if (c == 0x03) {
                printf("\n^C\n");
                if (auto_mode) {
                    auto_mode = 0;
                    printf("Ready\n");
                }
                execution_interrupted = 1;
                fflush(stdout);
                line[0] = '\0';
                len = 0;
                break;
            }
            
            // Handle Enter key: 0x0D (CR) or 0x0A (LF)
            if (c == 0x0D || c == 0x0A || c == '\r' || c == '\n') {
                putchar('\n');
                line[len] = '\0';
                break;
            }
            
            // Handle arrow keys (ESC sequences)
            if (c == 0x1B) {  // ESC character
                int next1 = getchar_timeout_us(0);
                if (next1 == PICO_ERROR_TIMEOUT) continue;
                
                if (next1 == '[') {  // CSI sequence
                    int next2 = getchar_timeout_us(0);
                    if (next2 == PICO_ERROR_TIMEOUT) continue;
                    
                    if (next2 == 'A') {  // Up arrow - previous in history
                        if (history_count > 0) {
                            if (history_index == -1) {
                                // Save current input
                                strcpy(current_input, line);
                            }
                            history_index++;
                            if (history_index >= history_count) {
                                history_index = history_count - 1;
                            }
                            
                            // Clear current line
                            for (int i = 0; i < len; i++) {
                                putchar('\b');
                                putchar(' ');
                                putchar('\b');
                            }
                            
                            // Display history entry
                            strcpy(line, history[history_count - 1 - history_index]);
                            len = strlen(line);
                            cursor_pos = len;
                            printf("%s", line);
                            fflush(stdout);
                        }
                        continue;
                    }
                    else if (next2 == 'B') {  // Down arrow - next in history
                        if (history_index > 0) {
                            history_index--;
                            
                            // Clear current line
                            for (int i = 0; i < len; i++) {
                                putchar('\b');
                                putchar(' ');
                                putchar('\b');
                            }
                            
                            // Display history entry
                            strcpy(line, history[history_count - 1 - history_index]);
                            len = strlen(line);
                            cursor_pos = len;
                            printf("%s", line);
                            fflush(stdout);
                        }
                        else if (history_index == 0) {
                            history_index = -1;
                            
                            // Clear current line
                            for (int i = 0; i < len; i++) {
                                putchar('\b');
                                putchar(' ');
                                putchar('\b');
                            }
                            
                            // Restore saved input
                            strcpy(line, current_input);
                            len = strlen(line);
                            cursor_pos = len;
                            printf("%s", line);
                            fflush(stdout);
                        }
                        continue;
                    }
                    else if (next2 == 'C') {  // Right arrow - move cursor right
                        if (cursor_pos < len) {
                            cursor_pos++;
                            putchar(line[cursor_pos - 1]);
                            fflush(stdout);
                        }
                        continue;
                    }
                    else if (next2 == 'D') {  // Left arrow - move cursor left
                        if (cursor_pos > 0) {
                            putchar('\b');
                            cursor_pos--;
                            fflush(stdout);
                        }
                        continue;
                    }
                }
                continue;
            }
            
            if (c == 0x08 || c == 0x7F) {  // Backspace
                if (len > 0 && cursor_pos > 0) {
                    // Remove character at cursor position
                    for (int i = cursor_pos - 1; i < len - 1; i++) {
                        line[i] = line[i + 1];
                    }
                    len--;
                    cursor_pos--;
                    
                    // Redraw from cursor to end
                    putchar('\b');
                    for (int i = cursor_pos; i < len; i++) {
                        putchar(line[i]);
                    }
                    putchar(' ');
                    for (int i = cursor_pos; i <= len; i++) {
                        putchar('\b');
                    }
                    fflush(stdout);
                }
                continue;
            }
            
            if (c >= 32 && c <= 126) {  // Printable ASCII
                if (len < LINE_MAX - 1) {
                    // Insert character at cursor position
                    for (int i = len; i > cursor_pos; i--) {
                        line[i] = line[i - 1];
                    }
                    line[cursor_pos] = (char)c;
                    len++;
                    
                    // Redraw from cursor to end
                    for (int i = cursor_pos; i < len; i++) {
                        putchar(line[i]);
                    }
                    cursor_pos++;
                    
                    // Move cursor back to position
                    for (int i = cursor_pos; i < len; i++) {
                        putchar('\b');
                    }
                    fflush(stdout);
                }
            }
        }
        
        // Add non-empty commands to history
        if (len > 0 && (line[0] < '0' || line[0] > '9')) {  // Don't add numbered lines to history
            add_to_history(line);
        }
        history_index = -1;  // Reset history index
        
        // Process the line
        if (len > 0) {
            // Strip leading "> " if present (from paste operations)
            const char *cmd = line;
            if (line[0] == '>' && line[1] == ' ') {
                cmd = line + 2;  // Skip "> "
            }
            
            if (strlen(cmd) > 0) {
                int line_num;
                
                if (auto_mode) {
                    // In AUTO mode: auto-number the line
                    // If user enters a line number, exit AUTO mode
                    if (prog_has_line_number(cmd, &line_num)) {
                        // User entered a numbered line - exit AUTO mode
                        auto_mode = 0;
                        printf("Ready\n");
                        prog_store_line(cmd);
                    } else {
                        // Auto-number the line
                        int current_line = prog_get_auto_line_number();
                        char auto_numbered[LINE_MAX];
                        snprintf(auto_numbered, LINE_MAX, "%d %s", current_line, cmd);
                        prog_store_line(auto_numbered);
                        prog_increment_auto_line_number(10);  // Increment by 10
                    }
                } else {
                    // Normal mode: check if numbered or command
                    if (prog_has_line_number(cmd, &line_num)) {
                        // Store it in the program
                        prog_store_line(cmd);
                    } else {
                        // Execute immediately, supporting ':' separated statements
                        const char *trimmed = cmd;
                        while (*trimmed == ' ' || *trimmed == '\t') {
                            trimmed++;
                        }
                        size_t trimmed_len = strlen(trimmed);
                        bool is_auto = false;
                        if (trimmed_len >= 4) {
                            bool matches_auto = (toupper((unsigned char)trimmed[0]) == 'A' &&
                                                 toupper((unsigned char)trimmed[1]) == 'U' &&
                                                 toupper((unsigned char)trimmed[2]) == 'T' &&
                                                 toupper((unsigned char)trimmed[3]) == 'O');
                            if (matches_auto) {
                                char next_char = trimmed_len > 4 ? trimmed[4] : '\0';
                                if (next_char == '\0' || next_char == ' ' ||
                                    next_char == '\t' || next_char == ':') {
                                    is_auto = true;
                                }
                            }
                        }
                        if (is_auto) {
                            auto_mode = 1;
                        }
                        execute_line_direct(cmd);
                    }
                }
            }
        }
    }
    
    return 0;
}