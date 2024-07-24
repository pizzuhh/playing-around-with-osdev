#ifndef ATA_H
#define ATA_H
#include "../include/libc/stdint.h"
#include "../include/io.h"
#include "../include/libc/stdio.h"


#define ATA_DATA            0x1F0          // R/W
#define ATA_ERROR           (ATA_DATA + 1) // R
#define ATA_FEATURES        (ATA_DATA + 1) // W
#define ATA_SECTOR_CT       (ATA_DATA + 2) // R/W
#define ATA_LBA_LOW         (ATA_DATA + 3) // R/W
#define ATA_LBA_MID         (ATA_DATA + 4) // R/W
#define ATA_LBA_HIGH        (ATA_DATA + 5) // R/W
#define ATA_DRIVE           (ATA_DATA + 6) // R/W
#define ATA_STATUS          (ATA_DATA + 7) // R/W
#define ATA_COMMAND         (ATA_DATA + 7) // R/W

void ata_write_sector(uint32_t lba, uint8_t sectorct, uint16_t *data);
void ata_read_sector(uint32_t lba, uint8_t sectorct, uint16_t *data);

#endif
