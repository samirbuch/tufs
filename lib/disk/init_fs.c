#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "disk.h"
#include "tufsdef.h"

int init_fs(char *name) {
    if (make_disk(name) == TUFS_ERROR) {
        return TUFS_ERROR;
    }

    if (open_disk(name) == TUFS_ERROR) {
        return TUFS_ERROR;
    }

    /* Write boot sector information */

    struct boot_sector bs_instance;
    // Initialize jump instruction (common values)
    bs_instance.jump_instruction[0] = 0xFF;
    bs_instance.jump_instruction[1] = 0xFF;
    bs_instance.jump_instruction[2] = 0xFF; // bootstrap doesn't exist in this toy filesystem
    // Initialize OEM name/version (pad with spaces)
    memset(bs_instance.oem, ' ', sizeof(bs_instance.oem));
    memcpy(bs_instance.oem, "TUFS24", 6); // Copy up to 8 bytes
    // Initialize bytes per sector (little-endian)
    bs_instance.bytes_per_sector = (uint16_t) BLOCK_SIZE; // hex: 0x1000
    // Initialize sectors per cluster
    bs_instance.sectors_per_cluster = 1;
    // Initialize reserved sectors
    bs_instance.reserved_sectors = 1;
    // Initialize number of FAT copies
    bs_instance.num_fats = 2;
    // Initialize number of root directory entries
    bs_instance.num_root_entries = 64;
    // Initialize total sectors
    bs_instance.total_sectors = (uint16_t) DISK_BLOCKS; // hex: 0x2000
    // Initialize FAT1 start block
    bs_instance.fat1_start = 100; // Disk location 0x64000
    // Initialize FAT2 start block
    bs_instance.fat2_start = 200; // Disk location 0xC8000
    // Initialize root directory start block
    bs_instance.root_start = 300;
    // Initialize data start block
    bs_instance.data_start = 4096;
    // Initialize unused area to zeros
    memset(bs_instance.unused, 0, sizeof(bs_instance.unused));
    // Initialize signature (0x55AA in little-endian)
    bs_instance.signature = 0xAA55;

    // Create temporary buffer size of block
    void *bs = malloc(BLOCK_SIZE);
    if (!bs) {
        perror("malloc");
        return TUFS_ERROR;
    }
    // Zero it
    memset(bs, 0, BLOCK_SIZE);
    // Copy boot sector struct into buffer
    memcpy(bs, &bs_instance, sizeof(bs_instance));

    // Write boot sector to disk
    if (block_write(0, (char *) bs) == TUFS_ERROR) {
        free(bs);
        return TUFS_ERROR;
    }

    // Initialize the FAT
    struct tufs_fat fat;
    // Initialize all entries to 0xFFFF (unused)
    for (int i = 0; i < FAT_SIZE; i++) {
        fat.table[i] = 0xFFFF;
        fat.block_status[i] = EMPTY;
    }
    void *f = malloc(sizeof(fat)); // 12288 bytes
    if (!f) {
        free(bs);
        perror("malloc");
        return TUFS_ERROR;
    }

    memset(f, 0, sizeof(fat));
    memcpy(f, &fat, sizeof(fat));

    // Write the FAT to the disk
    uint16_t num_blocks_to_write = ceil((double) sizeof(fat) / BLOCK_SIZE);
    for (int i = 0; i < num_blocks_to_write; i++) {
        if (block_write(bs_instance.fat1_start + i, (char *) f + i * BLOCK_SIZE) == TUFS_ERROR) {
            free(bs);
            free(f);
            return TUFS_ERROR;
        }

        if (block_write(bs_instance.fat2_start + i, (char *) f + i * BLOCK_SIZE) == TUFS_ERROR) {
            free(bs);
            free(f);
            return TUFS_ERROR;
        }
    }

    // Initialize the root directory
    struct tufs_root root;
    // Initialize all entries to 0xFFFF (unused)
    for (int i = 0; i < MAX_FILES; i++) {
        root.files[i] = (file_t) {0};
    }
    void *r = malloc(BLOCK_SIZE);
    if (!r) {
        free(bs);
        free(f);
        perror("malloc");
        return TUFS_ERROR;
    }
    memset(r, 0, BLOCK_SIZE);
    memcpy(r, &root, sizeof(root));

    // Write the root directory to the disk
    if (block_write(bs_instance.root_start, (char *) r) == TUFS_ERROR) {
        free(bs);
        free(f);
        free(r);
        return TUFS_ERROR;
    }

    free(bs);
    free(f);
    free(r);

    if (close_disk() == TUFS_ERROR) {
        return TUFS_ERROR;
    }

    return TUFS_SUCCESS;
}