#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_FILES 100
#define MAX_FILENAME 256
#define MAX_CONTENT 1024
#define MAX_DIRECTORIES 50
#define MAX_PASSWORD_LENGTH 128
#define MAX_COMMAND_LENGTH 100

// 파일 구조체
struct File {
    char name[MAX_FILENAME];
    char content[MAX_CONTENT];
};

// 디렉토리 구조체
struct Directory {
    char name[MAX_FILENAME];
    struct File files[MAX_FILES];
    int fileCount;
};

// 파일 시스템 구조체
struct BasicFileSystemLayer {
    struct Directory directories[MAX_DIRECTORIES];
    int directoryCount;
};

// 파일 시스템 초기화 함수
void initBasicFileSystemLayer(struct BasicFileSystemLayer* fs) {
    fs->directoryCount = 0;
}

// 디렉토리 생성 함수
int createDirectory(struct BasicFileSystemLayer* fs, char* dirname) {
    if (fs->directoryCount >= MAX_DIRECTORIES) {
        return -1; // 디렉토리 개수 제한 초과
    }
    struct Directory newDir;
    strcpy(newDir.name, dirname);
    newDir.fileCount = 0;
    fs->directories[fs->directoryCount++] = newDir;
    return 0;
}

// 파일 생성 함수
int createFile(struct Directory* dir, char* filename, char* content) {
    if (dir->fileCount >= MAX_FILES) {
        return -1; // 파일 개수 제한 초과
    }
    struct File newFile;
    strcpy(newFile.name, filename);
    strcpy(newFile.content, content);
    dir->files[dir->fileCount++] = newFile;
    return 0;
}

// 파일 읽기 함수
char* readFile(struct BasicFileSystemLayer* fs, char* filename) {
    for (int i = 0; i < fs->directoryCount; i++) {
        struct Directory* dir = &(fs->directories[i]);
        for (int j = 0; j < dir->fileCount; j++) {
            struct File* file = &(dir->files[j]);
            if (strcmp(file->name, filename) == 0) {
                return file->content;
            }
        }
    }
    return NULL; // 파일을 찾지 못함
}

// 파일 업데이트 함수
int updateFile(struct BasicFileSystemLayer* fs, char* filename, char* newContent) {
    for (int i = 0; i < fs->directoryCount; i++) {
        struct Directory* dir = &(fs->directories[i]);
        for (int j = 0; j < dir->fileCount; j++) {
            struct File* file = &(dir->files[j]);
            if (strcmp(file->name, filename) == 0) {
                strcpy(file->content, newContent);
                return 0; // 업데이트 성공
            }
        }
    }
    return -1; // 파일을 찾지 못함
}

// 파일 목록 표시 함수
void listFiles(struct Directory* dir) {
    printf("Files in directory %s:\n", dir->name);
    for (int i = 0; i < dir->fileCount; i++) {
        printf("- %s\n", dir->files[i].name);
    }
}

// 명령어 처리 함수
void handleCommand(struct BasicFileSystemLayer* fs, char* command) {
    char* token = strtok(command, " ");
    if (token == NULL) {
        printf("Invalid command.\n");
        return;
    }

    if (strcmp(token, "createDir") == 0) {
        token = strtok(command, " ");
        if (token == NULL) {
            printf("Usage: createDir [directory_name]\n");
            return;
        }
        if (createDirectory(fs, token) == 0) {
            printf("Directory created successfully.\n");
            createDirectory(fs, token);
        } else {
            printf("Failed to create directory.\n");
        }
    } else if (strcmp(token, "createFile") == 0) {
        token = strtok(command, " ");
        char* content = strtok(command, "");
        if (token == NULL || content == NULL) {
            printf("Usage: createFile [file_name] [file_content]\n");
            return;
        }
        if (createFile(&(fs->directories[0]), token, content) == 0) {
            printf("File created successfully.\n");
            createFile(&(fs->directories[0]), token, content);
        } else {
            printf("Failed to create file.\n");
        }
    } else if (strcmp(token, "readFile") == 0) {
        token = strtok(command, " ");
        if (token == NULL) {
            printf("Usage: readFile [file_name]\n");
            return;
        }
        char* content = readFile(fs, token);
        if (content != NULL) {
            printf("File content: %s\n", content);
        } else {
            printf("File not found.\n");
        }
    } else if (strcmp(token, "updateFile") == 0) {
        token = strtok(NULL, " ");
        char* newContent = strtok(NULL, "");
        if (token == NULL || newContent == NULL) {
            printf("Usage: updateFile [file_name] [new_content]\n");
            return;
        }
        if (updateFile(fs, token, newContent) == 0) {
            printf("File updated successfully.\n");
        } else {
            printf("Failed to update file.\n");
        }
    } else if (strcmp(token, "listFiles") == 0) {
        token = strtok(command, " ");
        if (token == NULL) {
            printf("Usage: listFiles [directory_name]\n");
            return;
        }
        // 디렉토리를 찾고 파일 목록을 표시
        struct Directory* dir = NULL;
        for (int i = 0; i < fs->directoryCount; i++) {
            if (strcmp(fs->directories[i].name, token) == 0) {
                dir = &(fs->directories[i]);
                break;
            }
        }
        if (dir != NULL) {
            listFiles(dir);
        } else {
            printf("Directory not found.\n");
        }
    } else {
        printf("Unknown command.\n");
    }
}

// 메인 API 함수
void aapi_main() {
    struct BasicFileSystemLayer fs;
    initBasicFileSystemLayer(&fs);

    // 사용자로부터 명령어를 입력받아 처리하는 루프
    char command[MAX_COMMAND_LENGTH];
    while (1) {
        printf("Enter a command: ");
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = '\0'; // 개행 문자 제거
        
        handleCommand(&fs, command);
        
        if (strcmp(command, "exit") == 0) {
            break;
        }
    }
}
