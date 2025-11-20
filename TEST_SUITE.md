# OBI-88 BASIC - Test Suite Documentation

This directory contains comprehensive test files for validating all features of the OBI-88 BASIC interpreter running on Raspberry Pi Pico 2.

## Test Files Overview

### 1. **test.bas** - Comprehensive Full Test Suite
- **14 comprehensive tests** covering all major features
- Original test suite that validates all functionality
- Good for quick validation of overall system
- ~190 lines
- **Tests:**
  - TEST 1: Basic output (PRINT)
  - TEST 2: Numeric variables
  - TEST 3: String variables
  - TEST 4: Numeric comparisons (IF/THEN)
  - TEST 5: String comparisons
  - TEST 6: FOR loops
  - TEST 7: Nested FOR loops
  - TEST 8: WHILE loops
  - TEST 9: Mixed output
  - TEST 10: IF/THEN logic
  - TEST 11: Variable capacity
  - TEST 12: Program features
  - TEST 13: GOSUB/RETURN subroutines
  - TEST 14: GOTO statement

### 2. **test_loops.bas** - Loop Operations
- **14 dedicated tests** for loop functionality
- Covers FOR/NEXT and WHILE/WEND loops
- Tests loop variants, nested loops, and variable behavior
- ~170 lines
- **Tests:**
  - TEST 1: Simple FOR loop (1 to 5)
  - TEST 2: FOR loop with STEP
  - TEST 3: FOR loop descending (10 to 1, step -1)
  - TEST 4: Nested FOR loops (3x3 grid)
  - TEST 5: Triple nested loops
  - TEST 6: FOR loop with conditions
  - TEST 7: Simple WHILE loop
  - TEST 8: WHILE loop countdown
  - TEST 9: WHILE with conditions
  - TEST 10: WHILE with iteration counting
  - TEST 11: Nested WHILE loops
  - TEST 12: Loop variable persistence
  - TEST 13: FOR loop with variable bounds
  - TEST 14: WHILE with arithmetic (sum)

### 3. **test_variables.bas** - Variables & Strings
- **13 dedicated tests** for variable operations
- Tests numeric and string variables
- Tests arithmetic operations and expressions
- Tests variable reassignment and scope
- ~155 lines
- **Tests:**
  - TEST 1: Numeric variables
  - TEST 2: String variables
  - TEST 3: Variable reassignment
  - TEST 4: String reassignment
  - TEST 5: Numeric arithmetic (+, -, *, /)
  - TEST 6: Variables from arithmetic expressions
  - TEST 7: Numeric vs string output
  - TEST 8: Multiple variables
  - TEST 9: String concatenation in output
  - TEST 10: Zero and negative values
  - TEST 11: Variables in expressions
  - TEST 12: String and loop variables
  - TEST 13: Mixed variables in same scope

### 4. **test_control_flow.bas** - Control Flow
- **13 dedicated tests** for control flow statements
- Tests IF/THEN, GOTO, and GOSUB/RETURN
- Tests conditional logic and subroutine handling
- Tests nested subroutines and multiple calls
- ~160 lines
- **Tests:**
  - TEST 1: Simple IF/THEN
  - TEST 2: Numeric comparisons
  - TEST 3: String comparisons
  - TEST 4: Multiple conditions
  - TEST 5: IF/THEN with variables
  - TEST 6: GOTO unconditional jump
  - TEST 7: GOTO with condition
  - TEST 8: Simple GOSUB/RETURN
  - TEST 9: GOSUB with logic
  - TEST 10: Nested GOSUB calls
  - TEST 11: Multiple GOSUB calls
  - TEST 12: GOTO as loop
  - TEST 13: Complex conditions

### 5. **test_io.bas** - Input/Output
- **13 dedicated tests** for I/O operations
- Tests PRINT with various data types
- Tests INPUT for numbers and strings
- Tests semicolon suppression (no newline)
- ~120 lines
- **Tests:**
  - TEST 1: Basic PRINT
  - TEST 2: PRINT numbers
  - TEST 3: PRINT strings
  - TEST 4: PRINT multiple items
  - TEST 5: PRINT with semicolon (no newline)
  - TEST 6: PRINT empty lines
  - TEST 7: INPUT number
  - TEST 8: INPUT string
  - TEST 9: INPUT with arithmetic
  - TEST 10: INPUT and expressions
  - TEST 11: PRINT stored variables
  - TEST 12: Output formatting
  - TEST 13: INPUT in loops

### 6. **test_math_functions.bas** - Math Functions & Order of Operations
- **20 comprehensive tests** for mathematical functions
- Tests all 15 math functions with proper operation order
- Tests nested functions and complex expressions
- ~230 lines
- **Tests:**
  - TEST 1-11: Individual math functions
    - ABS, SGN, INT, SQR, ROUND
    - SIN, COS, TAN, ATN, LOG, EXP
  - TEST 12-15: Multi-argument functions
    - POWER, MOD, MAX, MIN
  - TEST 16: PI constant
  - TEST 17: Basic order of operations (2+3*4 = 14)
  - TEST 18: Complex order of operations
  - TEST 19: Functions with mixed operations
  - TEST 20: Nested function calls

**Supported Math Functions:**
- **Single-argument:** ABS, SGN, INT, SQR, SIN, COS, TAN, ATN, LOG, EXP, ROUND
- **Multi-argument:** MAX, MIN, MOD, POWER
- **Constants:** PI

### 7. **test_math_functions_dedicated.bas** (Alias: test_math.bas)
- Quick math function validation
- Minimal tests for rapid checking

## How to Use

### Loading Tests onto Pico 2

1. **Copy test file to Pico filesystem:**
   ```bash
   # Mount Pico in mass storage mode (hold BOOTSEL while plugging in)
   cp test_loops.bas /mnt/pico/
   ```

2. **Or use the OBI-88 commands:**
   ```basic
   LOAD test_loops.bas
   ```

### Running Tests

1. **Boot OBI-88 on Pico 2**
2. **Load desired test:**
   ```basic
   LOAD test_loops.bas
   ```
3. **Run test suite:**
   ```basic
   RUN
   ```
4. **Follow prompts** - Press Enter after each test to continue
5. **Review output** - Each test prints "Result: PASS" if successful

## Test Results Expected

### All Tests Should Show:
- Clear test names and descriptions
- Input prompts between tests
- Expected values for verification
- "Result: PASS" for successful tests
- Final summary at end of suite

### Features Validated:

✅ **Loops:** FOR/NEXT (with STEP), WHILE/WEND, nested loops
✅ **Variables:** Numeric, string, reassignment, scope
✅ **Control Flow:** IF/THEN, GOTO, GOSUB/RETURN, subroutines
✅ **I/O:** PRINT (with semicolon), INPUT
✅ **Math:** 15 functions, order of operations, nested functions
✅ **Expressions:** Arithmetic, function composition, variable references

## Running All Tests

**Complete validation sequence:**
```basic
RUN test.bas                    REM Comprehensive full suite
RUN test_loops.bas              REM Loop operations
RUN test_variables.bas          REM Variables and strings
RUN test_control_flow.bas       REM Control flow
RUN test_io.bas                 REM Input/output
RUN test_math_functions.bas     REM Math and operations
```

## Test Infrastructure

Each test file includes:
- **Clear headers** with test suite name
- **Numbered sections** (TEST 1, TEST 2, etc.)
- **User prompts** between tests (INPUT statements)
- **Expected values** for verification
- **PASS/FAIL indicators** for each test
- **Summary section** at end

## Architecture Notes

- Tests use sequential line numbering (10-line increments)
- Each test suite is independent and can run standalone
- Tests validate both functionality and expected output
- User input prompts allow manual verification
- All tests are deterministic and reproducible

## Troubleshooting

If a test fails:
1. Note which test number failed
2. Check the expected vs actual output
3. Review test code for the failing test
4. Verify the implemented feature in the main codebase

## Future Enhancements

Potential additional test files:
- `test_arrays.bas` - Array operations (if/when implemented)
- `test_strings.bas` - String functions (MID$, LEFT$, RIGHT$, etc.)
- `test_io_advanced.bas` - File operations (SAVE, LOAD, DIR)
- `test_edge_cases.bas` - Boundary conditions and error handling
