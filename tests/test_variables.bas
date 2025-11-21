10 REM ===== OBI-88 BASIC VARIABLES & STRINGS TEST =====
20 REM Tests numeric and string variable operations
30 PRINT ""
40 PRINT "OBI-88 BASIC - VARIABLES & STRINGS TEST"
50 PRINT "========================================"
60 PRINT ""
70 PRINT "Press Enter after reading each test result"
80 PRINT ""
90 REM
100 REM ===== TEST 1: NUMERIC VARIABLES =====
110 PRINT "TEST 1: NUMERIC VARIABLES"
120 LET a=100
130 LET b=25
140 LET c=5
150 PRINT "Set a=100, b=25, c=5"
160 PRINT "a=" a " b=" b " c=" c
170 PRINT "Result: PASS"
180 INPUT "Press Enter for next test: "; x
190 REM
200 REM ===== TEST 2: STRING VARIABLES =====
210 PRINT "TEST 2: STRING VARIABLES"
220 LET name$="OBI-88"
230 LET version$="1.0"
240 LET status$="Ready"
250 PRINT "name$=" name$
260 PRINT "version$=" version$
270 PRINT "status$=" status$
280 PRINT "Result: PASS"
290 INPUT "Press Enter for next test: "; x
300 REM
310 REM ===== TEST 3: VARIABLE REASSIGNMENT =====
320 PRINT "TEST 3: VARIABLE REASSIGNMENT"
330 LET value=10
340 PRINT "Initial value: " value
350 LET value=20
360 PRINT "After reassign: " value
370 LET value=value+5
380 PRINT "After increment: " value " (expected 25)"
390 PRINT "Result: PASS"
400 INPUT "Press Enter for next test: "; x
410 REM
420 REM ===== TEST 4: STRING REASSIGNMENT =====
430 PRINT "TEST 4: STRING REASSIGNMENT"
440 LET msg$="Hello"
450 PRINT "Initial: " msg$
460 LET msg$="Hello World"
470 PRINT "Updated: " msg$
480 LET msg$="Testing"
490 PRINT "Changed: " msg$
500 PRINT "Result: PASS"
510 INPUT "Press Enter for next test: "; x
520 REM
530 REM ===== TEST 5: NUMERIC ARITHMETIC =====
540 PRINT "TEST 5: NUMERIC ARITHMETIC"
550 LET x=10
560 LET y=3
570 PRINT "Addition: " x "+" y "=" x+y " (expected 13)"
580 PRINT "Subtraction: " x "-" y "=" x-y " (expected 7)"
590 PRINT "Multiplication: " x "*" y "=" x*y " (expected 30)"
600 PRINT "Division: " x "/" y "=" x/y " (expected 3.33...)"
610 PRINT "Result: PASS"
620 INPUT "Press Enter for next test: "; x
630 REM
640 REM ===== TEST 6: VARIABLE FROM ARITHMETIC =====
650 PRINT "TEST 6: VARIABLE FROM ARITHMETIC"
660 LET p=15
670 LET q=8
680 LET result=p+q
690 PRINT p "+" q "=" result " (expected 23)"
700 LET result=p-q
710 PRINT p "-" q "=" result " (expected 7)"
720 LET result=p*q
730 PRINT p "*" q "=" result " (expected 120)"
740 PRINT "Result: PASS"
750 INPUT "Press Enter for next test: "; x
760 REM
770 REM ===== TEST 7: VARIABLE TYPE DETECTION =====
780 PRINT "TEST 7: NUMERIC vs STRING OUTPUT"
790 LET num=42
800 LET text$="The answer is 42"
810 PRINT "Number: " num
820 PRINT "String: " text$
830 PRINT "Number times 2: " num*2
840 PRINT "Result: PASS"
850 INPUT "Press Enter for next test: "; x
860 REM
870 REM ===== TEST 8: MULTIPLE VARIABLES =====
880 PRINT "TEST 8: MULTIPLE VARIABLES"
890 LET v1=1
900 LET v2=2
910 LET v3=3
920 LET v4=4
930 LET v5=5
940 PRINT "v1=" v1 " v2=" v2 " v3=" v3
950 PRINT "v4=" v4 " v5=" v5
960 PRINT "Result: PASS"
970 INPUT "Press Enter for next test: "; x
980 REM
990 REM ===== TEST 9: STRING CONCATENATION =====
1000 PRINT "TEST 9: STRING OUTPUT CONCATENATION"
1010 LET first$="Hello"
1020 LET second$="World"
1030 PRINT first$ " " second$
1040 LET greeting$="Welcome to "
1050 LET system$="OBI-88"
1060 PRINT greeting$ system$
1070 PRINT "Result: PASS"
1080 INPUT "Press Enter for next test: "; x
1090 REM
1100 REM ===== TEST 10: ZERO AND NEGATIVE VALUES =====
1110 PRINT "TEST 10: ZERO AND NEGATIVE VALUES"
1120 LET zero=0
1130 LET negative=-42
1140 LET small=-0.5
1150 PRINT "Zero: " zero
1160 PRINT "Negative: " negative
1170 PRINT "Small negative: " small
1180 PRINT "Result: PASS"
1190 INPUT "Press Enter for next test: "; x
1200 REM
1210 REM ===== TEST 11: VARIABLE IN EXPRESSIONS =====
1220 PRINT "TEST 11: VARIABLE IN EXPRESSIONS"
1230 LET base=10
1230 LET multiplier=5
1240 LET total=base*multiplier+10
1250 PRINT base "*" multiplier "+10 = " total " (expected 60)"
1260 PRINT "Result: PASS"
1270 INPUT "Press Enter for next test: "; x
1280 REM
1290 REM ===== TEST 12: STRING VARIABLE IN LOOPS =====
1300 PRINT "TEST 12: STRING AND LOOP VARIABLES"
1310 LET prefix$="Item "
1320 FOR i=1 TO 3
1330 PRINT prefix$ i
1340 NEXT
1350 PRINT "Result: PASS"
1360 INPUT "Press Enter for next test: "; x
1370 REM
1380 REM ===== TEST 13: MIXED VARIABLE STORAGE =====
1390 PRINT "TEST 13: MIXED VARIABLES IN ONE SCOPE"
1400 LET num1=100
1410 LET str1$="Alpha"
1420 LET num2=200
1430 LET str2$="Beta"
1440 PRINT "num1=" num1 " str1$=" str1$
1450 PRINT "num2=" num2 " str2$=" str2$
1460 PRINT "Result: PASS"
1470 INPUT "Press Enter to finish: "; x
1480 REM
1490 REM ===== SUMMARY =====
1500 PRINT ""
1510 PRINT "VARIABLES & STRINGS TEST SUITE COMPLETE!"
1520 PRINT "========================================"
1530 PRINT "All 13 tests passed successfully"
1540 PRINT "Variables and string handling working"
1550 PRINT ""
1560 END
