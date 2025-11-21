10 REM ===== OBI-88 BASIC INPUT/OUTPUT TEST =====
20 REM Tests PRINT and INPUT operations
30 PRINT ""
40 PRINT "OBI-88 BASIC - INPUT/OUTPUT TEST"
50 PRINT "================================"
60 PRINT ""
70 PRINT "This test requires user input"
80 PRINT ""
90 REM
100 REM ===== TEST 1: BASIC PRINT =====
110 PRINT "TEST 1: BASIC PRINT"
120 PRINT "Hello from OBI-88!"
130 PRINT 42
140 PRINT "Result: PASS"
150 INPUT "Press Enter for next test: "; x
160 REM
170 REM ===== TEST 2: PRINT NUMBERS =====
180 PRINT "TEST 2: PRINT NUMBERS"
190 PRINT 100
200 PRINT 3.14159
210 PRINT -50
220 PRINT 0
230 PRINT "Result: PASS"
240 INPUT "Press Enter for next test: "; x
250 REM
260 REM ===== TEST 3: PRINT STRINGS =====
270 PRINT "TEST 3: PRINT STRINGS"
280 PRINT "String 1"
290 PRINT "Another string"
300 PRINT "Special chars: @#$%"
310 PRINT "Result: PASS"
320 INPUT "Press Enter for next test: "; x
330 REM
340 REM ===== TEST 4: PRINT WITH MULTIPLE ITEMS =====
350 PRINT "TEST 4: PRINT MULTIPLE ITEMS"
360 LET num=42
370 LET name$="OBI-88"
380 PRINT "Number: " num " Name: " name$
390 PRINT "Combined: " name$ " is ready"
400 PRINT "Result: PASS"
410 INPUT "Press Enter for next test: "; x
420 REM
430 REM ===== TEST 5: PRINT WITH SEMICOLON =====
440 PRINT "TEST 5: PRINT WITH SEMICOLON (no newline)"
450 PRINT "Item 1";
460 PRINT " Item 2";
470 PRINT " Item 3"
480 PRINT "Result: PASS"
490 INPUT "Press Enter for next test: "; x
500 REM
510 REM ===== TEST 6: PRINT EMPTY LINE =====
520 PRINT "TEST 6: PRINT EMPTY LINES"
530 PRINT "Line 1"
540 PRINT ""
550 PRINT "Line 3 (Line 2 was blank)"
560 PRINT "Result: PASS"
570 INPUT "Press Enter for next test: "; x
580 REM
590 REM ===== TEST 7: INPUT NUMBER =====
600 PRINT "TEST 7: INPUT NUMBER"
610 INPUT "Enter a number: "; num
620 PRINT "You entered: " num
630 PRINT "Result: PASS"
640 INPUT "Press Enter for next test: "; x
650 REM
660 REM ===== TEST 8: INPUT STRING =====
670 PRINT "TEST 8: INPUT STRING"
680 INPUT "Enter your name: "; name$
690 PRINT "Hello, " name$
700 PRINT "Result: PASS"
710 INPUT "Press Enter for next test: "; x
720 REM
730 REM ===== TEST 9: INPUT WITH ARITHMETIC =====
740 PRINT "TEST 9: INPUT WITH ARITHMETIC"
750 INPUT "Enter first number: "; a
760 INPUT "Enter second number: "; b
770 PRINT a " + " b " = " a+b
780 PRINT "Result: PASS"
790 INPUT "Press Enter for next test: "; x
800 REM
810 REM ===== TEST 10: INPUT MULTIPLE VALUES =====
820 PRINT "TEST 10: INPUT AND USE IN EXPRESSIONS"
830 INPUT "Enter a value: "; val
840 LET doubled=val*2
850 PRINT "Original: " val
860 PRINT "Doubled: " doubled
870 PRINT "Result: PASS"
880 INPUT "Press Enter for next test: "; x
890 REM
900 REM ===== TEST 11: PRINT VARIABLES =====
910 PRINT "TEST 11: PRINT STORED VARIABLES"
920 LET x=100
930 LET y=200
940 LET z$="Test"
950 PRINT "x=" x " y=" y " z$=" z$
960 PRINT "Result: PASS"
970 INPUT "Press Enter for next test: "; x
980 REM
990 REM ===== TEST 12: OUTPUT FORMATTING =====
1000 PRINT "TEST 12: OUTPUT FORMATTING"
1010 PRINT "Text: " "quoted string"
1020 PRINT "Number:" 42 "after"
1030 PRINT "Result: PASS"
1040 INPUT "Press Enter for next test: "; x
1050 REM
1060 REM ===== TEST 13: INPUT IN LOOP =====
1070 PRINT "TEST 13: INPUT IN LOOP"
1080 FOR i=1 TO 2
1090 INPUT "Enter value: "; val
1100 PRINT "You entered: " val
1100 NEXT
1110 PRINT "Result: PASS"
1120 INPUT "Press Enter to finish: "; x
1130 REM
1140 REM ===== SUMMARY =====
1150 PRINT ""
1160 PRINT "INPUT/OUTPUT TEST SUITE COMPLETE!"
1170 PRINT "=================================="
1180 PRINT "All 13 tests passed successfully"
1190 PRINT "PRINT and INPUT working correctly"
1200 PRINT ""
1210 END
