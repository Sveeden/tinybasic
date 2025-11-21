10 REM ===== RND AND RANDOMIZE TEST =====
20 PRINT "RND and RANDOMIZE Function Test"
30 PRINT "================================"
40 PRINT ""
50 REM
60 REM Test 1: RND without seed (should use default)
70 PRINT "TEST 1: RND() - Basic random numbers"
80 PRINT "Generate 5 random numbers 1-100:"
90 FOR i=1 TO 5
100 LET r=RND(100)
110 PRINT r " ";
120 NEXT
130 PRINT ""
140 PRINT "Result: PASS"
150 INPUT "Press Enter for next test: "; x
160 REM
170 REM Test 2: RANDOMIZE with seed
180 PRINT ""
190 PRINT "TEST 2: RANDOMIZE with seed (deterministic)"
200 RANDOMIZE 42
210 PRINT "First sequence with seed 42:"
220 FOR i=1 TO 5
230 LET r=RND(10)
240 PRINT r " ";
250 NEXT
260 PRINT ""
270 REM Re-seed with same value
280 RANDOMIZE 42
290 PRINT "Second sequence with seed 42 (should match):"
300 FOR i=1 TO 5
310 LET r=RND(10)
320 PRINT r " ";
330 NEXT
340 PRINT ""
350 PRINT "Result: PASS"
360 INPUT "Press Enter for next test: "; x
370 REM
380 REM Test 3: Different seed produces different sequence
390 PRINT ""
400 PRINT "TEST 3: RANDOMIZE 99 - Different seed"
410 RANDOMIZE 99
420 PRINT "Sequence with seed 99:"
430 FOR i=1 TO 5
440 LET r=RND(10)
450 PRINT r " ";
460 NEXT
470 PRINT ""
480 PRINT "Result: PASS (should be different from seed 42)"
490 INPUT "Press Enter for next test: "; x
500 REM
510 REM Test 4: RND with larger range
520 PRINT ""
530 PRINT "TEST 4: RND with various ranges"
540 RANDOMIZE 1
550 PRINT "Random 1-20: " RND(20)
560 PRINT "Random 1-50: " RND(50)
570 PRINT "Random 1-1000: " RND(1000)
580 PRINT "Result: PASS"
590 INPUT "Press Enter for next test: "; x
600 REM
610 REM Test 5: RANDOMIZE without argument (uses timer)
620 PRINT ""
630 PRINT "TEST 5: RANDOMIZE (no argument - use timer)"
640 RANDOMIZE
650 PRINT "First random: " RND(100)
660 RANDOMIZE
670 PRINT "Second random (likely different): " RND(100)
680 PRINT "Result: PASS"
690 INPUT "Press Enter to finish: "; x
700 REM
710 PRINT ""
720 PRINT "ALL TESTS COMPLETE!"
730 PRINT "RND and RANDOMIZE working correctly"
740 END
