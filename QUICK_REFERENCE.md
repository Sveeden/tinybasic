# OBI-88 BASIC Test Suite Quick Reference

## Test Files at a Glance

| File | Tests | Size | Purpose |
|------|-------|------|---------|
| **test.bas** | 14 | 5.3K | Comprehensive full validation |
| **test_loops.bas** | 14 | 4.4K | FOR/NEXT, WHILE/WEND loops |
| **test_variables.bas** | 13 | 4.8K | Numeric & string variables |
| **test_control_flow.bas** | 13 | 4.8K | IF/THEN, GOTO, GOSUB |
| **test_io.bas** | 13 | 3.6K | PRINT, INPUT operations |
| **test_math_functions.bas** | 20 | 7.8K | Math functions, operations |

**Total: 87 tests across 1043 BASIC code lines**

## Quick Test Commands

```basic
REM Load and run each test suite:
LOAD test_loops.bas
RUN

LOAD test_variables.bas
RUN

LOAD test_control_flow.bas
RUN

LOAD test_io.bas
RUN

LOAD test_math_functions.bas
RUN

LOAD test.bas
RUN
```

## Features Tested

### Loops (14 tests)
- ✅ FOR 1 TO N
- ✅ FOR 1 TO N STEP 2
- ✅ FOR N TO 1 STEP -1
- ✅ Nested FOR loops
- ✅ WHILE condition
- ✅ Nested WHILE loops
- ✅ Loop variable persistence

### Variables (13 tests)
- ✅ Numeric variables (LET x=100)
- ✅ String variables (LET x$="text")
- ✅ Variable reassignment
- ✅ Arithmetic: +, -, *, /
- ✅ Multiple variables
- ✅ Type detection

### Control Flow (13 tests)
- ✅ IF condition THEN statement
- ✅ Numeric comparisons (>, <, =, <>)
- ✅ String comparisons
- ✅ GOTO unconditional jump
- ✅ Conditional GOTO
- ✅ GOSUB subroutine calls
- ✅ RETURN from subroutines
- ✅ Nested GOSUB calls
- ✅ Return stack (10 levels)

### Input/Output (13 tests)
- ✅ PRINT string
- ✅ PRINT number
- ✅ PRINT multiple items
- ✅ PRINT with semicolon (no newline)
- ✅ PRINT empty lines
- ✅ INPUT number
- ✅ INPUT string
- ✅ INPUT in loops

### Math Functions (20 tests)
- ✅ ABS(x) - absolute value
- ✅ SGN(x) - sign (-1, 0, 1)
- ✅ INT(x) - integer truncation
- ✅ SQR(x) - square root
- ✅ ROUND(x) - rounding
- ✅ SIN(x), COS(x), TAN(x) - trigonometry
- ✅ ATN(x) - arctangent
- ✅ LOG(x) - natural logarithm
- ✅ EXP(x) - exponential
- ✅ POWER(x,y) - power function
- ✅ MOD(x,y) - modulo
- ✅ MAX(x,y), MIN(x,y) - min/max
- ✅ PI constant
- ✅ Order of operations (PEMDAS)
- ✅ Nested function calls

## Test Results Expected

All tests should display:
```
OBI-88 BASIC - [TEST TYPE] TEST
============================

Press Enter after reading each test result

TEST 1: [Description]
[Test output]
Result: PASS
Press Enter for next test: _
```

Final message:
```
[TEST TYPE] TEST SUITE COMPLETE!
All [N] tests passed successfully
```

## Running Full Validation (6 minutes total)

```bash
# Sequential execution
LOAD test_loops.bas          # 2 min - 14 tests
RUN

LOAD test_variables.bas      # 1.5 min - 13 tests
RUN

LOAD test_control_flow.bas   # 1.5 min - 13 tests
RUN

LOAD test_io.bas             # 2 min - 13 tests (includes user input)
RUN

LOAD test_math_functions.bas # 1.5 min - 20 tests
RUN

LOAD test.bas                # 1.5 min - 14 tests
RUN
```

## Test Execution Guide

### Per-Test Prompts
- Each test displays output and expected results
- Press ENTER to continue to next test
- Type any character and ENTER to skip ahead
- Watch for "Result: PASS" confirmation

### INPUT Tests
Some tests require user input:
```basic
REM When prompted:
Enter a number: _        REM Type any number and press ENTER
Enter your name: _       REM Type any text and press ENTER
```

### Expected Outputs
Each test includes expected values:
```
ABS(-5) = 5 (expected 5)
MAX(10,20) = 20 (expected 20)
2+3*4 = 14 (expected 14)
```

## Troubleshooting Failures

### If a test fails:
1. Note the **test number** and **feature**
2. Check the **expected value** in output
3. Verify the **actual output**
4. Compare with feature documentation
5. Check implementation in C code

### Common Issues:
- **PASS not displayed** - Feature may not be implemented
- **Wrong value** - Logic error in implementation
- **Program stops** - Likely an END statement or crash
- **No prompt** - INPUT may be missing or broken

## Performance Notes

- **Small tests (loops, variables):** ~2 min including input waits
- **Complex tests (math, control flow):** ~1.5 min
- **Full suite:** ~8-10 minutes with all user interactions

## Coverage Statistics

- **Statements tested:** PRINT, LET, INPUT, FOR, NEXT, WHILE, WEND, IF, THEN, GOTO, GOSUB, RETURN, END
- **Operations tested:** +, -, *, /, comparisons, function calls
- **Functions tested:** 15 mathematical functions + operators
- **Test scenarios:** 87 distinct test cases
- **Code lines:** 1043 lines of BASIC code

---
For detailed test breakdown, see **TEST_SUITE.md**
