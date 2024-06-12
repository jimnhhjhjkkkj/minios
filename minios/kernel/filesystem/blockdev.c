#include "blockdev.h"
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>

#define BLOCK_SIZE 4096    // 블록 크기를 4096 바이트로 정의합니다.
#define NUM_BLOCKS 1024    // 블록의 수를 정의합니다.

static char storage[NUM_BLOCKS][BLOCK_SIZE]; // 블록 장치 데이터를 저장할 배열입니다.

// blockdev_read 함수 구현
int blockdev_read(uint32_t block, void* buf, size_t count) {
    printf("blockdev_read called with block: %u, count: %zu\n", block, count);
    if (block >= NUM_BLOCKS) {
        fprintf(stderr, "blockdev_read: Invalid block number %u\n", block);
        return -1;
    }
    if (count > BLOCK_SIZE) {
        fprintf(stderr, "blockdev_read: Read size exceeds block size\n");
        return -1;
    }

    memcpy(buf, storage[block], count);
    printf("blockdev_read: Read %zu bytes from block %u\n", count, block);
    return count;
}

// blockdev_write 함수 구현
int blockdev_write(uint32_t block, const void* buf, size_t count) {
    printf("blockdev_write called with block: %u, count: %zu\n", block, count);
    if (block >= NUM_BLOCKS) {
        fprintf(stderr, "blockdev_write: Invalid block number %u\n", block);
        return -1;
    }
    if (count > BLOCK_SIZE) {
        fprintf(stderr, "blockdev_write: Write size exceeds block size\n");
        return -1;
    }

    memcpy(storage[block], buf, count);
    printf("blockdev_write: Wrote %zu bytes to block %u\n", count, block);
    return count;
}

