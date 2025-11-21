# OBI-88 BASIC Test Suite

All test files are organized in this `tests/` folder.

## Individual Feature Tests (New Functions)

### Core Features Added This Session
- **test_auto.bas** - AUTO command (automatic line numbering)
- **test_renum.bas** - RENUM command (renumber program with reference updates)
- **test_snippet.bas** - SNIPPET command (insert code examples)
- **test_edit.bas** - EDIT command (edit program lines with arrow keys)
- **test_rnd.bas** - RND() function and RANDOMIZE command

## Comprehensive Test Suites

### Program Execution & Features
- **test.bas** - Full comprehensive test suite (14 tests covering all major features)

### Basic Features (Individual Categories)
- **test_variables.bas** - Variable assignment and manipulation
- **test_loops.bas** - FOR/NEXT and WHILE/WEND loop tests
- **test_control_flow.bas** - IF/THEN, GOTO, GOSUB/RETURN tests
- **test_io.bas** - PRINT and INPUT tests
- **test_math_functions.bas** - Math function tests (ABS, SQR, SIN, COS, etc.)

### Quick Tests
- **test_math.bas** - Quick math function test
- **test_ctrlc.bas** - Ctrl+C interrupt handling test
- **test_help.bas** - Help system test
- **test_print_expr.bas** - Print with expressions test
- **test_feature_5_input.bas** - Input validation test

## How to Use

### Load and Run a Test
```
> LOAD tests/test_auto.bas
> RUN
```

### Run Multiple Tests in Sequence
```
> LOAD tests/test_variables.bas
> RUN
> (Press Enter after each test)

> LOAD tests/test_loops.bas
> RUN

> LOAD tests/test_control_flow.bas
> RUN
```

### Quick Feature Verification
1. **AUTO** - `LOAD tests/test_auto.bas` and `RUN`
2. **RENUM** - `LOAD tests/test_renum.bas` and `RUN`
3. **SNIPPET** - `LOAD tests/test_snippet.bas` and `RUN`
4. **EDIT** - `LOAD tests/test_edit.bas` and `RUN`
5. **RND** - `LOAD tests/test_rnd.bas` and `RUN`

### Full Validation
Run `test.bas` for comprehensive validation of all features:
```
> LOAD tests/test.bas
> RUN
```

## Test Coverage

| Feature | Test File | Status |
|---------|-----------|--------|
| AUTO | test_auto.bas | ✅ |
| RENUM | test_renum.bas | ✅ |
| SNIPPET | test_snippet.bas | ✅ |
| EDIT | test_edit.bas | ✅ |
| RND/RANDOMIZE | test_rnd.bas | ✅ |
| Variables | test_variables.bas | ✅ |
| FOR/WHILE loops | test_loops.bas | ✅ |
| IF/GOTO/GOSUB | test_control_flow.bas | ✅ |
| PRINT/INPUT | test_io.bas | ✅ |
| Math Functions | test_math_functions.bas | ✅ |
| Comprehensive | test.bas | ✅ |

## File Organization

```
/tests/
├── test.bas                    # Master comprehensive test
├── test_auto.bas              # NEW: AUTO command
├── test_renum.bas             # NEW: RENUM command
├── test_snippet.bas           # NEW: SNIPPET command
├── test_edit.bas              # NEW: EDIT command
├── test_rnd.bas               # NEW: RND/RANDOMIZE
├── test_variables.bas         # Basic: Variables
├── test_loops.bas             # Basic: Loops
├── test_control_flow.bas      # Basic: Control flow
├── test_io.bas                # Basic: I/O
├── test_math_functions.bas    # Basic: Math
├── test_math.bas              # Quick: Math
├── test_ctrlc.bas             # Quick: Ctrl+C
├── test_help.bas              # Quick: Help
├── test_print_expr.bas        # Quick: Print expressions
└── test_feature_5_input.bas   # Quick: Input validation
```

## Notes

- Each test displays results with PASS/FAIL indicators
- Tests prompt for Enter to continue between sections
- Press Ctrl+C to exit any test at any INPUT prompt
- Tests are designed to be interactive with user feedback
- See individual test files for specific feature documentation
