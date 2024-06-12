#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "ext2.h"
#include "blockdev.h"

int ext2_open(const char* path, int flags, mode_t mode) {
	
    
    // inode 할당 및 초기화
    ext2_file_t* file;
    get_inode_from_path(path, file->inode);
    printf("ext2_open called with path: %s, flags: %d, mode: %d\n", path, flags, mode);
    
    file->fd = malloc(sizeof(ext2_file_t));
    if (file == NULL) {
        printf("ext2_open: Failed to allocate memory for file\n");
        return -1;
    }
    file->inode = malloc(sizeof(ext2_inode_t));
    if (file->inode == NULL) {
        printf("ext2_open: Failed to allocate memory for inode\n");
        free(file);
        return -1;
    }
    if (get_inode_from_path(path, file->inode) != 0) {
        printf("ext2_open: Failed to get inode from path\n");
        free(file->inode);
        free(file);
        return -1;
    }
    file->pos = 0;
    printf("ext2_open: File opened successfully, fd: %p\n", (void*)file);
    return (int)file->fd;
}

int ext2_read(int fd, void* buf, size_t count) {
    printf("ext2_read called with fd: %d, count: %zu\n", fd, count);
    ext2_file_t* file = (ext2_file_t*)fd;
    uint32_t remaining = file->inode->size - file->pos;
    if (remaining == 0) {
        return 0; // 파일 끝에 도달함
    }
    size_t bytes_to_read = (remaining < count) ? remaining : count;

    // 데이터 블록에서 읽기
    int read_result = read_blocks(file->inode->blocks, file->pos, buf, bytes_to_read);
    file->pos += bytes_to_read;
    printf("ext2_read: Read %zu bytes, new pos: %u\n", bytes_to_read, file->pos);
    return read_result;
}

int ext2_write(int fd, const void* buf, size_t count) {
    printf("ext2_write called with fd: %d, count: %zu\n", fd, count);
    ext2_file_t* file = (ext2_file_t*)fd;
    if (file->pos + count > file->inode->size) {
        printf("ext2_write: Write exceeds file size\n");
        return -1; // 파일 크기를 초과함
    }

    // 데이터 블록에 쓰기
    int write_result = write_blocks(file->inode->blocks, file->pos, buf, count);
    file->pos += count;
    printf("ext2_write: Wrote %zu bytes, new pos: %u\n", count, file->pos);
    return write_result;
}

int ext2_close(int fd) {
    printf("ext2_close called with fd: %d\n", fd);
    ext2_file_t* file = (ext2_file_t*)fd;
    free(file->inode);
    free(file);
    printf("ext2_close: File closed successfully\n");
    return 0;
}

// inode 테이블 및 관련 함수
#define MAX_FILES 10

static inode_t inode_table[MAX_FILES];
static bool inode_table_initialized = false;

int get_inode_from_path(const char* path, inode_t* inode) {
    printf("get_inode_from_path called with path: %s\n", path);
    if (!inode_table_initialized) {
    
        // inode_table 초기화 (간단한 예제를 위해)
        for (int i = 0; i < MAX_FILES; i++) {
            inode_table[i].block_start = i * 10; // 각 파일은 10 블록씩 떨어져 있음
            inode_table[i].size = BLOCK_SIZE * 2; // 각 파일은 2 블록 크기
        }
        inode_table_initialized = true;
    }

    // 여기서는 단순화를 위해 항상 첫 번째 inode를 반환합니다.
    if (inode) {
        *inode = inode_table[0];
        printf("get_inode_from_path: Inode obtained successfully\n");
        return 0; // 성공
    }
    printf("get_inode_from_path: Failed to obtain inode\n");
    return -1; // 실패
}

// 연속된 블록을 읽는 함수
int read_blocks(inode_t* inode, void* buf, size_t size, uint32_t start_block) {
    printf("read_blocks called with start_block: %u, size: %zu\n", start_block, size);
    if (start_block + (size / BLOCK_SIZE) > NUM_BLOCKS) {
        fprintf(stderr, "read_blocks: Request exceeds block device size\n");
        return -1;
    }

    size_t total_bytes_read = 0;
    uint32_t current_block = start_block;
    while (total_bytes_read < size) {
        size_t bytes_to_read = (size - total_bytes_read > BLOCK_SIZE) ? BLOCK_SIZE : size - total_bytes_read;
        if (blockdev_read(current_block, (char*)buf + total_bytes_read, bytes_to_read) < 0) {
            return -1; // blockdev_read 실패
        }
        total_bytes_read += bytes_to_read;
        current_block++;
    }
    printf("read_blocks: Read %zu bytes in total\n", total_bytes_read);
    return total_bytes_read;
}

// 연속된 블록에 데이터를 쓰는 함수
int write_blocks(inode_t* inode, const void* buf, size_t size, uint32_t start_block) {
    printf("write_blocks called with start_block: %u, size: %zu\n", start_block, size);
    if (start_block + (size / BLOCK_SIZE) > NUM_BLOCKS) {
        fprintf(stderr, "write_blocks: Request exceeds block device size\n");
        return -1;
    }

    size_t total_bytes_written = 0;
    uint32_t current_block = start_block;
    while (total_bytes_written < size) {
        size_t bytes_to_write = (size - total_bytes_written > BLOCK_SIZE) ? BLOCK_SIZE : size - total_bytes_written;
        if (blockdev_write(current_block, (const char*)buf + total_bytes_written, bytes_to_write) < 0) {
            return -1; // blockdev_write 실패
        }
        total_bytes_written += bytes_to_write;
        current_block++;
    }
    printf("write_blocks: Wrote %zu bytes in total\n", total_bytes_written);
    return total_bytes_written;
}

