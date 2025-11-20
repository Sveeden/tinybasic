#ifndef VARIABLES_H
#define VARIABLES_H

// Initialize variable storage
void var_init(void);

// Set a variable: x=10 or x$="hello"
void var_set(const char *assignment);

// Get a variable value: returns pointer to value string
const char* var_get(const char *name);

// Check if variable is a string (ends with $)
int var_is_string(const char *name);

// Check if variable is a number
int var_is_number(const char *name);

#endif
