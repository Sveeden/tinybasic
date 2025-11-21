#ifndef PROGRAM_H
#define PROGRAM_H

// Initialize program storage
void prog_init(void);

// Store a numbered line (e.g., "10 PRINT hello")
void prog_store_line(const char *line);

// Get a line by line number
const char* prog_get_line(int line_num);

// Get the next line number after the given one (for sequential execution)
int prog_next_line(int current_line);

// Get the first line number
int prog_first_line(void);

// Clear all stored lines
void prog_clear(void);

// List all stored lines
void prog_list(void);

// Check if a line starts with a number
int prog_has_line_number(const char *line, int *line_num);

// Get the last line number in the program
int prog_get_last_line_number(void);

// Calculate memory used by program in bytes
int prog_get_memory_used(void);

// Get the current AUTO line number
int prog_get_auto_line_number(void);

// Set the AUTO line number
void prog_set_auto_line_number(int line_num);

// Increment AUTO line number by the given step
void prog_increment_auto_line_number(int step);

// Renumber all lines in program by the given step (starting at step)
void prog_renumber(int step);

#endif
