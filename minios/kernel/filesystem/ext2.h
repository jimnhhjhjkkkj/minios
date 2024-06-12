#ifndef EXT2_H
#define EXT2_H

#include <stdint.h>
#include <sys/types.h>

typedef struct ext2_inode {
    uint32_t mode;
    uint32_t size;
    uint32_t blocks[15];
    // ... 다른 필드들
} ext2_inode_t;

typedef struct ext2_file {
    ext2_inode_t* inode;
    uint32_t pos;
    int fd;
} ext2_file_t;

// inode 구조체 정의
typedef struct inode {
    uint32_t block_start; // 파일 데이터가 시작하는 블록 번호
    size_t size; // 파일 크기 (바이트 단위)
} inode_t;


int ext2_open(const char* path, int flags, mode_t mode);
int ext2_read(int fd, void* buf, size_t count);
int ext2_write(int fd, const void* buf, size_t count);
int ext2_close(int fd);
int get_inode_from_path(const char* path, inode_t* inode);
int read_blocks(inode_t* inode, void* buf, size_t size, uint32_t start_block);
int write_blocks(inode_t* inode, const void* buf, size_t size, uint32_t start_block);

#endif


