# OBI-88 BASIC for Raspberry Pi Pico 2

A complete BASIC interpreter for the Raspberry Pi Pico 2 (RP2350) with filesystem support. OBI-88 stands for Optimized BASIC Interpreter for embedded systems.

## Features

### BASIC Commands (11 commands)
- **PRINT** - Output text and variables (quoted strings and numeric expressions)
- **LET** - Variable assignment (numeric and string variables)
- **INPUT** - User input with TRS-80 compatible validation (re-prompts on empty input)
- **IF/THEN** - Conditional execution with >, <, =, >= (numeric and string comparisons)
- **FOR/NEXT** - Counted loops (1 to N, with proper nesting)
- **WHILE/WEND** - Conditional loops with dynamic condition evaluation
- **REM** - Comments (line numbers can be skipped after REM)
- **LIST** - Display entire program with line numbers
- **RUN** - Execute program from line 1
- **NEW** - Clear program and variables
- **GOTO** - Jump to line number (basic support)

### Variables (50 capacity)
- **Numeric variables** - Single letter (a, b, x, y, i, etc.) or up to 26 total
- **String variables** - Single letter with $ suffix (name$, city$, etc.)
- Automatic type detection
- Preserved through SAVE/LOAD cycles

### Filesystem Commands (10 commands)
- **SAVE "filename"** - Save program to flash storage
- **LOAD "filename"** - Load program from flash storage
- **DIR** - List files and directories in current path
- **CD "path"** - Change to directory (supports ".." for parent)
- **PWD** - Print working directory path
- **MKDIR "name"** - Create new directory
- **RMDIR "name"** - Remove empty directory
- **RM "file"** - Delete file
- **DRIVES** - Show available drives (0: = internal flash)
- **FORMAT "drive:" YES** - Format drive (safety prompt required)

### Storage & Filesystem
- **1MB flash partition** (drive 0:) with hierarchical directory structure
- **Dynamic allocation** - Files use only space needed (rounded to 4KB sectors)
- **64 file/directory limit** per filesystem
- **32KB max file size**
- **Quoted strings preserved** through save/load operations
- **Ready for SD card** expansion (drive 1:) - syntax already supports it

### Program Capabilities
- **100 program lines** maximum
- **256 characters per line**
- **10-level loop nesting** (FOR/WHILE combined)
- **Line numbers required** (1-9999)
- **Comments support** - REM ignores rest of line
- **Persistent programs** - SAVE/LOAD to flash

### Input/Output
- **USB serial connection** - Interactive REPL
- **Auto-detection** - Waits for terminal connection at startup
- **TRS-80 compatible** - INPUT re-prompts on empty input with "?SN ERROR"
- **Line buffering** - Up to 256 characters per command

### Loop Features
- **FOR loops** - Proper 1-based iteration with NEXT
- **WHILE loops** - Condition re-evaluated on each WEND
- **Loop nesting** - Up to 10 levels deep
- **Break semantics** - Correct jump targets for nested loops

### Comparison Operators (4 operators)
- **>** (greater than) - Works for numbers and strings
- **<** (less than) - Works for numbers and strings
- **=** (equal) - Works for numbers and strings
- **>=** (greater than or equal) - Numeric only
- *Note: <= and <> could be added if needed*

### Data Types
- **Numbers** - 32-bit integers (range -2147483648 to 2147483647)
- **Strings** - Up to variable storage space (typically hundreds of characters)

## Hardware Requirements
- **Raspberry Pi Pico 2** (RP2350-arm-s)
- **USB connection** for serial I/O and programming

## Building

```bash
cd /home/rob/pico/tinybasic
mkdir -p build
cd build
cmake ..
make -j$(nproc)
```

Flash `obi88basic.uf2` to your Pico 2:
- Hold BOOTSEL while plugging in USB
- Copy `build/obi88basic.uf2` to the mounted drive

## Usage Examples

### Hello World
```basic
PRINT "Hello from OBI-88 BASIC!"
```

### Variables and Arithmetic
```basic
10 LET x=42
20 LET y=8
30 PRINT x
40 PRINT y
50 RUN
```

### Conditionals
```basic
10 LET age=18
20 IF age>=18 THEN PRINT "Adult"
30 IF age<18 THEN PRINT "Minor"
40 RUN
```

### For Loops
```basic
10 PRINT "Counting to 10:"
20 FOR i=1 TO 10
30 PRINT i
40 NEXT
50 RUN
```

### While Loops
```basic
10 LET x=5
20 WHILE x>0
30 PRINT x
40 LET x=x-1
50 WEND
60 PRINT "Blastoff!"
70 RUN
```

### User Input
```basic
10 INPUT "What is your name?"; name$
20 PRINT "Hello, "
30 PRINT name$
40 RUN
```

### Nested Loops
```basic
10 FOR i=1 TO 3
20 FOR j=1 TO 3
30 PRINT i
40 PRINT " "
50 PRINT j
60 PRINT " "
70 NEXT
80 NEXT
90 RUN
```

### File Operations
```basic
10 PRINT "Hello World!"
20 SAVE "hello.bas"
30 LIST
```

Then in new session:
```basic
LOAD "hello.bas"
RUN
```

### Directory Management
```basic
MKDIR "projects"
CD "projects"
PWD
SAVE "game.bas"
DIR
CD ".."
```

## Limits Summary
| Item | Limit |
|------|-------|
| Program lines | 100 |
| Line length | 256 chars |
| Variables | 50 |
| Loop nesting | 10 levels |
| File size | 32 KB |
| Files/dirs | 64 per filesystem |
| Storage | 1 MB (flash) |

## Technical Details
- **Language**: C (ARM cross-compiler)
- **Platform**: Raspberry Pi Pico 2 with Pico SDK v2.2.0
- **Build system**: CMake
- **I/O**: USB serial via stdio
- **Storage**: Internal flash with dynamic allocation
- **Memory safety**: Static buffers for flash operations

## Debugging / Common Issues

### Program won't execute
- Use `LIST` to verify program was entered correctly
- Check that line numbers are sequential (1, 10, 20, etc.)
- Variables must be assigned before use in expressions

### INPUT keeps asking again
- OBI-88 uses TRS-80 compatible INPUT validation
- Empty input (just pressing Enter) causes "?SN ERROR" and re-prompt
- Enter a value or press Ctrl+C to cancel

### File operations fail
- Use `DIR` to list current files
- Check drive with `DRIVES`
- Ensure filename is in quotes: `SAVE "file.bas"`
- Use `FORMAT "0:" YES` to reset filesystem if corrupted

## Future Enhancements
- SD card support (drive 1:)
- GOTO/GOSUB subroutines
- String functions (LEN, LEFT$, RIGHT$, MID$)
- Array support
- STEP support in FOR loops
- More operators (%, AND, OR)

## License
MIT License
