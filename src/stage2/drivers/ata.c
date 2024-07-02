#include "ata.h"

void ata_write_sector(uint32_t lba, uint8_t sectorct, uint16_t *data) {
    while (inb(ATA_STATUS) & 0x80);
    outb(ATA_DRIVE, 0xE0 | (lba >> 24) & 0x0F);
    outb(ATA_SECTOR_CT, sectorct);
    outb(ATA_LBA_LOW, (uint8_t)(lba));
    outb(ATA_LBA_MID, (uint8_t)(lba >> 8));
    outb(ATA_LBA_HIGH, (uint8_t)(lba >> 16));

    outb(ATA_COMMAND, 0x30); // 0x30 - write sectors

    uint16_t status = 0;
    do {
        status = inb(ATA_STATUS);
    } while (!(status & 0x08));

    for (uint32_t i = 0; i < 256 * sectorct; i++) {
        outw(ATA_DATA, data[i]);
    }
    while (inb(ATA_STATUS) & 0x08);
}
