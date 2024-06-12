#define FUSE_USE_VERSION 31

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <assert.h>
#include <stdlib.h>

static const char *vfs_str = "Hello, World!\n";
static const char *vfs_path = "/hello";

static int vfs_getattr(const char *path, struct stat *stbuf, struct fuse_file_info *fi)
{
    int res = 0;

    memset(stbuf, 0, sizeof(struct stat));
    if (strcmp(path, "/") == 0) {
        stbuf->st_mode = S_IFDIR | 0755;
        stbuf->st_nlink = 2;
    } else if (strcmp(path, vfs_path) == 0) {
        stbuf->st_mode = S_IFREG | 0444;
        stbuf->st_nlink = 1;
        stbuf->st_size = strlen(vfs_str);
    } else
        res = -ENOENT;

    return res;
}

static int vfs_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                       off_t offset, struct fuse_file_info *fi,
                       enum fuse_readdir_flags flags)
{
    if (strcmp(path, "/") != 0)
        return -ENOENT;

    filler(buf, ".", NULL, 0, 0);
    filler(buf, "..", NULL, 0, 0);
    filler(buf, vfs_path + 1, NULL, 0, 0);

    return 0;
}

static int vfs_open(const char *path, struct fuse_file_info *fi)
{
    if (strcmp(path, vfs_path) != 0)
        return -ENOENT;

    return 0;
}

static int vfs_read(const char *path, char *buf, size_t size, off_t offset,
                    struct fuse_file_info *fi)
{
    size_t len;
    (void) fi;
    if(strcmp(path, vfs_path) != 0)
        return -ENOENT;

    len = strlen(vfs_str);
    if (offset < len) {
        if (offset + size > len)
            size = len - offset;
        memcpy(buf, vfs_str + offset, size);
    } else
        size = 0;

    return size;
}

static struct fuse_operations vfs_oper = {
    .getattr = vfs_getattr,
    .readdir = vfs_readdir,
    .open = vfs_open,
    .read = vfs_read,
};

int vfs_main(int argc, char *argv[])
{
    return fuse_main(argc, argv, &vfs_oper, NULL);
}

