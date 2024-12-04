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
int list_all_commands(char **args);

/**
 * Execute a command in the native shell rather than in TUFS REPL
 * @param args
 * @return
 */
int shell_cmd(char **args);

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
 * Sync the changes in memory with the physical disk without unmounting
 * @param args
 * @return
 */
int sync_disk(char **args);

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
 * Print some useful information about a tufs_dirent in the virtual disk
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
 * Create a new tufs_dirent on the virtual disk
 * @param args
 * @return
 */
int create_file(char **args);

/**
 * Remove a tufs_dirent from the virtual disk
 * @param args
 * @return
 */
int remove_file(char **args);

/**
 * Rename a tufs_dirent on the virtual disk
 * @param args
 * @return
 */
int rename_file(char **args);

/**
 * Copy a tufs_dirent.
 * <br><br>
 * There are a few different modes: <br>
 * <b>(nothing)</b> : Copy a tufs_dirent from one location to another on the virtual disk <br>
 * <b>-ei</b> : Copy a tufs_dirent from your computer to the virtual disk <br>
 * <b>-ie</b> : Copy a tufs_dirent from the virtual disk to your computer <br>
 * @param args
 * @return
 */
int copy_file(char **args);

/**
 * Print the contents of a tufs_dirent to stdout
 * @param args
 * @return
 */
int display_file_contents(char **args);

/**
 * Edit the contents of a tufs_dirent
 * @param args
 * @return
 */
int edit_file(char **args);

/**
 * Write to a specific file
 * @param args First arg file name, rest of the args what you want to write.
 * @return
 */
int write_to_file(char **args);

/**
 * Truncate a file to a specific length
 * @param args
 * @return
 */
int truncate_file(char **args);

#endif //TUFSIMPL_COMMANDS_H
