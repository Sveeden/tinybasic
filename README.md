# OBI-88 BASIC for Raspberry Pi Pico 2

A complete BASIC interpreter for the Raspberry Pi Pico 2 (RP2350) with filesystem support. OBI-88 stands for Optimized BASIC Interpreter for embedded systems.

## Features

### BASIC Commands
- **PRINT** - Output text and variables
- **LET** - Variable assignment
- **INPUT** - User input with validation
- **IF/THEN** - Conditional execution
- **FOR/NEXT** - Counted loops
- **WHILE/WEND** - Conditional loops
- **REM** - Comments
- **LIST** - Display program
- **RUN** - Execute program
- **NEW** - Clear program

### Variables
- Numeric variables (single letter: x, y, i)
- String variables (with $ suffix: name$, x$)
- 50 variable capacity
- Automatic type handling

### Filesystem Commands
- **SAVE "file.bas"** - Save program to flash
- **LOAD "file.bas"** - Load program from flash
- **DIR** - List files and directories
- **CD "path/"** - Change directory
- **PWD** - Show current directory
- **MKDIR "name/"** - Create directory
- **RMDIR "name/"** - Remove directory
- **RM "file"** - Delete file
- **DRIVES** - Show available drives
- **FORMAT "0:" YES** - Format drive

### Storage
- 1MB flash filesystem (drive 0:)
- Up to 64 files/directories
- Dynamic file allocation
- Hierarchical directory structure
- Ready for SD card expansion (drive 1:)

### Limits
- 100 program lines
- 256 characters per line
- 32KB max file size
- 10-level loop nesting

## Hardware Requirements
- Raspberry Pi Pico 2 (RP2350)
- USB connection for serial I/O

## Building

```bash
mkdir build
cd build
cmake ..
make -j$(nproc)
```

Flash `tinybasic.uf2` to your Pico 2.

## Usage Examples

### Basic Programming
```basic
10 PRINT "Hello, World!"
20 LET x=42
30 IF x>40 THEN PRINT "x is large"
40 FOR i=1 TO 5
50 PRINT i
60 NEXT
70 INPUT "Enter your name: "; name$
80 PRINT "Hello, "; name$
```

### File Operations
```basic
SAVE "hello.bas"
NEW
LOAD "hello.bas"
LIST
RUN
```

### Directory Management
```basic
MKDIR "projects/"
CD "projects/"
PWD
SAVE "game.bas"
DIR
CD ".."
```

## Technical Details
- USB serial I/O with connection detection
- TRS-80 compatible INPUT behavior
- Flash-based persistent storage
- Quoted string literals preserved
- Empty INPUT validation

## License
MIT License
