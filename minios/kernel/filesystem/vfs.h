#ifndef VFS_H
#define VFS_H

#include <sys/types.h>

#define VFS_O_RDONLY 0x01
#define VFS_O_WRONLY 0x02
#define VFS_O_RDWR   0x03
#define VFS_O_CREAT  0x04

typedef struct vfs_file vfs_file_t;

int vfs_open(const char* path, int flags, mode_t mode);
int vfs_read(int fd, void* buf, size_t count);
int vfs_write(int fd, const void* buf, size_t count);
int vfs_close(int fd);

#endif


