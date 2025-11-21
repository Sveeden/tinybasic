#include "help.h"
#include "program.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// All help entries organized by category
static const HelpEntry help_entries[] = {
    // PROGRAM CONTROL
    {"RUN", "Program Control", "RUN", "", ""},
    {"LOAD", "Program Control", "LOAD \"program.bas\"", "", ""},
    {"SAVE", "Program Control", "SAVE \"program.bas\"", "", ""},
    {"LIST", "Program Control", "LIST", "10 PRINT \"LINE 10\"", "20 PRINT \"LINE 20\""},
    {"NEW", "Program Control", "NEW", "Program and variables cleared", ""},
    {"DELETE", "Program Control", "DELETE 10-50", "Deletes lines 10 through 50", ""},
    {"AUTO", "Program Control", "AUTO", "AUTO 100", "Enters auto line numbering mode"},
    {"CLEAR", "Program Control", "CLEAR", "LET x=100", "CLEAR"},
    {"DIR", "Program Control", "DIR", "DIR", ""},
    {"NOTE", "Program Control", "10 NOTE \"Comment in code\"", "20 PRINT \"Next line\"", ""},
    {"END", "Program Control", "FOR i=1 TO 100", "PRINT i", "NEXT"},
    
    // INPUT/OUTPUT
    {"PRINT", "Input/Output", "PRINT \"Hello\"", "PRINT 42", "PRINT \"Value: \" x"},
    {"INPUT", "Input/Output", "INPUT \"Enter name: \"; name$", "PRINT name$", ""},
    {"LET", "Input/Output", "LET x=100", "LET y=x+50", "PRINT y"},
    {"CLS", "Input/Output", "CLS", "Screen cleared", ""},
    {"BEEP", "Input/Output", "BEEP", "Sounds speaker", ""},
    
    // LOOPS
    {"FOR", "Loops", "FOR i=1 TO 10", "PRINT i", "NEXT"},
    {"NEXT", "Loops", "FOR i=1 TO 5", "PRINT i*2", "NEXT"},
    {"WHILE", "Loops", "WHILE x<100", "LET x=x+1", "WEND"},
    {"WEND", "Loops", "WHILE x>0", "PRINT x", "WEND"},
    
    // CONDITIONS
    {"IF", "Conditions", "IF x>10 THEN PRINT \"Big\"", "IF x<5 THEN GOTO 100", "IF y=0 THEN END"},
    {"THEN", "Conditions", "IF x>0 THEN PRINT \"Positive\"", "", ""},
    {"ELSE", "Conditions", "IF x>0 THEN PRINT \"Pos\" ELSE PRINT \"Neg\"", "", ""},
    {"GOTO", "Conditions", "100 PRINT \"START\"", "GOTO 200", "200 END"},
    {"GOSUB", "Conditions", "GOSUB 1000", "PRINT result", "END"},
    {"RETURN", "Conditions", "1000 LET result=100", "RETURN", ""},
    
    // MATH FUNCTIONS - SINGLE ARGUMENT
    {"ABS", "Math Functions", "LET x=-42", "LET y=ABS(x)", "PRINT y"},
    {"SGN", "Math Functions", "LET s=SGN(-5)", "PRINT s", ""},
    {"INT", "Math Functions", "LET i=INT(3.7)", "PRINT i", ""},
    {"SQR", "Math Functions", "LET s=SQR(25)", "PRINT s", ""},
    {"ROUND", "Math Functions", "LET r=ROUND(3.7)", "PRINT r", ""},
    {"SIN", "Math Functions", "LET s=SIN(PI/2)", "PRINT s", ""},
    {"COS", "Math Functions", "LET c=COS(0)", "PRINT c", ""},
    {"TAN", "Math Functions", "LET t=TAN(PI/4)", "PRINT t", ""},
    {"ATN", "Math Functions", "LET a=ATN(1)", "PRINT a", ""},
    {"LOG", "Math Functions", "LET l=LOG(10)", "PRINT l", ""},
    {"EXP", "Math Functions", "LET e=EXP(1)", "PRINT e", ""},
    
    // MULTI-ARGUMENT MATH FUNCTIONS
    {"POWER", "Math Functions", "LET p=POWER(2,8)", "PRINT p", ""},
    {"MOD", "Math Functions", "LET m=MOD(17,5)", "PRINT m", ""},
    {"MAX", "Math Functions", "LET big=MAX(10,20)", "PRINT big", ""},
    {"MIN", "Math Functions", "LET small=MIN(10,20)", "PRINT small", ""},
    {"PI", "Math Functions", "LET c=2*PI*r", "PRINT c", ""},
    
    // STRING FUNCTIONS
    {"LEN", "String Functions", "LET l=LEN(\"HELLO\")", "PRINT l", ""},
    {"LEFT$", "String Functions", "LET s=LEFT$(\"HELLO\",2)", "PRINT s", ""},
    {"RIGHT$", "String Functions", "LET s=RIGHT$(\"HELLO\",2)", "PRINT s", ""},
    {"MID$", "String Functions", "LET s=MID$(\"HELLO\",2,3)", "PRINT s", ""},
    {"CHR$", "String Functions", "LET c=CHR$(65)", "PRINT c", ""},
    {"ASC", "String Functions", "LET a=ASC(\"A\")", "PRINT a", ""},
    {"STR$", "String Functions", "LET s=STR$(42)", "PRINT s", ""},
    {"VAL", "String Functions", "LET v=VAL(\"123\")", "PRINT v", ""},
    {"INSTR", "String Functions", "LET p=INSTR(1,\"HELLO\",\"LL\")", "PRINT p", ""},
    {"UCASE$", "String Functions", "LET u=UCASE$(\"hello\")", "PRINT u", ""},
    {"LCASE$", "String Functions", "LET l=LCASE$(\"HELLO\")", "PRINT l", ""},
    
    // SYSTEM VARIABLES
    {"TIMER", "System Variables", "LET t1=TIMER", "LET t2=TIMER", "PRINT t2-t1"},
    {"RND", "System Variables", "LET r=RND", "PRINT r", ""},
    
    // FILE OPERATIONS
    {"OPEN", "File Operations", "OPEN \"file.txt\" FOR OUTPUT AS 1", "", ""},
    {"CLOSE", "File Operations", "CLOSE 1", "", ""},
    {"PRINT#", "File Operations", "PRINT# 1, \"data\"", "", ""},
    {"INPUT#", "File Operations", "INPUT# 1, data$", "", ""},
    
    // DATA OPERATIONS
    {"DATA", "Data Operations", "10 DATA 1,2,3,4,5", "20 READ x", ""},
    {"READ", "Data Operations", "10 DATA 100,200", "20 READ x,y", ""},
    {"RESTORE", "Data Operations", "100 RESTORE", "110 READ x", ""},
    {"DIM", "Data Operations", "DIM array(10)", "DIM matrix(5,5)", ""},
    
    // REM AND UTILITIES
    {"REM", "Utilities", "10 REM This is a comment", "20 PRINT \"Code\"", ""},
    {"HELP", "Utilities", "HELP", "HELP PRINT", "HELP LOOPS"},
};


#define NUM_ENTRIES (sizeof(help_entries) / sizeof(HelpEntry))

// Convert string to uppercase for comparison
static void to_upper(char *str) {
    while (*str) {
        *str = toupper((unsigned char)*str);
        str++;
    }
}

// Get help entry by topic (case-insensitive)
const HelpEntry* help_get(const char *topic) {
    if (!topic || *topic == '\0') {
        return NULL;
    }
    
    char upper_topic[256];
    strncpy(upper_topic, topic, sizeof(upper_topic) - 1);
    upper_topic[sizeof(upper_topic) - 1] = '\0';
    to_upper(upper_topic);
    
    for (int i = 0; i < NUM_ENTRIES; i++) {
        char upper_entry_topic[256];
        strncpy(upper_entry_topic, help_entries[i].topic, sizeof(upper_entry_topic) - 1);
        upper_entry_topic[sizeof(upper_entry_topic) - 1] = '\0';
        to_upper(upper_entry_topic);
        
        if (strcmp(upper_topic, upper_entry_topic) == 0) {
            return &help_entries[i];
        }
    }
    return NULL;
}

// Print a help entry to output
void help_print(const HelpEntry *entry) {
    if (!entry) {
        printf("?HELP TOPIC NOT FOUND\n");
        return;
    }
    
    printf("HELP - %s\n", entry->topic);
    printf("====================================\n");
    
    if (entry->line1 && *entry->line1) {
        printf("%s\n", entry->line1);
    }
    if (entry->line2 && *entry->line2) {
        printf("%s\n", entry->line2);
    }
    if (entry->line3 && *entry->line3) {
        printf("%s\n", entry->line3);
    }
    printf("\n");
}

// List all categories and topics
void help_list_categories(void) {
    printf("HELP - OBI-88 BASIC COMMAND REFERENCE\n");
    printf("====================================\n\n");
    
    // Print by category
    const char *current_category = "";
    for (int i = 0; i < NUM_ENTRIES; i++) {
        if (strcmp(help_entries[i].category, current_category) != 0) {
            current_category = help_entries[i].category;
            printf("%-20s", current_category);
            
            // Print all topics in this category on one line
            int first = 1;
            for (int j = 0; j < NUM_ENTRIES; j++) {
                if (strcmp(help_entries[j].category, current_category) == 0) {
                    if (!first) printf(", ");
                    printf("%s", help_entries[j].topic);
                    first = 0;
                }
            }
            printf("\n");
        }
    }
    
    printf("\n");
    printf("Type: HELP <topic> for code example\n");
    printf("(e.g., HELP PRINT)\n");
}

// Append help code lines to current program
void help_append_to_program(const HelpEntry *entry) {
    if (!entry) {
        return;
    }
    
    // Find the next available line number (in increments of 10)
    int last_line = 0;
    
    // Scan through program to find last line number
    for (int line_num = 10; line_num < 10000; line_num += 10) {
        if (prog_get_line(line_num) == NULL) {
            last_line = line_num - 10;
            break;
        }
    }
    
    int next_line = last_line + 10;
    
    // Build and add each non-empty line
    if (entry->line1 && *entry->line1) {
        char line_buf[512];
        snprintf(line_buf, sizeof(line_buf), "%d %s", next_line, entry->line1);
        prog_store_line(line_buf);
        next_line += 10;
    }
    
    if (entry->line2 && *entry->line2) {
        char line_buf[512];
        snprintf(line_buf, sizeof(line_buf), "%d %s", next_line, entry->line2);
        prog_store_line(line_buf);
        next_line += 10;
    }
    
    if (entry->line3 && *entry->line3) {
        char line_buf[512];
        snprintf(line_buf, sizeof(line_buf), "%d %s", next_line, entry->line3);
        prog_store_line(line_buf);
        next_line += 10;
    }
}
