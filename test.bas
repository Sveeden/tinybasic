10 REM Test 1: PRINT with quoted string
20 PRINT "Test 1: Hello World"
30 INPUT "Press Enter to continue";dummy
40 REM Test 2: PRINT with variable
50 PRINT "Test 2: Print a number"
60 PRINT 42
70 INPUT "Press Enter to continue";dummy
80 REM Test 3: LET with number
90 LET x=10
100 PRINT "Test 3: Set x to 10"
110 INPUT "Press Enter to continue";dummy
120 REM Test 4: PRINT variable
130 PRINT "Test 4: Print x (should be 10):"
140 PRINT x
150 INPUT "Press Enter to continue";dummy
160 REM Test 5: LET with string
170 LET x$="poop"
180 PRINT "Test 5: Set x$ to poop"
190 INPUT "Press Enter to continue";dummy
200 REM Test 6: PRINT string variable
210 PRINT "Test 6: Print x$ (should be poop):"
220 PRINT x$
230 INPUT "Press Enter to continue";dummy
240 REM Test 7: IF THEN true condition
250 PRINT "Test 7: IF x>5 (should execute):"
260 IF x>5 THEN PRINT "x is greater than 5"
270 INPUT "Press Enter to continue";dummy
280 REM Test 8: IF THEN false condition
290 PRINT "Test 8: IF x<5 (should NOT execute):"
300 IF x<5 THEN PRINT "x is less than 5"
310 INPUT "Press Enter to continue";dummy
320 REM Test 9: IF string comparison
330 PRINT "Test 9: IF x$=poop (should execute):"
340 IF x$="poop" THEN PRINT "x$ equals poop"
350 INPUT "Press Enter to continue";dummy
360 REM Test 10: Multiple variables
370 LET y=20
380 LET name$="Rob"
390 PRINT "Test 10: Multiple variables"
400 PRINT "y = "
410 PRINT y
420 PRINT "name$ = "
430 PRINT name$
440 INPUT "Press Enter to continue";dummy
450 REM Test 11: FOR loop
460 PRINT "Test 11: FOR loop 1 to 5"
470 FOR i=1 TO 5
480 PRINT i
490 NEXT
500 INPUT "Press Enter to continue";dummy
510 REM Test 12: WHILE loop with INPUT
520 PRINT "Test 12: WHILE loop (enter 0 to exit)"
530 LET x=5
540 WHILE x>0
550 PRINT x
560 INPUT "Enter next value: "; x
570 WEND
580 PRINT "WHILE loop done!"
590 INPUT "Press Enter to continue";dummy
600 REM All tests complete
610 PRINT "All tests complete!"
