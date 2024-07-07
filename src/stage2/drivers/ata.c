#include "ata.h"

#define ATA_SR_BSY      0x80
#define ATA_SR_DRQ      0x08

// ???
void ata_write_sector(uint32_t lba, uint8_t sectorct, uint16_t *data) {
    for (uint8_t sector = 0; sector < sectorct; sector++) {
        while (inb(ATA_STATUS) & ATA_SR_BSY);

        outb(ATA_DRIVE, 0xE0 | ((lba >> 24) & 0x0F));

        outb(ATA_SECTOR_CT, 1);

        outb(ATA_LBA_LOW, (uint8_t)(lba));
        outb(ATA_LBA_MID, (uint8_t)(lba >> 8));
        outb(ATA_LBA_HIGH, (uint8_t)(lba >> 16));

        outb(ATA_COMMAND, 0x30);

        while (!(inb(ATA_STATUS) & ATA_SR_DRQ));

        for (uint32_t i = 0; i < 256; i++) {
            outw(ATA_DATA, data[sector * 256 + i]);
        }

        while (inb(ATA_STATUS) & ATA_SR_BSY);
        lba++;
    }
}

void ata_read_sector(uint32_t lba, uint8_t sectorct, uint16_t *data) {
    printf("%d\n", lba);
    for (uint8_t sector = 0; sector < sectorct; sector++) {
        while (inb(ATA_STATUS) & ATA_SR_BSY);

        outb(ATA_DRIVE, 0xE0 | ((lba >> 24) & 0x0F));

        outb(ATA_SECTOR_CT, 1);

        outb(ATA_LBA_LOW, (uint8_t)(lba));
        outb(ATA_LBA_MID, (uint8_t)(lba >> 8));
        outb(ATA_LBA_HIGH, (uint8_t)(lba >> 16));

        outb(ATA_COMMAND, 0x20);

        while (!(inb(ATA_STATUS) & ATA_SR_DRQ));

        for (uint32_t i = 0; i < 256; i++) {
            data[sector * 256 + i] = inw(ATA_DATA);
        }

        while (inb(ATA_STATUS) & ATA_SR_BSY);
        lba++;
    }
}
