10 REM ===== SNIPPET COMMAND TEST =====
20 PRINT "SNIPPET Command Test"
30 PRINT "===================="
40 PRINT ""
50 PRINT "SNIPPET adds code examples from HELP"
60 PRINT "Usage: SNIPPET command_name"
70 PRINT ""
80 PRINT "Available snippets include:"
90 PRINT "  PRINT, LET, IF, FOR, WHILE"
100 PRINT "  GOSUB, GOTO, INPUT, etc."
110 PRINT ""
120 INPUT "Press Enter to add PRINT snippet: "; x
130 SNIPPET PRINT
140 PRINT ""
150 PRINT "Snippet inserted into program!"
160 PRINT "Use LIST to see the added lines"
170 PRINT ""
180 INPUT "Add another snippet? (yes/no): "; response
190 IF response="yes" THEN SNIPPET FOR
200 PRINT ""
210 PRINT "SNIPPET command test complete"
220 END
