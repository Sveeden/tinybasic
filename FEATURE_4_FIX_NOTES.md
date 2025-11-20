# Feature #4: PRINT Command - Float Fix

## Bug Found
When user typed `PRINT 3.14159`, the interpreter returned: `?UNDEFINED VARIABLE: 3.14159`

## Root Cause
The `is_number()` function in `execute.c` was using `strtol()` which only parses integers. When it encountered a decimal point, it would stop parsing, causing the function to reject the float literal.

## Fix Applied
Changed `is_number()` function (lines 30-37 in execute.c):

**BEFORE:**
```c
static int is_number(const char *str) {
    if (!str || *str == '\0') return 0;
    char *endptr;
    strtol(str, &endptr, 10);
    return *endptr == '\0';
}
```

**AFTER:**
```c
static int is_number(const char *str) {
    if (!str || *str == '\0') return 0;
    char *endptr;
    strtof(str, &endptr);
    // Accept if we parsed something and reached end of string
    // Also handle cases with just a sign or decimal point
    return *endptr == '\0' && endptr != str;
}
```

## Changes Made
1. `strtol()` → `strtof()` - Now handles floating-point notation
2. Added `endptr != str` check - Ensures at least one character was successfully parsed

## Test Cases to Verify
```
PRINT 3.14159
Output: 3.14159

PRINT 2.71828  
Output: 2.71828

PRINT 0.5
Output: 0.5

PRINT 100
Output: 100

PRINT 42
Output: 42
```

## Build Status
✓ Build successful: `[100%] Built target obi88basic`

## Next Steps
1. Flash the new obi88basic.uf2 to the Pi Pico
2. Test the PRINT command with floating-point numbers
3. Confirm Feature #4 is now fully working
4. Move to Feature #5 testing
