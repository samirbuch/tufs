//
// Created by Samir Buch on 11/8/24.
//

#ifndef TUFSIMPL_COMMANDS_H
#define TUFSIMPL_COMMANDS_H

/**
 * List all available commands
 * @param args
 * @return
 */
int help(char **args);

/**
 * Exit the TUFS REPL
 * @param args
 * @return
 */
int quit_repl(char **args);

/**
 * Create a new disk
 * @param args
 * @return
 */
int new_disk(char **args);

/**
 * Mount a disk
 * @param args
 * @return
 */
int mount_disk(char **args);

/**
 * Unmount and close a disk
 * @param args
 * @return
 */
int unmount_disk(char **args);

/**
 * Print some useful information about the open disk
 * @param args
 * @return
 */
int filesystem_info(char **args);

/**
 * Print some useful information about a tufs_file in the virtual disk
 * @param args
 * @return
 */
int file_info(char **args);

/**
 * List all files on the disk
 * @param args
 * @return
 */
int list_files(char **args);

/**
 * Create a new tufs_file on the virtual disk
 * @param args
 * @return
 */
int create_file(char **args);

/**
 * Remove a tufs_file from the virtual disk
 * @param args
 * @return
 */
int remove_file(char **args);

/**
 * Rename a tufs_file on the virtual disk
 * @param args
 * @return
 */
int rename_file(char **args);

/**
 * Copy a tufs_file.
 * <br><br>
 * There are a few different modes: <br>
 * <b>-ei</b> : Copy a tufs_file from your computer to the virtual disk <br>
 * <b>-ie</b> : Copy a tufs_file from the virtual disk to your computer <br>
 * <b>-ii</b> : Copy a tufs_file from the virtual disk to another location on the virtual disk <br>
 * * For obvious reasons, `-ee` does not exist. Use your OS's equivalent to `cp`.
 * @param args
 * @return
 */
int copy_file(char **args);

/**
 * Print the contents of a tufs_file to stdout
 * @param args
 * @return
 */
int display_file_contents(char **args);

#endif //TUFSIMPL_COMMANDS_H
