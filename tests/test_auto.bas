10 REM ===== AUTO COMMAND TEST =====
20 PRINT "AUTO Command Test"
30 PRINT "================="
40 PRINT ""
50 PRINT "AUTO starts line numbering at specified line"
60 PRINT "Default step is 10"
70 PRINT ""
80 PRINT "Starting AUTO mode at line 100..."
90 PRINT "Type these lines (press CTRL+C to exit):"
100 PRINT "  PRINT \"Test 1\""
110 PRINT "  PRINT \"Test 2\""
120 PRINT "  END"
130 PRINT ""
140 INPUT "Ready to test AUTO? "; response
150 PRINT "Entering AUTO 100 mode now:"
160 AUTO 100
170 REM After AUTO, you'll be back here
180 PRINT ""
190 PRINT "Back from AUTO mode"
200 PRINT "Use LIST to see the auto-numbered lines"
210 END
