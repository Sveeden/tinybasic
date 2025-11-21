10 REM Test ON...GOSUB with proper structure
20 PRINT "ON...GOSUB - Call different subroutines"
30 PRINT ""
40 FOR choice = 1 TO 3
50   PRINT "Calling subroutine ";choice;":"
60   ON choice GOSUB 200, 300, 400
70   PRINT "Returned from subroutine"
80   PRINT ""
90 NEXT
100 PRINT "ON...GOSUB test complete!"
110 END
200 PRINT "  -> Subroutine 1 executing"
210 RETURN
300 PRINT "  -> Subroutine 2 executing"
310 RETURN
400 PRINT "  -> Subroutine 3 executing"
410 RETURN
