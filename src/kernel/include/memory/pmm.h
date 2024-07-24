#ifndef PMM_H
#define PMM_H
#define BLOCK_SIZE 4096
#define BLOCKS_PER_BYTE 8
#include "../libc/stdint.h"
#include "../libc/stdbool.h"
#include "../libc/string.h"

// https://www.youtube.com/watch?v=RD9gtZEPFKU - Queso Fuego

extern uint32_t *memory_map;
extern uint32_t max_blocks;
extern uint32_t used_blocks;
void initialize_physical_memory_manager(uint32_t start_address, uint32_t size);
void initialize_memory_region(uint32_t address, uint32_t size);
void deinitialize_memory_region(uint32_t address, uint32_t size);
void *alloc_blocks(uint32_t blocks);
void free_blocks(void* address, uint32_t size);
#endif