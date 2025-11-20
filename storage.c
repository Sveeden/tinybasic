#include "storage.h"
#include "program.h"
#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/flash.h"
#include "hardware/sync.h"

// Reserve 1MB at the end of the 4MB flash for program storage
// Flash is at XIP_BASE (0x10000000)
// Pico 2 has 4MB = 0x400000 bytes
// Reserve last 1MB = 0x100000 bytes
// Storage starts at offset 3MB = 0x300000
#define FLASH_TARGET_OFFSET (3 * 1024 * 1024)  // 3MB offset (last 1MB reserved)
#define FLASH_STORAGE_SIZE (1 * 1024 * 1024)   // 1MB storage area
#define MAGIC_NUMBER 0x42415349  // "BASI" in hex

// Structure for saved program header
typedef struct {
    uint32_t magic;        // Magic number to verify valid data
    uint32_t program_size; // Size of program data in bytes
    uint32_t checksum;     // Simple checksum for validation
} StorageHeader;

void storage_init(void) {
    // Nothing to initialize - flash is already available
}

int storage_exists(void) {
    const uint8_t *flash_ptr = (const uint8_t *)(XIP_BASE + FLASH_TARGET_OFFSET);
    const StorageHeader *header = (const StorageHeader *)flash_ptr;
    
    return (header->magic == MAGIC_NUMBER);
}

int storage_save(void) {
    // Build program data to save
    // Format: series of "line_num:command\n" strings
    // Max program size: 100 lines * 256 chars/line = 25,600 bytes
    static char program_data[32768];  // 32KB buffer (static to keep off stack)
    int offset = 0;
    
    int line_num = prog_first_line();
    while (line_num >= 0) {
        const char *line_text = prog_get_line(line_num);
        if (line_text) {
            int written = snprintf(program_data + offset, 
                                  sizeof(program_data) - offset,
                                  "%d %s\n", line_num, line_text);
            if (written < 0 || offset + written >= sizeof(program_data)) {
                printf("?PROGRAM TOO LARGE\n");
                return -1;
            }
            offset += written;
        }
        line_num = prog_next_line(line_num);
    }
    program_data[offset] = '\0';
    
    // Calculate checksum (simple sum of bytes)
    uint32_t checksum = 0;
    for (int i = 0; i < offset; i++) {
        checksum += (uint8_t)program_data[i];
    }
    
    // Build header
    StorageHeader header;
    header.magic = MAGIC_NUMBER;
    header.program_size = offset;
    header.checksum = checksum;
    
    // Calculate total size to write (header + program data)
    uint32_t total_size = sizeof(StorageHeader) + offset;
    
    // Round up to sector size
    uint32_t sectors_needed = (total_size + FLASH_SECTOR_SIZE - 1) / FLASH_SECTOR_SIZE;
    uint32_t erase_size = sectors_needed * FLASH_SECTOR_SIZE;
    
    // Prepare write buffer (must be FLASH_PAGE_SIZE aligned)
    // Allocate enough space for header + data, rounded to page size
    uint32_t write_size = (total_size + FLASH_PAGE_SIZE - 1) & ~(FLASH_PAGE_SIZE - 1);
    static uint8_t write_buffer[32768];  // 32KB buffer (static to keep off stack)
    
    if (write_size > sizeof(write_buffer)) {
        printf("?PROGRAM TOO LARGE FOR FLASH\n");
        return -1;
    }
    
    memset(write_buffer, 0xFF, write_size);
    
    // Copy header and program data to write buffer
    memcpy(write_buffer, &header, sizeof(StorageHeader));
    memcpy(write_buffer + sizeof(StorageHeader), program_data, offset);
    
    // Disable interrupts during flash write
    uint32_t ints = save_and_disable_interrupts();
    
    // Erase the flash sectors
    flash_range_erase(FLASH_TARGET_OFFSET, erase_size);
    
    // Write the data
    flash_range_program(FLASH_TARGET_OFFSET, write_buffer, write_size);
    
    // Re-enable interrupts
    restore_interrupts(ints);
    
    printf("Program saved (%d bytes)\n", offset);
    return 0;
}

int storage_load(void) {
    // Get pointer to flash memory
    const uint8_t *flash_ptr = (const uint8_t *)(XIP_BASE + FLASH_TARGET_OFFSET);
    const StorageHeader *header = (const StorageHeader *)flash_ptr;
    
    // Check magic number
    if (header->magic != MAGIC_NUMBER) {
        printf("?NO SAVED PROGRAM\n");
        return -1;
    }
    
    // Get program data
    const char *program_data = (const char *)(flash_ptr + sizeof(StorageHeader));
    
    // Validate checksum
    uint32_t checksum = 0;
    for (uint32_t i = 0; i < header->program_size; i++) {
        checksum += (uint8_t)program_data[i];
    }
    
    if (checksum != header->checksum) {
        printf("?CORRUPTED PROGRAM\n");
        return -1;
    }
    
    // Clear current program
    prog_clear();
    
    // Parse and load lines
    char line_buffer[256];
    int buf_pos = 0;
    
    for (uint32_t i = 0; i < header->program_size; i++) {
        if (program_data[i] == '\n' || program_data[i] == '\0') {
            line_buffer[buf_pos] = '\0';
            if (buf_pos > 0) {
                prog_store_line(line_buffer);
            }
            buf_pos = 0;
        } else if (buf_pos < 255) {
            line_buffer[buf_pos++] = program_data[i];
        }
    }
    
    // Handle last line if no trailing newline
    if (buf_pos > 0) {
        line_buffer[buf_pos] = '\0';
        prog_store_line(line_buffer);
    }
    
    printf("Program loaded (%d bytes)\n", header->program_size);
    return 0;
}
