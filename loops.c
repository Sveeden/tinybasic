#include "loops.h"
#include "variables.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_LOOP_DEPTH 10

typedef enum {
    LOOP_FOR,
    LOOP_WHILE
} LoopType;

typedef struct {
    LoopType type;
    char var_name[50];  // FOR loop variable
    int end_val;        // FOR loop end value
    int start_line;     // Line number where loop starts
} LoopInfo;

static LoopInfo loop_stack[MAX_LOOP_DEPTH];
static int loop_depth = 0;

void loop_init(void) {
    loop_depth = 0;
    memset(loop_stack, 0, sizeof(loop_stack));
}

void loop_push_for(const char *var, int start_val, int end_val, int body_start_line) {
    if (loop_depth < MAX_LOOP_DEPTH) {
        loop_stack[loop_depth].type = LOOP_FOR;
        strcpy(loop_stack[loop_depth].var_name, var);
        loop_stack[loop_depth].end_val = end_val;
        loop_stack[loop_depth].start_line = body_start_line;
        
        // Set the loop variable to start value
        char assignment[100];
        snprintf(assignment, sizeof(assignment), "%s=%d", var, start_val);
        var_set(assignment);
        
        loop_depth++;
    }
}

void loop_push_while(int while_line) {
    if (loop_depth < MAX_LOOP_DEPTH) {
        loop_stack[loop_depth].type = LOOP_WHILE;
        loop_stack[loop_depth].start_line = while_line;
        loop_depth++;
    }
}

int loop_get_start_line(void) {
    if (loop_depth > 0) {
        return loop_stack[loop_depth - 1].start_line;
    }
    return -1;
}

int loop_for_should_continue(void) {
    if (loop_depth > 0 && loop_stack[loop_depth - 1].type == LOOP_FOR) {
        const char *var_name = loop_stack[loop_depth - 1].var_name;
        const char *val_str = var_get(var_name);
        if (val_str) {
            int current = atoi(val_str);
            return current <= loop_stack[loop_depth - 1].end_val;
        }
    }
    return 0;
}

// Fix: Only pop FOR loop when end is exceeded
int loop_for_next(void) {
    if (loop_depth > 0 && loop_stack[loop_depth - 1].type == LOOP_FOR) {
        const char *var_name = loop_stack[loop_depth - 1].var_name;
        const char *val_str = var_get(var_name);
        if (val_str) {
            int current = atoi(val_str);
            current++;
            // Update variable
            char assignment[100];
            snprintf(assignment, sizeof(assignment), "%s=%d", var_name, current);
            var_set(assignment);
            if (current <= loop_stack[loop_depth - 1].end_val) {
                return loop_stack[loop_depth - 1].start_line; // Continue loop
            } else {
                loop_pop(); // Only pop when done
            }
        }
    }
    return -1; // No jump, continue sequentially
}

void loop_pop(void) {
    if (loop_depth > 0) {
        loop_depth--;
    }
}

int loop_has_active(void) {
    return loop_depth > 0;
}

// Fix: WHILE/WEND stack pop only when condition fails
void loop_wend_check_pop(int condition) {
    if (loop_depth > 0 && loop_stack[loop_depth - 1].type == LOOP_WHILE) {
        if (!condition) {
            loop_pop();
        }
    }
}
