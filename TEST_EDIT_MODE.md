# EDIT Mode Integration Test

## Overview
EDIT mode now pre-loads a line into the input buffer for immediate editing.

## Test Procedure

### Test 1: Basic EDIT with Line Modification
```
> 10 PRINT "HELLO"
> 20 PRINT "WORLD"  
> 30 PRINT "TEST"
> LIST
10 PRINT "HELLO"
20 PRINT "WORLD"
30 PRINT "TEST"
> EDIT 20
20 PRINT "WORLD"
(cursor is here, ready to edit)
```

Expected: Line 20 appears with cursor at the end, ready to edit immediately.

### Test 2: Edit and Modify
After entering EDIT 20, type these keys to change WORLD to GOODBYE:
```
(Select "WORLD" - use backspace 5 times)
GOODBYE
Enter
```

Result: Line 20 should now be `20 PRINT "GOODBYE"` (or similar based on your edits)

### Test 3: EDIT with Cursor Navigation
```
> EDIT 10
10 PRINT "HELLO"
(cursor at end)
```

- Press LEFT arrow 5 times to move to before "HELLO"
- Type "AMAZING " to insert text
- Press RIGHT arrow to move right
- Continue editing

### Test 4: EDIT with Backspace
```
> EDIT 20
20 PRINT "GOODBYE"
(cursor at end)
```

Press backspace multiple times. Line should shrink character by character.

### Test 5: EDIT Non-existent Line
```
> EDIT 999
?LINE NOT FOUND
```

### Test 6: EDIT without Line Number
```
> EDIT
?EDIT requires a line number
```

### Test 7: EDIT then Cancel with Ctrl+C
```
> EDIT 10
10 PRINT "HELLO"
(press Ctrl+C)
^C
>
```

Should return to prompt without modifying the line.

### Test 8: Multiple EDIT Operations
```
> EDIT 10
10 PRINT "HELLO"
(modify to) 10 PRINT "TEST"
> EDIT 20
20 PRINT "WORLD"
(modify to) 20 PRINT "MODIFIED"
> LIST
10 PRINT "TEST"
20 PRINT "MODIFIED"
30 PRINT "TEST"
```

## Expected Behavior Summary

1. **Display**: Line number and content appear immediately (no need to press Enter first)
2. **Cursor**: Positioned at the end of the line content
3. **Character Input**: Inserts characters at cursor position with line redraw
4. **Backspace**: Deletes character before cursor, shifts remaining text
5. **Left/Right Arrows**: Move cursor within the line
6. **Ctrl+C**: Cancels edit without saving
7. **Enter**: Confirms edit and stores the modified line
8. **Line Update**: Modified line replaces the original line in memory
9. **Return to Prompt**: After edit completes, returns to normal prompt

## Line Numbers in EDIT

Important: When editing, the line number is part of the display but NOT part of the editable content.
- Display shows: `20 PRINT "HELLO"`
- User can only edit: `PRINT "HELLO"` part
- System stores as: `20 PRINT "HELLO"` (full line with number)

## Integration Points

The EDIT mode now works through:
1. User types: `EDIT 20`
2. `execute.c` recognizes TOKEN_EDIT
3. Calls `set_edit_mode(20, "PRINT \"HELLO\"")` 
4. Returns from execute()
5. Main loop detects `is_edit_mode()` is true
6. Pre-loads "PRINT \"HELLO\"" into line buffer
7. Displays "20 PRINT \"HELLO\"" with cursor ready
8. User can edit immediately without pressing Enter
9. When user presses Enter, line is stored and edit mode cleared
10. Returns to main prompt

## Differences from Before

**Before**: EDIT just printed the line and moved to next prompt
**Now**: EDIT pre-loads the line and allows immediate editing with cursor

## Testing Commands

For reference, here are useful commands to set up test data:
```
AUTO
10 PRINT "LINE 10"
20 PRINT "LINE 20"  
30 PRINT "LINE 30"
40 PRINT "LINE 40"
CTRL+C (exit AUTO)

LIST
EDIT 20
(modify and press Enter)

LIST
```
