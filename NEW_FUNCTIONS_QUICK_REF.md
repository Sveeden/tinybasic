# New Functions - Quick Reference

## AUTO Command
**Automatic line numbering**

```basic
AUTO [start_line] [step]

Examples:
AUTO              REM Start at 10, step 10
AUTO 100          REM Start at 100, step 10
AUTO 50 5         REM Start at 50, step 5
```

**Test:** `LOAD tests/test_auto.bas` then `RUN`

---

## RENUM Command
**Renumber program lines**

```basic
RENUM [step]

Examples:
RENUM             REM Renumber with step 10
RENUM 5           REM Renumber with step 5
RENUM 20          REM Renumber with step 20
```

**Features:**
- Automatically updates GOTO and GOSUB references
- Maintains program logic
- Assigns new starting line 10

**Test:** `LOAD tests/test_renum.bas` then `RUN`

---

## SNIPPET Command
**Insert code examples**

```basic
SNIPPET command_name

Examples:
SNIPPET PRINT     REM Insert PRINT example
SNIPPET FOR       REM Insert FOR example
SNIPPET IF        REM Insert IF example
SNIPPET WHILE     REM Insert WHILE example
```

**Features:**
- Adds working code examples to program
- Auto-numbers new lines
- Many built-in examples available

**Test:** `LOAD tests/test_snippet.bas` then `RUN`

---

## EDIT Command
**Edit existing program line**

```basic
EDIT line_number

Examples:
EDIT 10           REM Edit line 10
EDIT 100          REM Edit line 100
EDIT 1            REM Edit line 1
```

**Features:**
- Pre-loads line into editor
- Cursor ready for immediate editing
- Arrow keys for navigation
- Backspace for deletion
- Enter to save, Ctrl+C to cancel

**Controls:**
- Left/Right arrows - Move cursor
- Backspace/Delete - Remove character
- Type - Insert character at cursor
- Enter - Save changes
- Ctrl+C - Cancel edit

**Test:** `LOAD tests/test_edit.bas` then `RUN`

---

## RND() Function
**Generate random number**

```basic
RND(max)

Examples:
PRINT RND(100)    REM Random 1-100
PRINT RND(10)     REM Random 1-10 (dice)
PRINT RND(1000)   REM Random 1-1000

FOR i=1 TO 6
  PRINT RND(20)   REM 6 random 1-20
NEXT
```

**Features:**
- Returns integer from 1 to max (inclusive)
- 1-based (traditional BASIC style)
- Uses seeded random generator

**Test:** `LOAD tests/test_rnd.bas` then `RUN`

---

## RANDOMIZE Command
**Seed random number generator**

```basic
RANDOMIZE [seed]

Examples:
RANDOMIZE         REM Seed from timer (random)
RANDOMIZE 42      REM Seed with 42 (reproducible)
RANDOMIZE 1       REM Seed with 1
```

**Features:**
- With seed: Reproducible sequences
- Without seed: Uses timer for randomness
- Required before RND for controlled sequences

**Example - Reproducible:**
```basic
10 RANDOMIZE 100
20 FOR i=1 TO 3
30   PRINT RND(100)
40 NEXT
50 RANDOMIZE 100    REM Reset seed
60 FOR i=1 TO 3
70   PRINT RND(100) REM Same numbers again
80 NEXT
```

**Example - Random:**
```basic
10 RANDOMIZE        REM Seed from timer
20 PRINT RND(100)
30 RANDOMIZE        REM Different seed
40 PRINT RND(100)   REM Likely different
```

**Test:** `LOAD tests/test_rnd.bas` then `RUN`

---

## Testing All New Features

```basic
REM Load comprehensive test
LOAD tests/test.bas
RUN

REM Or test individually
LOAD tests/test_auto.bas
RUN

LOAD tests/test_renum.bas
RUN

LOAD tests/test_snippet.bas
RUN

LOAD tests/test_edit.bas
RUN

LOAD tests/test_rnd.bas
RUN
```

---

## Summary

| Command | Purpose | Example |
|---------|---------|---------|
| AUTO | Line numbering | AUTO 100 |
| RENUM | Renumber lines | RENUM 5 |
| SNIPPET | Add code examples | SNIPPET PRINT |
| EDIT | Edit line | EDIT 10 |
| RND(n) | Random number | RND(100) |
| RANDOMIZE | Seed RNG | RANDOMIZE 42 |

All features are fully implemented and tested!
