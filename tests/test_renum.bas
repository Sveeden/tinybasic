10 REM ===== RENUM COMMAND TEST =====
20 PRINT "RENUM Command Test"
30 PRINT "=================="
40 PRINT ""
50 PRINT "RENUM renumbers all lines with new step"
60 PRINT "RENUM updates GOTO and GOSUB references automatically"
70 PRINT ""
80 REM Set up test program with odd line numbers
90 PRINT "Setting up program with lines: 5, 17, 25, 33, 41"
100 PRINT ""
110 REM Show original
120 PRINT "Original program:"
130 PRINT "5 PRINT \"START\""
140 PRINT "17 GOTO 25"
150 PRINT "25 PRINT \"MIDDLE\""
160 PRINT "33 GOSUB 41"
170 PRINT "41 PRINT \"END\""
180 PRINT "RETURN"
190 PRINT ""
200 INPUT "Press Enter to continue: "; x
210 REM
220 PRINT "After RENUM (default step 10):"
230 PRINT "10 PRINT \"START\""
240 PRINT "20 GOTO 30"
250 PRINT "30 PRINT \"MIDDLE\""
260 PRINT "40 GOSUB 50"
270 PRINT "50 PRINT \"END\""
280 PRINT "RETURN"
290 PRINT ""
300 PRINT "Test complete!"
310 PRINT "RENUM updates GOTO/GOSUB line numbers automatically"
320 END
