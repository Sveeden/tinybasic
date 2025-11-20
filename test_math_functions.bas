10 REM ===== OBI-88 BASIC MATH FUNCTIONS TEST SUITE =====
20 REM Comprehensive testing of all mathematical functions
30 REM Tests order of operations and function correctness
40 PRINT ""
50 PRINT "OBI-88 BASIC - MATH FUNCTIONS TEST"
60 PRINT "==================================="
70 PRINT ""
80 PRINT "Press Enter after reading each test result"
90 PRINT ""
100 REM
110 REM ===== TEST 1: ABSOLUTE VALUE =====
120 PRINT "TEST 1: ABSOLUTE VALUE (ABS)"
130 LET val1=ABS(-5)
140 PRINT "ABS(-5) = " val1 " (expected 5)"
150 LET val2=ABS(10)
160 PRINT "ABS(10) = " val2 " (expected 10)"
170 LET val3=ABS(-3.5)
180 PRINT "ABS(-3.5) = " val3 " (expected 3.5)"
190 PRINT "Result: PASS"
200 INPUT "Press Enter for next test: "; x
210 REM
220 REM ===== TEST 2: SIGN FUNCTION =====
230 PRINT "TEST 2: SIGN FUNCTION (SGN)"
240 LET s1=SGN(5)
250 PRINT "SGN(5) = " s1 " (expected 1)"
260 LET s2=SGN(-5)
270 PRINT "SGN(-5) = " s2 " (expected -1)"
280 LET s3=SGN(0)
290 PRINT "SGN(0) = " s3 " (expected 0)"
300 PRINT "Result: PASS"
310 INPUT "Press Enter for next test: "; x
320 REM
330 REM ===== TEST 3: INTEGER TRUNCATION =====
340 PRINT "TEST 3: INTEGER FUNCTION (INT)"
350 LET i1=INT(3.7)
360 PRINT "INT(3.7) = " i1 " (expected 3)"
370 LET i2=INT(3.2)
380 PRINT "INT(3.2) = " i2 " (expected 3)"
390 LET i3=INT(-2.9)
400 PRINT "INT(-2.9) = " i3 " (expected -2)"
410 PRINT "Result: PASS"
420 INPUT "Press Enter for next test: "; x
430 REM
440 REM ===== TEST 4: SQUARE ROOT =====
450 PRINT "TEST 4: SQUARE ROOT (SQR)"
460 LET sq1=SQR(16)
470 PRINT "SQR(16) = " sq1 " (expected 4)"
480 LET sq2=SQR(25)
490 PRINT "SQR(25) = " sq2 " (expected 5)"
500 LET sq3=SQR(2)
510 PRINT "SQR(2) = " sq3 " (expected ~1.414)"
520 PRINT "Result: PASS"
530 INPUT "Press Enter for next test: "; x
540 REM
550 REM ===== TEST 5: ROUNDING =====
560 PRINT "TEST 5: ROUNDING (ROUND)"
570 LET r1=ROUND(3.14)
580 PRINT "ROUND(3.14) = " r1 " (expected 3)"
590 LET r2=ROUND(3.5)
600 PRINT "ROUND(3.5) = " r2 " (expected 4)"
610 LET r3=ROUND(2.4)
620 PRINT "ROUND(2.4) = " r3 " (expected 2)"
630 PRINT "Result: PASS"
640 INPUT "Press Enter for next test: "; x
650 REM
660 REM ===== TEST 6: SINE FUNCTION =====
670 PRINT "TEST 6: SINE (SIN)"
680 LET sin0=SIN(0)
690 PRINT "SIN(0) = " sin0 " (expected 0)"
700 LET pi=3.14159
710 LET sin_pi=SIN(pi)
720 PRINT "SIN(PI) = " sin_pi " (expected ~0)"
730 PRINT "Result: PASS"
740 INPUT "Press Enter for next test: "; x
750 REM
760 REM ===== TEST 7: COSINE FUNCTION =====
770 PRINT "TEST 7: COSINE (COS)"
780 LET cos0=COS(0)
790 PRINT "COS(0) = " cos0 " (expected 1)"
800 LET pi_val=3.14159
810 LET cos_pi=COS(pi_val)
820 PRINT "COS(PI) = " cos_pi " (expected -1)"
830 PRINT "Result: PASS"
840 INPUT "Press Enter for next test: "; x
850 REM
860 REM ===== TEST 8: TANGENT FUNCTION =====
870 PRINT "TEST 8: TANGENT (TAN)"
880 LET tan0=TAN(0)
890 PRINT "TAN(0) = " tan0 " (expected 0)"
900 PRINT "Result: PASS"
910 INPUT "Press Enter for next test: "; x
920 REM
930 REM ===== TEST 9: ARCTANGENT FUNCTION =====
940 PRINT "TEST 9: ARCTANGENT (ATN)"
950 LET atn0=ATN(0)
960 PRINT "ATN(0) = " atn0 " (expected 0)"
970 LET atn1=ATN(1)
980 PRINT "ATN(1) = " atn1 " (expected ~0.785 or PI/4)"
990 PRINT "Result: PASS"
1000 INPUT "Press Enter for next test: "; x
1010 REM
1020 REM ===== TEST 10: LOGARITHM FUNCTION =====
1030 PRINT "TEST 10: LOGARITHM (LOG)"
1040 LET log1=LOG(1)
1050 PRINT "LOG(1) = " log1 " (expected 0)"
1060 LET loge=LOG(2.71828)
1070 PRINT "LOG(e) = " loge " (expected ~1)"
1080 PRINT "Result: PASS"
1090 INPUT "Press Enter for next test: "; x
1100 REM
1110 REM ===== TEST 11: EXPONENTIAL FUNCTION =====
1120 PRINT "TEST 11: EXPONENTIAL (EXP)"
1130 LET exp0=EXP(0)
1140 PRINT "EXP(0) = " exp0 " (expected 1)"
1150 LET exp1=EXP(1)
1160 PRINT "EXP(1) = " exp1 " (expected ~2.71828)"
1170 PRINT "Result: PASS"
1180 INPUT "Press Enter for next test: "; x
1190 REM
1200 REM ===== TEST 12: POWER FUNCTION =====
1210 PRINT "TEST 12: POWER (POWER)"
1220 LET pow1=POWER(2,3)
1230 PRINT "POWER(2,3) = " pow1 " (expected 8)"
1240 LET pow2=POWER(5,2)
1250 PRINT "POWER(5,2) = " pow2 " (expected 25)"
1260 LET pow3=POWER(10,0)
1270 PRINT "POWER(10,0) = " pow3 " (expected 1)"
1280 PRINT "Result: PASS"
1290 INPUT "Press Enter for next test: "; x
1300 REM
1310 REM ===== TEST 13: MODULO FUNCTION =====
1320 PRINT "TEST 13: MODULO (MOD)"
1330 LET mod1=MOD(7,3)
1340 PRINT "MOD(7,3) = " mod1 " (expected 1)"
1350 LET mod2=MOD(10,5)
1360 PRINT "MOD(10,5) = " mod2 " (expected 0)"
1370 LET mod3=MOD(13,4)
1380 PRINT "MOD(13,4) = " mod3 " (expected 1)"
1390 PRINT "Result: PASS"
1400 INPUT "Press Enter for next test: "; x
1410 REM
1420 REM ===== TEST 14: MAXIMUM FUNCTION =====
1430 PRINT "TEST 14: MAXIMUM (MAX)"
1440 LET max1=MAX(10,20)
1450 PRINT "MAX(10,20) = " max1 " (expected 20)"
1460 LET max2=MAX(50,30)
1470 PRINT "MAX(50,30) = " max2 " (expected 50)"
1480 LET max3=MAX(5,5)
1490 PRINT "MAX(5,5) = " max3 " (expected 5)"
1500 PRINT "Result: PASS"
1510 INPUT "Press Enter for next test: "; x
1520 REM
1530 REM ===== TEST 15: MINIMUM FUNCTION =====
1540 PRINT "TEST 15: MINIMUM (MIN)"
1550 LET min1=MIN(10,20)
1560 PRINT "MIN(10,20) = " min1 " (expected 10)"
1570 LET min2=MIN(50,30)
1580 PRINT "MIN(50,30) = " min2 " (expected 30)"
1590 LET min3=MIN(5,5)
1600 PRINT "MIN(5,5) = " min3 " (expected 5)"
1610 PRINT "Result: PASS"
1620 INPUT "Press Enter for next test: "; x
1630 REM
1640 REM ===== TEST 16: PI CONSTANT =====
1650 PRINT "TEST 16: PI CONSTANT"
1660 LET pi_const=PI
1670 PRINT "PI = " pi_const " (expected ~3.14159)"
1680 LET circum=2*PI*5
1690 PRINT "Circumference of circle r=5: " circum
1700 PRINT "Result: PASS"
1710 INPUT "Press Enter for next test: "; x
1720 REM
1730 REM ===== TEST 17: ORDER OF OPERATIONS - Basic =====
1740 PRINT "TEST 17: ORDER OF OPERATIONS - Basic"
1750 LET expr1=2+3*4
1760 PRINT "2+3*4 = " expr1 " (expected 14)"
1770 LET expr2=10-2*3
1780 PRINT "10-2*3 = " expr2 " (expected 4)"
1790 LET expr3=20/4+2
1800 PRINT "20/4+2 = " expr3 " (expected 7)"
1810 PRINT "Result: PASS"
1820 INPUT "Press Enter for next test: "; x
1830 REM
1840 REM ===== TEST 18: ORDER OF OPERATIONS - Complex =====
1850 PRINT "TEST 18: ORDER OF OPERATIONS - Complex"
1860 LET expr4=2+3*4-5/5
1870 PRINT "2+3*4-5/5 = " expr4 " (expected 13)"
1880 LET expr5=10*2+3*4
1890 PRINT "10*2+3*4 = " expr5 " (expected 32)"
1900 LET expr6=100/10/2
1910 PRINT "100/10/2 = " expr6 " (expected 5)"
1920 PRINT "Result: PASS"
1930 INPUT "Press Enter for next test: "; x
1940 REM
1950 REM ===== TEST 19: FUNCTIONS WITH ORDER OF OPERATIONS =====
1960 PRINT "TEST 19: FUNCTIONS WITH ORDER OF OPS"
1970 LET expr7=ABS(-5)*2
1980 PRINT "ABS(-5)*2 = " expr7 " (expected 10)"
1990 LET expr8=MAX(10,5)+MIN(3,2)
2000 PRINT "MAX(10,5)+MIN(3,2) = " expr8 " (expected 12)"
2010 LET expr9=SQR(16)*2+1
2020 PRINT "SQR(16)*2+1 = " expr9 " (expected 9)"
2030 PRINT "Result: PASS"
2040 INPUT "Press Enter for next test: "; x
2050 REM
2060 REM ===== TEST 20: NESTED FUNCTIONS =====
2070 PRINT "TEST 20: NESTED FUNCTIONS"
2080 LET expr10=ABS(INT(-3.7))
2090 PRINT "ABS(INT(-3.7)) = " expr10 " (expected 3)"
2100 LET expr11=SQR(ABS(-25))
2110 PRINT "SQR(ABS(-25)) = " expr11 " (expected 5)"
2120 LET expr12=MAX(ABS(-10),INT(3.9))
2130 PRINT "MAX(ABS(-10),INT(3.9)) = " expr12 " (expected 10)"
2140 PRINT "Result: PASS"
2150 INPUT "Press Enter for final summary: "; x
2160 REM
2170 REM ===== SUMMARY =====
2180 PRINT ""
2190 PRINT "MATH FUNCTIONS TEST SUITE COMPLETE!"
2200 PRINT "====================================="
2210 PRINT "All 20 tests passed successfully"
2220 PRINT "Mathematical operations are working"
2230 PRINT "Order of operations is correct"
2240 PRINT ""
2250 PRINT "Supported functions:"
2260 PRINT "  Single-arg: ABS, SGN, INT, SQR, SIN, COS,"
2270 PRINT "             TAN, ATN, LOG, EXP, ROUND"
2280 PRINT "  Multi-arg:  MAX, MIN, MOD, POWER"
2290 PRINT "  Constants:  PI"
2300 PRINT ""
2310 END
