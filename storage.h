#ifndef STORAGE_H
#define STORAGE_H

// Initialize flash storage system
void storage_init(void);

// Save current program to flash
// Returns 0 on success, -1 on error
int storage_save(void);

// Load program from flash to memory
// Returns 0 on success, -1 on error
int storage_load(void);

// Check if a saved program exists in flash
// Returns 1 if exists, 0 if not
int storage_exists(void);

#endif // STORAGE_H
