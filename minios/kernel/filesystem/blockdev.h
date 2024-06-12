#ifndef BLOCKDEV_H
#define BLOCKDEV_H

#include <stdint.h>
#include <sys/types.h>

#define BLOCK_SIZE 4096    // 블록 크기를 4096 바이트로 정의합니다.
#define NUM_BLOCKS 1024    // 블록의 수를 정의합니다.

int blockdev_read(uint32_t block, void* buf, size_t count);
int blockdev_write(uint32_t block, const void* buf, size_t count);

#endif


