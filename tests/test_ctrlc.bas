10 REM ===== CTRL+C INTERRUPT TEST =====
20 PRINT "Infinite loop - press Ctrl+C to break"
30 PRINT ""
40 PRINT "Test 1: GOTO infinite loop"
50 PRINT "Counting: "
60 LET i=1
70 PRINT i " ";
80 LET i=i+1
90 IF i<=1000 THEN GOTO 70
100 PRINT ""
110 PRINT ""
120 PRINT "Test 1 complete - Ctrl+C should have stopped this if needed"
130 END
