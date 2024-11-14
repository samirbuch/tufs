//
// Created by Samir Buch on 11/8/24.
//

#ifndef TUFSIMPL_TUFSDEF_H
#define TUFSIMPL_TUFSDEF_H

#include <stdint.h>

#define TUFS_SUCCESS 0
#define TUFS_ERROR (-1)

#define FAT_SIZE 4096 // number of elements

struct boot_sector {
    uint8_t jump_instruction[3];    // 0-2: Jump instruction
    char oem[8];                    // 3-10: OEM name/version (8 bytes)
    uint16_t bytes_per_sector;      // 11-12: Number of bytes per sector
    uint8_t sectors_per_cluster;    // 13: Number of sectors per cluster
    uint16_t reserved_sectors;      // 14-15: Number of reserved sectors
    uint8_t num_fats;               // 16: Number of FAT copies
    uint16_t num_root_entries;      // 17-18: Number of root directory entries
    uint16_t total_sectors;         // 19-20: Total number of sectors in the filesystem
    uint16_t fat1_start;            // 21: FAT1 start block - NOT STANDARD
    uint16_t fat2_start;            // 22: FAT2 start block - NOT STANDARD
    uint16_t root_start;            // 23: Root directory start block - NOT STANDARD
    uint16_t data_start;            // 24: Data start block - NOT STANDARD
    uint8_t unused[478];            // 25-509: Unused
    uint16_t signature;             // 510-511: Signature 55 aa
};

// The index of the element of the table corresponds to the physical block number
// in the data section of the file system. The *value* of the element is the next index (block)
// that contains file data. If that value is 0xFFFF, then the block is the last block of the file.
//
// Similarly, with unused_blocks, upon initialization, all elements are set to 0. If an element in the
// FAT is used, then the corresponding index in unused_blocks is set to 1. When writing a file, the first
// found instance of 0 in unused_blocks is used as the new block index.
struct tufs_fat {
    uint16_t table[FAT_SIZE];
    uint8_t unused_blocks[FAT_SIZE];
};

typedef struct tufs_dirent {
    char name[15];
    char attribute; // unused. all files are files, no directories
    uint16_t create_time;
    uint16_t create_date;
    uint16_t last_access_date;
    uint16_t last_modified_time;
    uint16_t last_modified_date;
    uint16_t starting_cluster; // index in the FAT
    uint32_t file_size;
} file_t;

#endif //TUFSIMPL_TUFSDEF_H
