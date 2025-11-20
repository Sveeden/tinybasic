#ifndef LOOPS_H
#define LOOPS_H

// Initialize loop stack
void loop_init(void);

// Push a FOR loop onto the stack
void loop_push_for(const char *var, int start_val, int end_val, int body_start_line);

// Push a WHILE loop onto the stack
void loop_push_while(int while_line);

// Get the current loop's start line
int loop_get_start_line(void);

// Check if current FOR loop should continue
int loop_for_should_continue(void);

// Increment the FOR loop variable and check if done
int loop_for_next(void);

// Pop the top loop off the stack
void loop_pop(void);

// Check if there are any active loops
int loop_has_active(void);

#endif
