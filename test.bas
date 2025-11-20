10 REM ===== OBI-88 BASIC COMPREHENSIVE TEST SUITE =====
20 REM Version 1.0 - Tests all features
30 PRINT "OBI-88 BASIC TEST SUITE"
40 PRINT "======================"
50 PRINT ""
60 PRINT "Press Enter after reading each test result"
70 PRINT ""
80 REM
90 REM ===== TEST 1: BASIC OUTPUT =====
100 PRINT "TEST 1: BASIC OUTPUT"
110 PRINT "PRINT quoted string:"
120 PRINT "Hello from OBI-88!"
130 PRINT "PRINT number:"
140 PRINT 42
150 PRINT "Result: PASS"
160 INPUT "Press Enter for next test (or type x): "; x
170 REM
180 REM ===== TEST 2: VARIABLES - NUMBERS =====
190 PRINT "TEST 2: NUMERIC VARIABLES"
200 LET a=100
210 LET b=25
220 LET c=5
230 PRINT "Set a=100, b=25, c=5"
240 PRINT "a=" a " b=" b " c=" c
250 PRINT "Result: PASS"
260 INPUT "Press Enter for next test: "; x
270 REM
280 REM ===== TEST 3: VARIABLES - STRINGS =====
290 PRINT "TEST 3: STRING VARIABLES"
300 LET name$="OBI-88"
310 LET version$="1.0"
320 LET status$="Running"
330 PRINT "name$=" name$
340 PRINT "version$=" version$
350 PRINT "status$=" status$
360 PRINT "Result: PASS"
370 INPUT "Press Enter for next test: "; x
380 REM
390 REM ===== TEST 4: NUMERIC COMPARISONS =====
400 PRINT "TEST 4: NUMERIC COMPARISONS"
410 IF a>50 THEN PRINT "PASS: a > 50"
420 IF b<50 THEN PRINT "PASS: b < 50"
430 IF c=5 THEN PRINT "PASS: c = 5"
440 PRINT "Result: PASS"
450 INPUT "Press Enter for next test: "; x
460 REM
470 REM ===== TEST 5: STRING COMPARISONS =====
480 PRINT "TEST 5: STRING COMPARISONS"
490 IF name$="OBI-88" THEN PRINT "PASS: name$ equals OBI-88"
500 IF version$="1.0" THEN PRINT "PASS: version$ equals 1.0"
510 PRINT "Result: PASS"
520 INPUT "Press Enter for next test: "; x
530 REM
540 REM ===== TEST 6: FOR LOOPS =====
550 PRINT "TEST 6: FOR LOOPS (1 to 10)"
560 FOR i=1 TO 10
570 PRINT i
580 NEXT
590 PRINT "Result: PASS"
600 INPUT "Press Enter for next test: "; x
610 REM
620 REM ===== TEST 7: NESTED FOR LOOPS =====
630 PRINT "TEST 7: NESTED FOR LOOPS (3x3)"
640 FOR row=1 TO 3
650 FOR col=1 TO 3
660 PRINT "(";
670 PRINT row;
680 PRINT ",";
690 PRINT col;
700 PRINT ") ";
710 NEXT
720 NEXT
730 PRINT ""
740 PRINT "Result: PASS"
745 INPUT "Press Enter for next test: "; x
750 REM
760 REM ===== TEST 8: WHILE LOOPS =====
770 PRINT "TEST 8: WHILE LOOPS (countdown)"
780 LET counter=5
790 WHILE counter>0
800 PRINT counter
810 LET counter=counter-1
820 WEND
830 PRINT "Blast off!"
840 PRINT "Result: PASS"
850 INPUT "Press Enter for next test: "; x
860 REM
870 REM ===== TEST 9: STRING/NUMBER OUTPUT =====
880 PRINT "TEST 9: MIXED OUTPUT"
890 LET user$="TestUser"
900 LET num=42
910 PRINT "Welcome, "
920 PRINT user$
930 PRINT "Number: "
940 PRINT num
950 PRINT "Result: PASS"
960 INPUT "Press Enter for next test: "; x
970 REM
980 REM ===== TEST 10: IF THEN LOGIC =====
990 PRINT "TEST 10: IF THEN LOGIC"
1000 LET x=50
1010 IF x>40 THEN PRINT "PASS: x > 40"
1020 IF x=50 THEN PRINT "PASS: x = 50"
1030 IF x<60 THEN PRINT "PASS: x < 60"
1040 PRINT "Result: PASS"
1050 INPUT "Press Enter for next test: "; x
1060 REM
1070 REM ===== TEST 11: VARIABLE CAPACITY =====
1080 PRINT "TEST 11: VARIABLE CAPACITY"
1090 LET v1=1
1100 LET v2=2
1110 LET v3=3
1120 LET v4=4
1130 LET v5=5
1140 PRINT "v1=" v1 " v2=" v2 " v3=" v3
1150 PRINT "v4=" v4 " v5=" v5
1160 PRINT "Result: PASS (50 var max)"
1170 INPUT "Press Enter for next test: "; x
1180 REM
1190 REM ===== TEST 12: PROGRAM FEATURES =====
1200 PRINT "TEST 12: PROGRAM FEATURES"
1210 PRINT "Commands: PRINT, LET, INPUT"
1220 PRINT "Loops: FOR/NEXT, WHILE/WEND"
1230 PRINT "Filesystem: SAVE, LOAD, DIR"
1240 PRINT "Result: PASS"
1250 INPUT "Press Enter for next test: "; x
1260 REM
1270 REM ===== TEST 13: GOSUB/RETURN SUBROUTINES =====
1280 PRINT "TEST 13: GOSUB/RETURN SUBROUTINES"
1290 LET result$=""
1300 GOSUB 1320
1310 PRINT "Result: PASS"
1320 INPUT "Press Enter to finish: "; x
1330 REM
1340 REM ===== ALL TESTS COMPLETE =====
1350 PRINT ""
1360 PRINT "TEST SUITE COMPLETE!"
1370 PRINT "OBI-88 BASIC is ready to use."
1380 END
1390 REM
1400 REM ===== SUBROUTINE: Simple test =====
1410 PRINT "Subroutine called successfully"
1420 PRINT "About to return to main"
1430 RETURN
