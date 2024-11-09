//
// Created by Samir Buch on 11/8/24.
//

#ifndef TUFSIMPL_TUFSDEF_H
#define TUFSIMPL_TUFSDEF_H

#include <stdint.h>

#define FAT_SIZE 4096

typedef struct tufs_file {
    char name[15];
    char attribute; // unused. all files are files, no directories
    uint16_t create_time;
    uint16_t create_date;
    uint16_t last_access_date;
    uint16_t last_modified_time;
    uint16_t last_modified_date;
    uint16_t starting_cluster;
    uint32_t file_size;
} file_t;

#endif //TUFSIMPL_TUFSDEF_H
