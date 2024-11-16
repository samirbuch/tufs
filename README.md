# Project-5-FileSystem
## Project 5: Implementing a Simple File System

The goal of this project is to implement a simple file system. The approach is to implement the file system on a “virtual disk”. The virtual disk is a file within the existing operating system’s file system.  

The file system you implement will include the basic features that are found in any file system: a directory of its files, storage of files and file data, a library of functions to create and delete directories and files, and read and write files.  

In this project, **the virtual disk is a file in the Linux file system**. This means that the basic low-level file system functions used to access data in a Linux file can be used to build the library functions for your file system.  

To this end, you will implement a library of functions that offer a set of basic file system calls (such as open, read, write, ...) to applications. The file data and file system meta-information will be stored on the virtual disk. As noted, this virtual disk is a single file that is stored on the "real" file system provided by the Linux operating system.  

To create and access the virtual disk, we have provided a few definitions and helper functions that you can find in this header file (disk.h) and this source file (disk.c) . Note that, in the library you develop, you are not permitted to create any "real" files on the Linux file system itself (except the virtual disk file). Instead, you must use the provided helper functions and store all the data that you need and create on the virtual disk.  

As you can see by looking at the provided header and source files, the virtual disk has 8,192 blocks, and each block holds 4096 bytes (4KB). Using the helper files you can create an empty virtual disk, open and close a virtual disk, and read and write entire disk blocks (by providing a block number in the range between 0 and 8,191 inclusive).  

To make things simpler, your file system does not have to support a directory hierarchy. Instead, all files are stored in a single root directory on the virtual disk. In addition, your file system does not have to store more than 64 files (of course, you can create and delete files, and deleted files do not count against this 64 file limit). Finally, out of the 8,192 blocks available on disk, only 4,096 must be reserved as data blocks. That is, you have ample space to store your file meta-information. However, you have to free data blocks (make them available again) when the corresponding file is deleted. The maximum file size is 16 megabytes (all 4,096 data blocks, each block with 4KB).  

### File System Management Functions   
To manage your file system, you must provide the following three functions:  

     int init_fs(char *disk_name);  
     
This function creates a fresh (and empty) file system on the virtual disk with name disk_name. As part of this function, you should first invoke make_disk(disk_name) to create a new disk. Then, open this disk and write/initialize the necessary meta- information for your file system so that it can be later used (mounted). The function returns 0 on success, and -1 when the disk disk_name could not be created, opened, or properly initialized.  

     int mount_fs(char *disk_name);  
     
This function mounts a file system that is stored on a virtual disk with name disk_name. With the mount operation, a file system becomes "ready for use." You need to open the disk and then load to memory the meta-information that is necessary to handle the file system operations that are discussed below. The function returns 0 on success, and -1 when the disk disk_name could not be opened or when the disk does not contain a valid file system (that you previously created with init_fs).  

     int umount_fs(char *disk_name);  
     
This function unmounts your file system from a virtual disk with name disk_name. As part of this operation, you need to write back all meta-information so that the disk persistently reflects all changes that were made to the file system (such as new files that are created, data that is written, ...). You should also close the disk. The function returns 0 on success, and -1 when the disk disk_name could not be closed or when data could not be written to the disk (this should not happen). If there are any open file descriptors (that point to files in the file system that is about to be unmounted), umount_fs will close them. 

It is important to observe that your **file system must provide persistent storage**. That is, assume that you have created a file system on a virtual disk and mounted it. Then, you create a few files and write some data to them. Finally, you unmount the file system. At this point, all data must be written onto the virtual disk. Also, all directory structures (physical and logical) must be updated on the virtual disk.  

Another program that mounts the file system at a later point in time must see the previously created files and the data that was written. This means that whenever umount_fs is called, all meta-information and file data (that you could temporarily have only in memory; depending on your implementation) must be written out to disk. 

### File System Functions  

In addition to the management routines listed above, you are to implement the following file system functions (which are very similar to the corresponding Linux file system operations). These file system functions require that a file system was previously mounted.  

     int fs_open(char *name);  
     
The file specified by name is opened for reading and writing, and the file descriptor corresponding to this file is returned to the calling function. If successful, fs_open returns a non-negative integer, which is a file descriptor that can be used to subsequently access this file. Note that the same file (file with the same name) can be opened multiple times. When this happens, your file system is to provide multiple, independent file descriptors. Your library must support a maximum of 32 file descriptors that can be open simultaneously. fs_open returns -1 on failure. It is a failure when the file with name cannot be found (i.e., it has not been created previously or is already deleted). It is also a failure when there are already 32 file descriptors active. When a file is opened, the file offset (seek pointer) is set to 0 (the beginning of the file).  

     int fs_close(int fildes);  
     
The file descriptor fildes is closed. A closed file descriptor can no longer be used to access the corresponding file. Upon successful completion, a value of 0 is returned. In case the file descriptor fildes does not exist or is not open, the function returns -1.  

     int fs_create(char *name);  
     
This function creates a new file with name name in the root directory of your file system. The file is initially empty. The maximum length for a file name is 15 characters. Also, there can be at most 64 files in the directory. Upon successful completion, a value of 0 is returned. fs_create returns -1 on failure. It is a failure when the file with name already exists, when the file name is too long (it exceeds 15 characters), or when there are already 64 files present in the root directory. Note that to access a file that is created, it has to be subsequently opened.  

     int fs_delete(char *name);  
     
This function deletes the file with name name from the root directory of your file system and frees all data blocks and meta-information that correspond to that file. The file that is being deleted must not be open. That is, there cannot be any open file descriptor that refers to the file name. When the file is open at the time that fs_delete is called, the call fails and the file is not deleted. Upon successful completion, a value of 0 is returned. fs_delete returns -1 on failure. It is a failure when the file with name does not exist. It is also a failure when the file is currently open (i.e., there exists at least one open file descriptor that is associated with this file).   

     int fs_read(int fildes, void *buf, size_t nbyte);  
     
This function attempts to read nbyte bytes of data from the file referenced by the descriptor fildes into the buffer pointed to by buf. The function assumes that the buffer buf is large enough to hold at least nbyte bytes. When the function attempts to read past the end of the file, it reads all bytes until the end of the file. Upon successful completion, the number of bytes that were actually read is returned. This number could be smaller than nbyte when attempting to read past the end of the file (when trying to read while the file pointer is at the end of the file, the function returns zero). In case of failure, the function returns -1. It is a failure when the file descriptor fildes is not valid. The read function implicitly increments the file pointer by the number of bytes that were actually read.  

     int fs_write(int fildes, void *buf, size_t nbyte);  
     
This function attempts to write nbyte bytes of data to the file referenced by the descriptor fildes from the buffer pointed to by buf. The function assumes that the buffer buf holds at least nbyte bytes. When the function attempts to write past the end of the file, the file is automatically extended to hold the additional bytes. It is possible that the disk runs out of space while performing a write operation. In this case, the function attempts to write as many bytes as possible (i.e., to fill up the entire space that is left). The maximum file size is 16M (which is, 4,096 blocks, each 4KB). Upon successful completion, the number of bytes that were actually written is returned. This number could be smaller than nbyte when the disk runs out of space (when writing to a full disk, the function returns zero). In case of failure, the function returns -1. It is a failure when the file descriptor fildes is not valid. The write function implicitly increments the file pointer by the number of bytes that were actually written.   

int fs_get_filesize(int fildes);  

This function returns the current size of the file pointed to by the file descriptor fildes. In case fildes is invalid, the function returns -1.   

     int fs_lseek(int fildes, tufs_off_t offset);  
     
This function sets the file pointer (the offset used for read and write operations) associated with the file descriptor fildes to the argument offset. It is an error to set the file pointer beyond the end of the file. To append to a file, one can set the file pointer to the end of a file, for example, by calling fs_lseek(fd, fs_get_filesize(fd));. Upon successful completion, a value of 0 is returned. fs_lseek returns -1 on failure. It is a failure when the file descriptor fildes is invalid, when the requested offset is larger than the file size, or when offset is less than zero.   

     int fs_truncate(int fildes, tufs_off_t length);  
     
This function causes the file referenced by fildes to be truncated to length bytes in size. If the file was previously larger than this new size, the extra data is lost and the corresponding data blocks on disk (if any) must be freed. It is not possible to extend a file using fs_truncate. When the file pointer is larger than the new length, then it is also set to length (the end of the file). Upon successful completion, a value of 0 is returned. fs_lseek returns -1 on failure. It is a failure when the file descriptor fildes is invalid or the requested length is larger than the file size.   

### Implementation   

In principle, your file system could be implemented in any of many different file system structures, such as the FAT (file allocation table) design, or the (though likely more complex to implement) Unix (inode)-based design. But **for this project you are to use the FAT design model** with a file data storage region of 4,096 blocks. That space does not include the logical and physical directory storage regions.  

You will need several data structures on disk, including a super (boot) block, a root directory, information about free and empty blocks on disk, meta-information for each file (such as file size, current offset, etc.), and a mapping from a file to its data blocks.   

The super block is typically the first block of the disk, and it stores information about the location of the other data structures. For example, you can store in the super block the whereabouts of the file allocation table, the directory, and the start of the data blocks.  

The directory holds the names of the files. When using a FAT-based design, the directory also stores, for each file, its file size and the head of the list of corresponding data blocks.  

The file allocation table (FAT) is convenient because it can be used to keep track of empty blocks and the mapping between files and their data blocks.  

In addition to the file-system-related data structures on disk, you also need support for file descriptors. A **file descriptor** is an integer in the range between 0 and 31 (inclusive) that is returned when a file is opened, and it is used for subsequent file operations (such as reading and writing). A file descriptor is associated with a file, and it also contains a file offset (seek pointer). This offset indicates the point in the file where read and write operations start. It is implicitly updated (incremented) whenever you perform a fs_read or fs_write operation, and it can be explicitly moved within the file by calling fs_lseek. Note that file descriptors are **not** stored on disk. They are only meaningful while an application is running and the file system is mounted. Once the file system is unmounted, file descriptors are no longer meaningful (and, hence, should be all closed before a call to umount_fs).  

### Project Implementation Information:
1)	Development is to be done using the GitHub repository for the project. Your code must be implemented in C. You should include file system disk.h as well as file system disk.c to build a complete executable, but do not make any changes to these two files. 
2)	You are to prepare a design document that describes:  
a)	 the volume layout (space allocation),  
b)	the structure of the physical directory (space allocation),  
c)	the method of space allocation including the management of free blocks,  
d)	the logical directory structure, including record structure for naming and describing files, and  
e)	the relationship between the logical and physical directories.  
f)	Clear description of the functions you write to manage the disk and functions provided to the user.  
3)	In addition to the file system implementation, you are to develop one or more small applications to **demonstrate the functionality of your file system**. The applications should include the following operations:  
a)	Create a disk volume  
b)	Create a file  
c)	Open and Write data to the created file; then close the file  
d)	Another thread or process should be able to open the file and read data from various locations (blocks) in the file and compare those data to what was previous written.  
e)	Then this application should copy the file to a new file (different file name). When the copy is complete, the original file should be deleted.  
i)	You should be able to show that the directory has been correctly updated and that space originally allocated to the deleted file has been returned to the file system.  
4)	All files that you need to build the executable must be included (sources, headers, makefile) in the folder uploaded to Canvas. Please do not include any object or executable files.
   
### Deliverables  
**Week 1**  
Review the features of a FAT filesystem, its space allocation, components and directory requirements  
Design the information required for the 'boot/super block'  
Design the physical directory properties (include how to manage free blocks and allocate data blocks to files)  
Design the logical directory (how to identify and store the descriptive or meta information for a file)  
Design the linkage between physical and logical directories  
Design the file descriptor (fildes) structure and management  

create the File System Management functions  
- init_fs  
- mount_fs  
- unmount_fs  

**Week 2**  
Create the File System Functions  
- fs_open  
- fs_close  
- fs_create  
- fs_delete  
- fs_read  
- fs_write  
- fs_get_filesize  
- fs_lseek  
- fs_trunc  

**Week 3**
Complete any remaining functions in the project  
Complete Application to demonstrate the implementation  


