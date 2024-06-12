#include <stdio.h>
#include <stdlib.h>
#include "vfs.h"
#include "ext2.h"

int vfs_open(const char* path, int flags, mode_t mode) {
    printf("vfs_open called with path: %s, flags: %d, mode: %d\n", path, flags, mode);
    return ext2_open(path, flags, mode);
}

int vfs_read(int fd, void* buf, size_t count) {
    printf("vfs_read called with fd: %d, count: %zu\n", fd, count);
    return ext2_read(fd, buf, count);
}

int vfs_write(int fd, const void* buf, size_t count) {
    printf("vfs_write called with fd: %d, count: %zu\n", fd, count);
    return ext2_write(fd, buf, count);
}

int vfs_close(int fd) {
    printf("vfs_close called with fd: %d\n", fd);
    return ext2_close(fd);
}

