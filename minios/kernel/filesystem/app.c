#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vfs.h"

int app_main() {
    printf("Entering app_main\n");

    // 파일 생성
    int fd = vfs_open("/example.txt", VFS_O_CREAT | VFS_O_WRONLY, 0644);
    if (fd < 0) {
        printf("파일 생성 실패\n");
        return 1;
    }

    // 파일 쓰기
    char* data = "Hello, World!";
    vfs_write(fd, data, strlen(data));

    // 파일 닫기
    vfs_close(fd);

    printf("파일 생성 및 쓰기 완료\n");
    return 0;
}

