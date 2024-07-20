#include "pmm.h"

uint32_t *memory_map = 0;
uint32_t max_blocks = 0;
uint32_t used_blocks = 0;

static void set_block(uint32_t bit) {
    memory_map[bit/32] |= (1 << (bit % 32));
}

static void unset_block(uint32_t bit) {
    memory_map[bit/32] &= ~(1 << (bit % 32));
}

static bool test_block(uint32_t bit) {
    return memory_map[bit/32] & (1 << (bit % 32));
}

static int32_t find_first_free_blocks(uint32_t nblocks) {
    if (nblocks == 0) return -1;
    
    for (uint32_t i = 0; i < max_blocks / 32; i++) {
        if (memory_map[i] != 0xFFFFFFFF) {
            for (uint32_t j = 0; j < 32; j++) {
                int32_t bit = 1 << j;
                if (!(memory_map[i] & bit)) {
                    int32_t start_bit = i * 32 + bit;
                    uint32_t free_blocks = 0;
                    for (uint32_t count = 0; count <= nblocks; count++) {
                        if(!test_block(start_bit + count)) free_blocks++;
                        if (free_blocks == nblocks) return i * 32 + j;
                    }
                }
            }
        }
    }
    return -1;
}

void initialize_physical_memory_manager(uint32_t start_address, uint32_t size) {
    memory_map = (uint32_t*)start_address;
    max_blocks = size / BLOCK_SIZE;
    used_blocks = max_blocks;
    memset(memory_map, 0xFF, max_blocks / BLOCKS_PER_BYTE);
}
void initialize_memory_region(uint32_t address, uint32_t size) {
    int32_t align = address / BLOCK_SIZE;
    int32_t n_blocks = size / BLOCK_SIZE;
    for(; n_blocks > 0; n_blocks--) {
        unset_block(align++);
        used_blocks--;
    }
    set_block(0);
}
void deinitialize_memory_region(uint32_t address, uint32_t size) {
    int32_t align = address / BLOCK_SIZE;
    int32_t nblocks = size / BLOCK_SIZE;
    for(; nblocks > 0; nblocks--) {
        set_block(align++);
        used_blocks++;
    }
}
void *alloc_blocks(uint32_t nblocks) {
    
    if ((max_blocks - used_blocks) <= nblocks) return 0;
    
    int32_t start_addr = find_first_free_blocks(nblocks);
    if (start_addr == -1) return 0;
    for (uint32_t i = 0; i < nblocks; i++)
        set_block(start_addr + i);
    used_blocks += nblocks;
    
    uint32_t address = start_addr * BLOCK_SIZE;
    return (void*)address;
}
void free_blocks(void* address, uint32_t nblocks) {
    int32_t start_addr = (uint32_t)address / BLOCK_SIZE;
    for (uint32_t i = 0; i < nblocks; i++)
        unset_block(start_addr + i);
    used_blocks -= nblocks;
}