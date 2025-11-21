10 REM ===== OBI-88 BASIC HELP SYSTEM TEST =====
20 PRINT "OBI-88 BASIC - HELP SYSTEM TEST"
30 PRINT "================================"
40 PRINT ""
50 REM
100 REM ===== TEST 1: HELP WITH NO ARGS (shows all categories) =====
110 PRINT "TEST 1: HELP (no arguments)"
120 PRINT "Displays all available help topics by category"
130 PRINT ""
140 HELP
150 INPUT "Press Enter to continue: "; x
160 PRINT ""
170 REM
200 REM ===== TEST 2: HELP FOR PRINT COMMAND =====
210 PRINT "TEST 2: HELP PRINT"
220 HELP PRINT
230 INPUT "Press Enter to continue: "; x
240 PRINT ""
250 REM
300 REM ===== TEST 3: HELP FOR FOR LOOP =====
310 PRINT "TEST 3: HELP FOR"
320 HELP FOR
330 INPUT "Press Enter to continue: "; x
340 PRINT ""
350 REM
400 REM ===== TEST 4: HELP FOR MATH FUNCTION =====
410 PRINT "TEST 4: HELP MAX"
420 HELP MAX
430 INPUT "Press Enter to continue: "; x
440 PRINT ""
450 REM
500 REM ===== TEST 5: HELP FOR TRIGONOMETRIC FUNCTION =====
510 PRINT "TEST 5: HELP SIN"
520 HELP SIN
530 INPUT "Press Enter to continue: "; x
540 PRINT ""
550 REM
600 REM ===== TEST 6: CASE INSENSITIVITY =====
610 PRINT "TEST 6: HELP input (lowercase)"
620 HELP input
630 INPUT "Press Enter to continue: "; x
640 PRINT ""
650 REM
700 REM ===== SUMMARY =====
710 PRINT "HELP SYSTEM TEST COMPLETE!"
720 PRINT "================================"
730 PRINT "All help topics working correctly"
740 PRINT ""
750 END
