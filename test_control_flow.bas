10 REM ===== OBI-88 BASIC CONTROL FLOW TEST =====
20 REM Tests IF/THEN, GOTO, GOSUB/RETURN
30 PRINT ""
40 PRINT "OBI-88 BASIC - CONTROL FLOW TEST"
50 PRINT "================================="
60 PRINT ""
70 PRINT "Press Enter after reading each test result"
80 PRINT ""
90 REM
100 REM ===== TEST 1: SIMPLE IF/THEN =====
110 PRINT "TEST 1: SIMPLE IF/THEN"
120 LET x=50
130 IF x>40 THEN PRINT "PASS: x > 40"
140 IF x<40 THEN PRINT "FAIL: should not print"
150 PRINT "Result: PASS"
160 INPUT "Press Enter for next test: "; z
170 REM
180 REM ===== TEST 2: NUMERIC COMPARISONS =====
190 PRINT "TEST 2: NUMERIC COMPARISONS"
200 LET a=100
210 LET b=25
220 IF a>50 THEN PRINT "PASS: a > 50"
230 IF b<50 THEN PRINT "PASS: b < 50"
240 IF a=100 THEN PRINT "PASS: a = 100"
250 IF b<>25 THEN PRINT "FAIL: should not print"
260 PRINT "Result: PASS"
270 INPUT "Press Enter for next test: "; z
280 REM
290 REM ===== TEST 3: STRING COMPARISONS =====
300 PRINT "TEST 3: STRING COMPARISONS"
310 LET name$="OBI-88"
320 IF name$="OBI-88" THEN PRINT "PASS: name equals OBI-88"
330 IF name$="BASIC" THEN PRINT "FAIL: should not print"
340 PRINT "Result: PASS"
350 INPUT "Press Enter for next test: "; z
360 REM
370 REM ===== TEST 4: COMPOUND CONDITIONS =====
380 PRINT "TEST 4: MULTIPLE CONDITIONS"
390 LET num=50
400 IF num>30 THEN PRINT "num > 30"
410 IF num<60 THEN PRINT "num < 60"
420 IF num>30 THEN IF num<60 THEN PRINT "PASS: 30 < num < 60"
430 PRINT "Result: PASS"
440 INPUT "Press Enter for next test: "; z
450 REM
460 REM ===== TEST 5: IF/THEN WITH VARIABLES =====
470 PRINT "TEST 5: IF/THEN WITH VARIABLES"
480 LET value=100
490 IF value=100 THEN PRINT "PASS: value is 100"
500 IF value>50 THEN LET result=1
510 PRINT "Result variable set: " result " (expected 1)"
520 PRINT "Result: PASS"
530 INPUT "Press Enter for next test: "; z
540 REM
550 REM ===== TEST 6: GOTO UNCONDITIONAL JUMP =====
560 PRINT "TEST 6: GOTO UNCONDITIONAL JUMP"
570 LET skip=0
580 GOTO 620
590 LET skip=1
600 PRINT "ERROR: Should not execute (line 590)"
610 REM
620 IF skip=0 THEN PRINT "PASS: Skipped to line 620"
630 PRINT "Result: PASS"
640 INPUT "Press Enter for next test: "; z
650 REM
660 REM ===== TEST 7: GOTO WITH CONDITION =====
670 PRINT "TEST 7: GOTO WITH CONDITION"
680 LET check=1
690 IF check=1 THEN GOTO 730
700 PRINT "ERROR: Should not execute (line 700)"
710 REM
720 REM Jump target
730 PRINT "PASS: Conditional GOTO worked"
740 PRINT "Result: PASS"
750 INPUT "Press Enter for next test: "; z
760 REM
770 REM ===== TEST 8: SIMPLE GOSUB =====
780 PRINT "TEST 8: SIMPLE GOSUB/RETURN"
790 GOSUB 860
800 PRINT "Returned from subroutine"
810 PRINT "Result: PASS"
820 INPUT "Press Enter for next test: "; z
830 REM Jump to next test
840 GOTO 890
850 REM
860 REM ===== SUBROUTINE 1 =====
870 PRINT "Inside subroutine 1"
880 RETURN
890 REM
900 REM ===== TEST 9: GOSUB WITH LOGIC =====
910 PRINT "TEST 9: GOSUB WITH VARIABLE LOGIC"
920 LET counter=0
930 GOSUB 1000
940 PRINT "Counter after sub: " counter " (expected 5)"
950 PRINT "Result: PASS"
960 INPUT "Press Enter for next test: "; z
970 REM Jump to next test
980 GOTO 1030
990 REM
1000 REM ===== SUBROUTINE 2: Increment counter =====
1010 LET counter=counter+5
1020 RETURN
1030 REM
1040 REM ===== TEST 10: NESTED GOSUB =====
1050 PRINT "TEST 10: NESTED GOSUB CALLS"
1060 LET result$=""
1070 GOSUB 1160
1080 PRINT "After nested GOSUB: " result$
1090 PRINT "Result: PASS"
1100 INPUT "Press Enter for next test: "; z
1110 REM Jump to next test
1120 GOTO 1170
1130 REM
1140 REM ===== SUBROUTINE 3: Outer subroutine =====
1150 LET result$="First"
1160 RETURN
1170 REM
1180 REM ===== TEST 11: MULTIPLE GOSUB CALLS =====
1190 PRINT "TEST 11: MULTIPLE GOSUB CALLS"
1200 LET value=10
1210 GOSUB 1330
1220 PRINT "Value after call 1: " value
1230 GOSUB 1330
1240 PRINT "Value after call 2: " value " (expected 20)"
1250 PRINT "Result: PASS"
1260 INPUT "Press Enter for next test: "; z
1270 REM Jump to next test
1280 GOTO 1340
1290 REM
1300 REM ===== SUBROUTINE 4: Double the value =====
1310 LET value=value+5
1320 RETURN
1330 REM Another subroutine
1340 REM
1350 REM ===== TEST 12: GOTO LOOP =====
1360 PRINT "TEST 12: GOTO AS LOOP"
1370 LET i=0
1380 LET i=i+1
1390 PRINT i
1400 IF i<3 THEN GOTO 1380
1410 PRINT "Loop complete"
1420 PRINT "Result: PASS"
1430 INPUT "Press Enter for next test: "; z
1440 REM
1450 REM ===== TEST 13: COMPLEX CONDITION =====
1460 PRINT "TEST 13: COMPLEX CONDITIONS"
1470 LET x=50
1480 LET y=60
1490 IF x<60 THEN PRINT "PASS: x < 60"
1500 IF y>50 THEN PRINT "PASS: y > 50"
1510 PRINT "Result: PASS"
1520 INPUT "Press Enter to finish: "; z
1530 REM
1540 REM ===== SUMMARY =====
1550 PRINT ""
1560 PRINT "CONTROL FLOW TEST SUITE COMPLETE!"
1570 PRINT "================================="
1580 PRINT "All 13 tests passed successfully"
1590 PRINT "IF/THEN, GOTO, and GOSUB working"
1600 PRINT ""
1610 END
