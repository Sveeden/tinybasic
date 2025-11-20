#ifndef HELP_H
#define HELP_H

// Structure to hold help information
typedef struct {
    const char *topic;
    const char *category;
    const char *line1;
    const char *line2;
    const char *line3;
} HelpEntry;

// Get help entry by topic (case-insensitive)
// Returns NULL if topic not found
const HelpEntry* help_get(const char *topic);

// Print a help entry to output
void help_print(const HelpEntry *entry);

// Print category listing (for HELP with no arguments)
void help_list_categories(void);

// Append help code lines to current program
void help_append_to_program(const HelpEntry *entry);

#endif
