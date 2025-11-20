#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <stdint.h>

// Initialize filesystem (format if needed)
int fs_init(void);

// Mount/unmount drives
int fs_mount(uint8_t drive);
int fs_unmount(uint8_t drive);

// Get current drive and path
uint8_t fs_get_drive(void);
const char* fs_get_path(void);

// Change directory (handles drive switching: "0:", "1:", or path)
int fs_cd(const char *path);

// Directory operations
int fs_mkdir(const char *path);
int fs_rmdir(const char *path);
int fs_dir(const char *path);  // List directory

// File operations
int fs_save(const char *filename);   // Save current program
int fs_load(const char *filename);   // Load program
int fs_rm(const char *filename);     // Delete file
int fs_write_note(const char *filename, const char *text);  // Write single-line note to file

// System operations
int fs_format(uint8_t drive);
int fs_drives(void);  // List available drives

#endif // FILESYSTEM_H
