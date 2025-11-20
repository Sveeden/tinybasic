#ifndef EXECUTE_H
#define EXECUTE_H

#include "token.h"

// Execute a parsed token command
// line_num: the current line number (or -1 if immediate mode)
// Returns: the next line number to execute (or -1 to continue sequentially)
int execute(Token* tokens, int token_count, int line_num);

#endif
