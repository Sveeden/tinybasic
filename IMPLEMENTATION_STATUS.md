# TRS-80 Model 100 BASIC Implementation Status

## Program Editing & Execution

| Command | Status | Notes |
|---------|--------|-------|
| RUN | ✅ | Starts execution from lowest line |
| LIST | ✅ | Displays program |
| NEW | ✅ | Clears program and variables |
| DELETE | ✅ | Deletes line ranges |
| AUTO | ❌ | Auto line numbering not implemented |
| RENUM | ❌ | Renumbering not implemented |
| LOAD | ✅ | Loads from storage (filesystem) |
| SAVE | ✅ | Saves to storage |
| MERGE | ❌ | Combine files not implemented |
| SYSTEM | ❌ | Exit to OS not applicable (embedded) |

## Control Flow

| Command | Status | Notes |
|---------|--------|-------|
| GOTO | ✅ | Unconditional jumps |
| GOSUB | ✅ | Subroutines with return stack |
| RETURN | ✅ | Return from subroutine |
| FOR / NEXT | ✅ | Definite loops with STEP support |
| WHILE / WEND | ✅ | Indefinite loops working |
| IF / THEN / ELSE | ✅ | Conditional execution |
| END | ✅ | Stop execution |
| STOP | ❌ | Breakpoint/debug stop not implemented |
| ON...GOTO / ON...GOSUB | ✅ | Multi-way branching based on expression value |
| DEF | ❌ | User-defined functions not implemented |
| CALL | ❌ | Machine code calls not applicable |
| REM | ✅ | Comments supported |

## Numeric Functions

| Function | Status | Notes |
|----------|--------|-------|
| ABS(X) | ✅ | Absolute value |
| SQR(X) | ✅ | Square root |
| INT(X) | ✅ | Integer floor |
| FIX(X) | ❌ | Truncate toward zero not implemented |
| SGN(X) | ✅ | Sign function |
| RND | ✅ | Random number (1 to N) |
| RANDOMIZE | ✅ | Seed RNG with optional parameter |
| SIN(X) | ✅ | Sine (radians) |
| COS(X) | ✅ | Cosine (radians) |
| TAN(X) | ✅ | Tangent (radians) |
| ATN(X) | ✅ | Arctangent (radians) |
| LOG(X) | ✅ | Natural logarithm |
| EXP(X) | ✅ | e^X |
| PEEK(A) | ❌ | Memory peek not implemented |
| FRE(X) | ❌ | Free memory not implemented |

## String Functions and Variables

| Function | Status | Notes |
|----------|--------|-------|
| LEN(A$) | ✅ | String length |
| LEFT$(A$, N) | ✅ | Leftmost N chars |
| RIGHT$(A$, N) | ✅ | Rightmost N chars |
| MID$(A$, N, M) | ✅ | Substring |
| CHR$(N) | ✅ | ASCII to char |
| ASC(A$) | ✅ | Char to ASCII |
| STR$(X) | ✅ | Number to string |
| VAL(A$) | ✅ | String to number |
| INSTR(N, A$, B$) | ✅ | Find substring |
| LCASE$(A$) | ✅ | Lowercase |
| UCASE$(A$) | ✅ | Uppercase |

## I/O, Time, and System Control

| Command/Function | Status | Notes |
|------------------|--------|-------|
| PRINT | ✅ | Display output |
| INPUT | ✅ | Read user input |
| INKEY$ | ❌ | Non-blocking keyboard input not implemented |
| LPRINT | ❌ | Printer output not applicable |
| LOCATE X, Y | ❌ | Cursor positioning not implemented |
| CLS | ✅ | Clear screen |
| BEEP | ✅ | Sound beep (terminal bell) |
| CLEAR | 🔄 | Clear variables (tokenized, handler added) |
| POKE A, V | ❌ | Memory write not implemented |
| TIME$ | ❌ | Time string not implemented |
| DATE$ | ❌ | Date string not implemented |
| TIMER | ✅ | Milliseconds since boot (system variable) |

## File and Data Management

| Command | Status | Notes |
|---------|--------|-------|
| OPEN / CLOSE | 🔄 | File operations (tokenized, not yet implemented) |
| PRINT# / INPUT# | 🔄 | File I/O (tokenized, not yet implemented) |
| FIELD / LSET / RSET | ❌ | Random access buffer commands not implemented |
| DATA / READ / RESTORE | 🔄 | Static data (tokenized, not yet implemented) |
| DIM | 🔄 | Array declaration (tokenized, not yet implemented) |

---

## Priority Implementation Queue

### Tier 1: High Priority (Core Features)
1. AUTO - Auto line numbering ✅
2. RENUM - Renumber program ✅
3. WHILE / WEND - While loops ✅
4. RND / RANDOMIZE - Random numbers ✅
5. ON...GOTO / ON...GOSUB - Multi-way branching ✅
6. INKEY$ - Non-blocking input
7. DATE$ / TIME$ - Date/time strings

### Tier 2: Medium Priority (Nice to Have)
8. DEF - User-defined functions
9. STOP - Breakpoint/debug stop
10. FRE(X) - Free memory
11. FIX(X) - Truncate toward zero
12. LOCATE X, Y - Cursor positioning
13. FIELD / LSET / RSET - Random file access
14. PEEK(A) / POKE(A, V) - Memory operations

### Tier 3: Low Priority (Advanced/Not Applicable)
15. MERGE - File merging
16. CALL - Machine code execution (N/A on embedded)
17. LPRINT - Printer output (N/A on Pico)
18. SYSTEM - Exit to OS (N/A on embedded)

---

## Legend
- ✅ = Implemented and working
- ❌ = Not implemented
- 🔄 = Partially implemented
- ⚠️ = Known issues
