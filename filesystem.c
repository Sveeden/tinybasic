#include "filesystem.h"
#include "program.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/flash.h"
#include "hardware/sync.h"

// Simple filesystem implementation for flash storage
// Drive 0: = 1MB flash partition starting at 3MB offset
// Future: Drive 1: = SD card

#define FLASH_OFFSET_DRIVE0 (3 * 1024 * 1024)  // 3MB offset
#define FLASH_SIZE_DRIVE0 (1 * 1024 * 1024)    // 1MB size
#define MAX_FILENAME 64
#define MAX_PATH 128
#define MAX_FILES 64
#define MAX_FILE_SIZE 32768

// File entry structure
typedef struct {
    char name[MAX_FILENAME];
    uint8_t is_directory;
    uint32_t size;
    uint32_t offset;  // Offset within drive's data area
} FileEntry;

// Directory structure
typedef struct {
    char path[MAX_PATH];
    FileEntry files[MAX_FILES];
    uint16_t file_count;
} Directory;

// Filesystem header (stored at start of flash partition)
typedef struct {
    uint32_t magic;           // 0x46535953 "FSYS"
    uint8_t formatted;
    uint8_t current_drive;
    char current_path[MAX_PATH];
    Directory root;
    uint32_t next_data_offset;  // Next free space for file data
} FSHeader;

static FSHeader fs_header;
static uint8_t fs_mounted = 0;

// Helper: Get flash pointer for drive 0
static const uint8_t* get_flash_ptr(void) {
    return (const uint8_t *)(XIP_BASE + FLASH_OFFSET_DRIVE0);
}

// Helper: Write header to flash
static int write_header(void) {
    fs_header.magic = 0x46535953;  // "FSYS"
    
    uint32_t write_size = (sizeof(FSHeader) + FLASH_PAGE_SIZE - 1) & ~(FLASH_PAGE_SIZE - 1);
    static uint8_t write_buffer[8192];  // Larger buffer for FSHeader
    
    if (sizeof(FSHeader) > sizeof(write_buffer)) {
        printf("?HEADER TOO LARGE\n");
        return -1;
    }
    
    memset(write_buffer, 0xFF, sizeof(write_buffer));
    memcpy(write_buffer, &fs_header, sizeof(FSHeader));
    
    uint32_t ints = save_and_disable_interrupts();
    
    // Erase enough sectors for the header
    uint32_t erase_size = (sizeof(FSHeader) + FLASH_SECTOR_SIZE - 1) & ~(FLASH_SECTOR_SIZE - 1);
    flash_range_erase(FLASH_OFFSET_DRIVE0, erase_size);
    flash_range_program(FLASH_OFFSET_DRIVE0, write_buffer, write_size);
    restore_interrupts(ints);
    
    return 0;
}

// Helper: Read header from flash
static int read_header(void) {
    const uint8_t *flash_ptr = get_flash_ptr();
    memcpy(&fs_header, flash_ptr, sizeof(FSHeader));
    
    if (fs_header.magic != 0x46535953) {
        return -1;  // Not formatted
    }
    return 0;
}

int fs_init(void) {
    if (read_header() != 0) {
        // Not formatted, initialize
        printf("Flash filesystem not formatted. Formatting drive 0:...\n");
        memset(&fs_header, 0, sizeof(FSHeader));
        fs_header.formatted = 1;
        fs_header.current_drive = 0;
        strcpy(fs_header.current_path, "/");
        fs_header.root.file_count = 0;
        strcpy(fs_header.root.path, "/");
        fs_header.next_data_offset = 8192;  // After header (2 sectors)
        write_header();
        printf("Drive 0: formatted\n");
    }
    
    fs_mounted = 1;
    return 0;
}

int fs_mount(uint8_t drive) {
    if (drive != 0) {
        printf("?DRIVE NOT AVAILABLE\n");
        return -1;
    }
    return fs_init();
}

int fs_unmount(uint8_t drive) {
    if (drive == 0) {
        fs_mounted = 0;
    }
    return 0;
}

uint8_t fs_get_drive(void) {
    return fs_header.current_drive;
}

const char* fs_get_path(void) {
    static char full_path[MAX_PATH + 4];
    snprintf(full_path, sizeof(full_path), "%d:%s", fs_header.current_drive, fs_header.current_path);
    return full_path;
}

int fs_cd(const char *path) {
    if (!fs_mounted) {
        printf("?FILESYSTEM NOT MOUNTED\n");
        return -1;
    }
    
    // Check for drive switch: "0:" or "1:"
    if (strlen(path) >= 2 && path[1] == ':') {
        uint8_t drive = path[0] - '0';
        if (drive == 0) {
            fs_header.current_drive = 0;
            strcpy(fs_header.current_path, "/");
            write_header();
            return 0;
        } else if (drive == 1) {
            printf("?SD CARD NOT AVAILABLE YET\n");
            return -1;
        } else {
            printf("?INVALID DRIVE\n");
            return -1;
        }
    }
    
    // Handle relative paths
    if (strcmp(path, "..") == 0) {
        // Go up one directory
        char *last_slash = strrchr(fs_header.current_path, '/');
        if (last_slash != NULL && last_slash != fs_header.current_path) {
            *last_slash = '\0';
        } else {
            strcpy(fs_header.current_path, "/");
        }
        write_header();
        return 0;
    }
    
    // Absolute path
    if (path[0] == '/') {
        strncpy(fs_header.current_path, path, MAX_PATH - 1);
    } else {
        // Relative path - append to current
        if (strcmp(fs_header.current_path, "/") != 0) {
            strncat(fs_header.current_path, "/", MAX_PATH - strlen(fs_header.current_path) - 1);
        }
        strncat(fs_header.current_path, path, MAX_PATH - strlen(fs_header.current_path) - 1);
    }
    
    // Remove trailing slash
    int len = strlen(fs_header.current_path);
    if (len > 1 && fs_header.current_path[len - 1] == '/') {
        fs_header.current_path[len - 1] = '\0';
    }
    
    write_header();
    return 0;
}

int fs_mkdir(const char *path) {
    if (!fs_mounted) return -1;
    
    // Build full path
    char full_path[MAX_PATH];
    if (path[0] == '/') {
        strncpy(full_path, path, MAX_PATH - 1);
    } else {
        if (strcmp(fs_header.current_path, "/") == 0) {
            snprintf(full_path, MAX_PATH, "/%s", path);
        } else {
            snprintf(full_path, MAX_PATH, "%s/%s", fs_header.current_path, path);
        }
    }
    
    // Check if directory already exists
    for (int i = 0; i < fs_header.root.file_count; i++) {
        if (strcmp(fs_header.root.files[i].name, full_path) == 0) {
            printf("?DIRECTORY EXISTS\n");
            return -1;
        }
    }
    
    // Add directory entry
    if (fs_header.root.file_count >= MAX_FILES) {
        printf("?TOO MANY FILES\n");
        return -1;
    }
    
    FileEntry *entry = &fs_header.root.files[fs_header.root.file_count++];
    strncpy(entry->name, full_path, MAX_FILENAME - 1);
    entry->is_directory = 1;
    entry->size = 0;
    entry->offset = 0;
    
    write_header();
    printf("Directory created: %s\n", full_path);
    return 0;
}

int fs_rmdir(const char *path) {
    if (!fs_mounted) return -1;
    
    char full_path[MAX_PATH];
    if (path[0] == '/') {
        strncpy(full_path, path, MAX_PATH - 1);
    } else {
        snprintf(full_path, MAX_PATH, "%s/%s", fs_header.current_path, path);
    }
    
    // Find and remove directory
    for (int i = 0; i < fs_header.root.file_count; i++) {
        if (strcmp(fs_header.root.files[i].name, full_path) == 0) {
            if (!fs_header.root.files[i].is_directory) {
                printf("?NOT A DIRECTORY\n");
                return -1;
            }
            
            // Shift remaining entries
            for (int j = i; j < fs_header.root.file_count - 1; j++) {
                fs_header.root.files[j] = fs_header.root.files[j + 1];
            }
            fs_header.root.file_count--;
            write_header();
            printf("Directory removed\n");
            return 0;
        }
    }
    
    printf("?DIRECTORY NOT FOUND\n");
    return -1;
}

int fs_dir(const char *path) {
    if (!fs_mounted) return -1;
    
    const char *list_path = (path && strlen(path) > 0) ? path : fs_header.current_path;
    
    printf("Directory of %d:%s\n", fs_header.current_drive, list_path);
    printf("----------------------------------------\n");
    
    int count = 0;
    for (int i = 0; i < fs_header.root.file_count; i++) {
        FileEntry *f = &fs_header.root.files[i];
        
        // Check if file/dir is in the target directory
        // For root "/", we want items like "/projects" or "/hello.bas"
        // For "/projects", we want items like "/projects/file.bas"
        
        const char *item_path = f->name;
        int list_path_len = strlen(list_path);
        
        // Check if item starts with list_path
        if (strncmp(item_path, list_path, list_path_len) != 0) {
            continue;  // Not in this directory
        }
        
        // Get the part after list_path
        const char *remainder = item_path + list_path_len;
        
        // Skip leading slash if list_path doesn't end with one
        if (*remainder == '/' && list_path[list_path_len - 1] != '/') {
            remainder++;
        }
        
        // Skip if remainder is empty (it's the directory itself)
        if (strlen(remainder) == 0) {
            continue;
        }
        
        // Check if there's another slash (means it's in a subdirectory)
        const char *slash = strchr(remainder, '/');
        if (slash != NULL) {
            continue;  // In a subdirectory, skip it
        }
        
        // Display the item
        if (f->is_directory) {
            printf("%-40s <DIR>\n", remainder);
        } else {
            printf("%-40s %6d bytes\n", remainder, f->size);
        }
        count++;
    }
    
    printf("----------------------------------------\n");
    printf("%d items\n", count);
    return 0;
}

int fs_save(const char *filename) {
    if (!fs_mounted) return -1;
    
    // Build full path
    char full_path[MAX_PATH];
    if (filename[0] == '/') {
        strncpy(full_path, filename, MAX_PATH - 1);
    } else {
        if (strcmp(fs_header.current_path, "/") == 0) {
            snprintf(full_path, MAX_PATH, "/%s", filename);
        } else {
            snprintf(full_path, MAX_PATH, "%s/%s", fs_header.current_path, filename);
        }
    }
    
    // Build program data
    static char program_data[MAX_FILE_SIZE];
    int offset = 0;
    
    int line_num = prog_first_line();
    while (line_num >= 0) {
        const char *line_text = prog_get_line(line_num);
        if (line_text) {
            int written = snprintf(program_data + offset, 
                                  MAX_FILE_SIZE - offset,
                                  "%d %s\n", line_num, line_text);
            if (written < 0 || offset + written >= MAX_FILE_SIZE) {
                printf("?PROGRAM TOO LARGE\n");
                return -1;
            }
            offset += written;
        }
        line_num = prog_next_line(line_num);
    }
    
    // Find or create file entry
    FileEntry *entry = NULL;
    for (int i = 0; i < fs_header.root.file_count; i++) {
        if (strcmp(fs_header.root.files[i].name, full_path) == 0) {
            entry = &fs_header.root.files[i];
            break;
        }
    }
    
    if (!entry) {
        if (fs_header.root.file_count >= MAX_FILES) {
            printf("?TOO MANY FILES\n");
            return -1;
        }
        entry = &fs_header.root.files[fs_header.root.file_count++];
        strncpy(entry->name, full_path, MAX_FILENAME - 1);
        entry->is_directory = 0;
        entry->offset = fs_header.next_data_offset;
    }
    
    // Update file size and advance next_data_offset
    entry->size = offset;
    
    // Calculate space needed (round up to sector size for flash erase)
    uint32_t space_needed = (offset + FLASH_SECTOR_SIZE - 1) & ~(FLASH_SECTOR_SIZE - 1);
    
    // Only advance next_data_offset if this is a new file
    // For existing files, we reuse the space (may waste if file grew)
    if (entry->offset == fs_header.next_data_offset) {
        fs_header.next_data_offset += space_needed;
    }
    
    // Write file data to flash
    uint32_t write_offset = FLASH_OFFSET_DRIVE0 + entry->offset;
    uint32_t write_size = (offset + FLASH_PAGE_SIZE - 1) & ~(FLASH_PAGE_SIZE - 1);
    uint32_t erase_size = (offset + FLASH_SECTOR_SIZE - 1) & ~(FLASH_SECTOR_SIZE - 1);
    
    static uint8_t write_buffer[MAX_FILE_SIZE];
    memset(write_buffer, 0xFF, MAX_FILE_SIZE);
    memcpy(write_buffer, program_data, offset);
    
    uint32_t ints = save_and_disable_interrupts();
    
    // Erase only the sectors we need
    flash_range_erase(write_offset, erase_size);
    flash_range_program(write_offset, write_buffer, write_size);
    
    restore_interrupts(ints);
    
    // Update header
    write_header();
    
    printf("Saved: %s (%d bytes)\n", full_path, offset);
    return 0;
}

int fs_write_note(const char *filename, const char *text) {
    if (!fs_mounted) return -1;
    
    // Build full path
    char full_path[MAX_PATH];
    if (filename[0] == '/') {
        strncpy(full_path, filename, MAX_PATH - 1);
    } else {
        if (strcmp(fs_header.current_path, "/") == 0) {
            snprintf(full_path, MAX_PATH, "/%s", filename);
        } else {
            snprintf(full_path, MAX_PATH, "%s/%s", fs_header.current_path, filename);
        }
    }
    
    // Build note data (text + newline)
    static char note_data[MAX_FILE_SIZE];
    int offset = snprintf(note_data, MAX_FILE_SIZE, "%s\n", text);
    
    if (offset >= MAX_FILE_SIZE) {
        printf("?NOTE TOO LARGE\n");
        return -1;
    }
    
    // Find or create file entry
    FileEntry *entry = NULL;
    for (int i = 0; i < fs_header.root.file_count; i++) {
        if (strcmp(fs_header.root.files[i].name, full_path) == 0) {
            entry = &fs_header.root.files[i];
            break;
        }
    }
    
    if (!entry) {
        if (fs_header.root.file_count >= MAX_FILES) {
            printf("?TOO MANY FILES\n");
            return -1;
        }
        entry = &fs_header.root.files[fs_header.root.file_count++];
        strncpy(entry->name, full_path, MAX_FILENAME - 1);
        entry->is_directory = 0;
        entry->offset = fs_header.next_data_offset;
    }
    
    // Update file size
    entry->size = offset;
    
    // Calculate space needed
    uint32_t space_needed = (offset + FLASH_SECTOR_SIZE - 1) & ~(FLASH_SECTOR_SIZE - 1);
    
    // Advance next_data_offset if new file
    if (entry->offset == fs_header.next_data_offset) {
        fs_header.next_data_offset += space_needed;
    }
    
    // Write file data to flash
    uint32_t write_offset = FLASH_OFFSET_DRIVE0 + entry->offset;
    uint32_t write_size = (offset + FLASH_PAGE_SIZE - 1) & ~(FLASH_PAGE_SIZE - 1);
    uint32_t erase_size = (offset + FLASH_SECTOR_SIZE - 1) & ~(FLASH_SECTOR_SIZE - 1);
    
    static uint8_t write_buffer[MAX_FILE_SIZE];
    memset(write_buffer, 0xFF, MAX_FILE_SIZE);
    memcpy(write_buffer, note_data, offset);
    
    uint32_t ints = save_and_disable_interrupts();
    
    flash_range_erase(write_offset, erase_size);
    flash_range_program(write_offset, write_buffer, write_size);
    
    restore_interrupts(ints);
    
    // Update header
    write_header();
    
    printf("Note saved: %s (%d bytes)\n", full_path, offset);
    return 0;
}

int fs_load(const char *filename) {
    if (!fs_mounted) return -1;
    
    // Build full path
    char full_path[MAX_PATH];
    if (filename[0] == '/') {
        strncpy(full_path, filename, MAX_PATH - 1);
    } else {
        if (strcmp(fs_header.current_path, "/") == 0) {
            snprintf(full_path, MAX_PATH, "/%s", filename);
        } else {
            snprintf(full_path, MAX_PATH, "%s/%s", fs_header.current_path, filename);
        }
    }
    
    // Find file
    FileEntry *entry = NULL;
    for (int i = 0; i < fs_header.root.file_count; i++) {
        if (strcmp(fs_header.root.files[i].name, full_path) == 0) {
            entry = &fs_header.root.files[i];
            break;
        }
    }
    
    if (!entry) {
        printf("?FILE NOT FOUND: %s\n", full_path);
        return -1;
    }
    
    if (entry->is_directory) {
        printf("?IS A DIRECTORY\n");
        return -1;
    }
    
    // Read file data from flash
    const uint8_t *file_data = get_flash_ptr() + entry->offset;
    
    // Clear current program
    prog_clear();
    
    // Parse and load lines
    char line_buffer[256];
    int buf_pos = 0;
    
    for (uint32_t i = 0; i < entry->size; i++) {
        if (file_data[i] == '\n' || file_data[i] == '\0') {
            line_buffer[buf_pos] = '\0';
            if (buf_pos > 0) {
                prog_store_line(line_buffer);
            }
            buf_pos = 0;
        } else if (buf_pos < 255) {
            line_buffer[buf_pos++] = file_data[i];
        }
    }
    
    if (buf_pos > 0) {
        line_buffer[buf_pos] = '\0';
        prog_store_line(line_buffer);
    }
    
    printf("Loaded: %s (%d bytes)\n", full_path, entry->size);
    return 0;
}

int fs_rm(const char *filename) {
    if (!fs_mounted) return -1;
    
    char full_path[MAX_PATH];
    if (filename[0] == '/') {
        strncpy(full_path, filename, MAX_PATH - 1);
    } else {
        if (strcmp(fs_header.current_path, "/") == 0) {
            snprintf(full_path, MAX_PATH, "/%s", filename);
        } else {
            snprintf(full_path, MAX_PATH, "%s/%s", fs_header.current_path, filename);
        }
    }
    
    // Find and remove file
    for (int i = 0; i < fs_header.root.file_count; i++) {
        if (strcmp(fs_header.root.files[i].name, full_path) == 0) {
            if (fs_header.root.files[i].is_directory) {
                printf("?IS A DIRECTORY (use RMDIR)\n");
                return -1;
            }
            
            // Shift remaining entries
            for (int j = i; j < fs_header.root.file_count - 1; j++) {
                fs_header.root.files[j] = fs_header.root.files[j + 1];
            }
            fs_header.root.file_count--;
            write_header();
            printf("File deleted: %s\n", full_path);
            return 0;
        }
    }
    
    printf("?FILE NOT FOUND\n");
    return -1;
}

int fs_format(uint8_t drive) {
    if (drive != 0) {
        printf("?INVALID DRIVE\n");
        return -1;
    }
    
    printf("Formatting drive 0:...\n");
    
    memset(&fs_header, 0, sizeof(FSHeader));
    fs_header.formatted = 1;
    fs_header.current_drive = 0;
    strcpy(fs_header.current_path, "/");
    fs_header.root.file_count = 0;
    strcpy(fs_header.root.path, "/");
    fs_header.next_data_offset = 8192;  // After header (2 sectors)
    
    // Erase entire partition
    uint32_t ints = save_and_disable_interrupts();
    flash_range_erase(FLASH_OFFSET_DRIVE0, FLASH_SIZE_DRIVE0);
    restore_interrupts(ints);
    
    write_header();
    fs_mounted = 1;
    
    printf("Drive 0: formatted\n");
    return 0;
}

int fs_drives(void) {
    printf("Available drives:\n");
    printf("  0: Flash (1MB) %s\n", fs_mounted ? "[MOUNTED]" : "[NOT MOUNTED]");
    printf("  1: SD Card (not available yet)\n");
    return 0;
}
