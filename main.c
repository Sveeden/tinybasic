#include "pico/stdlib.h"
#include "pico/stdio_usb.h"
#include <stdio.h>
#include <string.h>
#include "token.h"
#include "execute.h"
#include "variables.h"
#include "program.h"
#include "loops.h"
#include "filesystem.h"

#define LINE_MAX 256
#define VERSION "1.0.0"

int main() {
    stdio_init_all();
    var_init();
    prog_init();
    loop_init();
    
    // Wait for user to connect serial terminal
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
    
    while (true) {
        printf("> ");
        fflush(stdout);
        
        // Read a line
        size_t len = 0;
        while (true) {
            int c = getchar_timeout_us(0);
            if (c == PICO_ERROR_TIMEOUT) continue;
            
            // Check for Ctrl+C (0x03)
            if (c == 0x03) {
                printf("\n^C\n");
                execution_interrupted = 1;  // Set interrupt flag
                fflush(stdout);
                line[0] = '\0';
                len = 0;
                break;  // Exit input loop, go back to prompt
            }
            
            // Handle Enter key: 0x0D (CR) or 0x0A (LF)
            if (c == 0x0D || c == 0x0A || c == '\r' || c == '\n') {
                putchar('\n');
                line[len] = '\0';
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
                if (len < LINE_MAX - 1) {
                    line[len++] = (char)c;
                    putchar(c);
                    fflush(stdout);
                }
            }
        }
        
        // Process the line
        if (len > 0) {
            // Strip leading "> " if present (from paste operations)
            const char *cmd = line;
            if (line[0] == '>' && line[1] == ' ') {
                cmd = line + 2;  // Skip "> "
            }
            
            if (strlen(cmd) > 0) {
                int line_num;
                
                // Check if this is a numbered line
                if (prog_has_line_number(cmd, &line_num)) {
                    // Store it in the program
                    prog_store_line(cmd);
                } else {
                    // Execute immediately
                    int token_count;
                    Token* tokens = tokenize(cmd, &token_count);
                    execute(tokens, token_count, -1);  // -1 = immediate mode
                    free_tokens(tokens);
                }
            }
        }
    }
    
    return 0;
}