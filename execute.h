#ifndef EXECUTE_H
#define EXECUTE_H

#include "token.h"

// Global interrupt flag set by Ctrl-C
extern volatile int execution_interrupted;

// Execute a parsed token command
// line_num: the current line number (or -1 if immediate mode)
// Returns: the next line number to execute (or -1 to continue sequentially)
int execute(Token* tokens, int token_count, int line_num);

// Check if execution should be interrupted and clear the flag
int should_stop_execution(void);

// Execute one or more direct-mode statements (handles ':' separators)
void execute_line_direct(const char *line_text);

#endif
