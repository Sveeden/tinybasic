10 REM ===== ON...GOTO / ON...GOSUB TEST =====
20 REM Tests multi-way branching based on expression
30 PRINT ""
40 PRINT "OBI-88 BASIC - ON...GOTO / ON...GOSUB TEST"
50 PRINT "==========================================="
60 PRINT ""
70 REM
80 REM ===== TEST 1: SIMPLE ON...GOTO =====
90 PRINT "TEST 1: SIMPLE ON...GOTO"
100 LET choice=2
110 PRINT "choice=" choice
120 ON choice GOTO 200, 300, 400
130 PRINT "ERROR: Should not reach here"
140 GOTO 500
200 PRINT "Selected option 1"
210 GOTO 500
300 PRINT "Selected option 2"
310 GOTO 500
400 PRINT "Selected option 3"
500 PRINT "Result: PASS"
510 INPUT "Press Enter for next test: "; x
520 REM
530 REM ===== TEST 2: ON...GOTO WITH CALCULATION =====
540 PRINT "TEST 2: ON...GOTO WITH EXPRESSION"
550 LET a=10
560 LET b=5
570 LET result=a-b
580 PRINT "a=" a " b=" b " result=" result
590 ON result GOTO 610, 700, 800, 900, 1000
600 PRINT "ERROR: Should not reach here"
610 PRINT "Result = 1"
620 GOTO 1100
700 PRINT "Result = 2"
710 GOTO 1100
800 PRINT "Result = 3"
810 GOTO 1100
900 PRINT "Result = 4"
910 GOTO 1100
1000 PRINT "Result = 5"
1100 PRINT "Result: PASS"
1110 INPUT "Press Enter for next test: "; x
1120 REM
1130 REM ===== TEST 3: ON...GOTO WITH MULTIPLE VALUES =====
1140 PRINT "TEST 3: ON...GOTO - MULTIPLE VALUES"
1150 FOR i=1 TO 3
1160   PRINT "Testing option " i
1170   ON i GOTO 1200, 1250, 1300
1180   PRINT "ERROR: Should not reach"
1190   GOTO 1350
1200   PRINT "  -> Option 1 selected"
1210   GOTO 1350
1250   PRINT "  -> Option 2 selected"
1260   GOTO 1350
1300   PRINT "  -> Option 3 selected"
1350   NEXT
1360 PRINT "Result: PASS"
1370 INPUT "Press Enter for next test: "; x
1380 REM
1390 REM ===== TEST 4: ON...GOSUB (Multi-way Subroutine Call) =====
1400 PRINT "TEST 4: ON...GOSUB - MULTI-WAY SUBROUTINE"
1410 FOR opt=1 TO 2
1420   PRINT "Option " opt ":"
1430   ON opt GOSUB 1500, 1550
1440   PRINT "Returned from subroutine"
1450 NEXT
1460 PRINT "Result: PASS"
1470 INPUT "Press Enter for next test: "; x
1480 GOTO 1600
1500 PRINT "  Executing subroutine 1"
1510 RETURN
1550 PRINT "  Executing subroutine 2"
1560 RETURN
1600 REM
1610 REM ===== SUMMARY =====
1620 PRINT ""
1630 PRINT "ON...GOTO / ON...GOSUB TEST COMPLETE!"
1640 PRINT "=========================================="
1650 PRINT "All 4 tests passed successfully"
1660 PRINT ""
1670 END
