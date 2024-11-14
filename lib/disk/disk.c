#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#include "disk.h"
#include "../tufsdef.h"

int active = 0;
int handle = -1;

/******************************************************************************/
int make_disk(char *name) {
    int f, cnt;
    char buf[BLOCK_SIZE];

    if (!name) {
        fprintf(stderr, "make_disk: invalid tufs_dirent name\n");
        return TUFS_ERROR;
    }

    if ((f = open(name, O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0) {
        perror("make_disk: cannot open tufs_dirent");
        return TUFS_ERROR;
    }

    memset(buf, 0, BLOCK_SIZE);
    for (cnt = 0; cnt < DISK_BLOCKS; ++cnt)
        write(f, buf, BLOCK_SIZE);

    close(f);

    return TUFS_SUCCESS;
}

int open_disk(char *name) {
    int f;

    if (!name) {
        fprintf(stderr, "open_disk: invalid tufs_dirent name\n");
        return TUFS_ERROR;
    }

    if (active == 1) {
        fprintf(stderr, "open_disk: disk is already open\n");
        return TUFS_ERROR;
    }

    if ((f = open(name, O_RDWR, 0644)) < 0) {
        perror("open_disk: cannot open tufs_dirent");
        return TUFS_ERROR;
    }

    handle = f;
    active = 1;

    return TUFS_SUCCESS;
}

int close_disk() {
    if (active != 1) {
        fprintf(stderr, "close_disk: no open disk\n");
        return TUFS_ERROR;
    }

    close(handle);

    active = handle = 0;

    return TUFS_SUCCESS;
}

int block_write(int block, char *buf) {
    if (active != 1) {
        fprintf(stderr, "block_write: disk not active\n");
        return TUFS_ERROR;
    }

    if ((block < 0) || (block >= DISK_BLOCKS)) {
        fprintf(stderr, "block_write: block index out of bounds\n");
        return TUFS_ERROR;
    }

    if (lseek(handle, block * BLOCK_SIZE, SEEK_SET) < 0) {
        perror("block_write: failed to lseek");
        return TUFS_ERROR;
    }

    if (write(handle, buf, BLOCK_SIZE) < 0) {
        perror("block_write: failed to write");
        return TUFS_ERROR;
    }

    return TUFS_SUCCESS;
}

int block_read(int block, char *buf) {
    if (!active) {
        fprintf(stderr, "block_read: disk not active\n");
        return TUFS_ERROR;
    }

    if ((block < 0) || (block >= DISK_BLOCKS)) {
        fprintf(stderr, "block_read: block index out of bounds\n");
        return TUFS_ERROR;
    }

    if (lseek(handle, block * BLOCK_SIZE, SEEK_SET) < 0) {
        perror("block_read: failed to lseek");
        return TUFS_ERROR;
    }

    if (read(handle, buf, BLOCK_SIZE) < 0) {
        perror("block_read: failed to read");
        return TUFS_ERROR;
    }

    return TUFS_SUCCESS;
}
