#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHILDREN 100 // 최대 자식 수

typedef struct FileSystemNode {
    char name[256];
    int isDirectory;
    union {
        struct FileSystemNode *children[MAX_CHILDREN];  // 자식 노드 배열 (디렉토리)
        char *content;  // 파일 내용
    };
    int childCount;  // 자식 노드 수 (디렉토리인 경우)
} FileSystemNode;

typedef struct {
    FileSystemNode *root;
} FileSystem;

FileSystem fs;

// 파일 시스템 초기화 함수
void initializeFileSystem() {
    fs.root = (FileSystemNode *)malloc(sizeof(FileSystemNode));
    strcpy(fs.root->name, "/");
    fs.root->isDirectory = 1;
    fs.root->childCount = 0;
}

// 경로를 기반으로 부모 디렉토리 찾기
FileSystemNode *findParentDirectory(const char *path) {
    if (strcmp(path, "/") == 0) {
        return fs.root;
    }

    FileSystemNode *current = fs.root;
    char *token;
    char tempPath[256];
    strcpy(tempPath, path);

    token = strtok(tempPath, "/");
    while (token != NULL && current != NULL) {
        int found = 0;
        for (int i = 0; i < current->childCount; i++) {
            if (strcmp(current->children[i]->name, token) == 0 && current->children[i]->isDirectory) {
                current = current->children[i];
                found = 1;
                break;
            }
        }
        if (!found) {
            return NULL;
        }
        token = strtok(NULL, "/");
    }

    return current;
}

// 경로를 기반으로 파일 노드 찾기
FileSystemNode *findFileNode(const char *path) {
    if (strcmp(path, "/") == 0) {
        return fs.root;
    }

    FileSystemNode *current = fs.root;
    char *token;
    char tempPath[256];
    strcpy(tempPath, path);

    token = strtok(tempPath, "/");
    while (token != NULL && current != NULL) {
        int found = 0;
        for (int i = 0; i < current->childCount; i++) {
            if (strcmp(current->children[i]->name, token) == 0) {
                current = current->children[i];
                found = 1;
                break;
            }
        }
        if (!found) {
            return NULL;
        }
        token = strtok(NULL, "/");
    }

    return current;
}

// 디렉토리 생성 함수
int createDirectory(const char *path) {
    // 경로의 부모 디렉토리 찾기
    char parentPath[256];
    strcpy(parentPath, path);
    char *lastSlash = strrchr(parentPath, '/');
    char newDirName[256];
    
    if (lastSlash != NULL && lastSlash != parentPath) {
        strcpy(newDirName, lastSlash + 1);
        *lastSlash = '\0';
    } else {
        strcpy(parentPath, "/");
        strcpy(newDirName, path + 1);
    }

    FileSystemNode *parent = findParentDirectory(parentPath);
    if (parent == NULL || !parent->isDirectory) {
        printf("Parent directory not found\n");
        return -1;
    }

    // 새 디렉토리 노드 생성
    FileSystemNode *newDir = (FileSystemNode *)malloc(sizeof(FileSystemNode));
    strcpy(newDir->name, newDirName);
    newDir->isDirectory = 1;
    newDir->childCount = 0;

    // 부모 디렉토리에 새 디렉토리 추가
    if (parent->childCount < MAX_CHILDREN) {
        parent->children[parent->childCount++] = newDir;
    } else {
        printf("Maximum number of children reached\n");
        free(newDir);
        return -1;
    }

    return 0;  // 성공
}

// 파일 생성 함수
int createFile(const char *path, const char *content) {
    char parentPath[256];
    strcpy(parentPath, path);
    char *lastSlash = strrchr(parentPath, '/');
    char newFileName[256];
    
    if (lastSlash != NULL && lastSlash != parentPath) {
        strcpy(newFileName, lastSlash + 1);
        *lastSlash = '\0';
    } else {
        strcpy(parentPath, "/");
        strcpy(newFileName, path + 1);
    }

    FileSystemNode *parent = findParentDirectory(parentPath);
    if (parent == NULL || !parent->isDirectory) {
        printf("Parent directory not found\n");
        return -1;
    }

    FileSystemNode *newFile = (FileSystemNode *)malloc(sizeof(FileSystemNode));
    strcpy(newFile->name, newFileName);
    newFile->isDirectory = 0; // 파일로 설정
    newFile->content = strdup(content); // 파일 내용 저장
    newFile->childCount = 0; // 파일은 자식을 가질 수 없음

    if (parent->childCount < MAX_CHILDREN) {
        parent->children[parent->childCount++] = newFile;
    } else {
        printf("Maximum number of children reached\n");
        free(newFile->content);
        free(newFile);
        return -1;
    }

    return 0;  // 성공
}

// 파일 내용 출력 함수
void printFileContent(const char *path) {
    FileSystemNode *fileNode = findFileNode(path);
    if (fileNode == NULL || fileNode->isDirectory) {
        printf("File not found or path is a directory\n");
        return;
    }

    printf("Content of %s:\n%s\n", path, fileNode->content);
}

// 파일 시스템 트리 구조 출력 함수
void printFileSystem(FileSystemNode *node, int depth) {
    for (int i = 0; i < depth; i++) {
        printf("  ");
    }
    printf("%s%s\n", node->name, node->isDirectory ? "/" : "");
    for (int i = 0; i < node->childCount; i++) {
        printFileSystem(node->children[i], depth + 1);
    }
}

// 파일 내용 수정 함수
int updateFileContent(const char *path, const char *newContent) {
    FileSystemNode *fileNode = findFileNode(path);
    if (fileNode == NULL || fileNode->isDirectory) {
        printf("File not found or path is a directory\n");
        return -1;
    }

    // 이전 내용 해제
    free(fileNode->content);

    // 새로운 내용 할당
    fileNode->content = strdup(newContent);

    return 0;  // 성공
}



// 초기화 함수
void initialize() {
    initializeFileSystem();
}

// 주요 함수들을 사용하는 예시
int main() {
    initialize();

    // 루트 디렉토리에 서브 디렉토리 생성 예제
    if (createDirectory("/home") == 0) {
        printf("Directory /home created successfully\n");
    } else {
        printf("Failed to create directory /home\n");
    }

    if (createDirectory("/home/user") == 0) {
        printf("Directory /home/user created successfully\n");
    } else {
        printf("Failed to create directory /home/user\n");
    }

    if (createDirectory("/home/user") == 0) {
        printf("Directory /home/user created successfully\n");
    } else {
        printf("Failed to create directory /home/user\n");
    }

    if (createDirectory("/home/user/docs") == 0) {
        printf("Directory /home/user/docs created successfully\n");
    } else {
        printf("Failed to create directory /home/user/docs\n");
    }

    // 파일 생성 예제
    if (createFile("/home/user/docs/file1.txt", "This is the content of file1.") == 0) {
        printf("File /home/user/docs/file1.txt created successfully\n");
    } else {
        printf("Failed to create file /home/user/docs/file1.txt\n");
    }

    // 파일 시스템 트리 구조 출력
    printf("\nFile System Structure:\n");
    printFileSystem(fs.root, 0);

    // 파일 내용 출력
    printf("\nReading file content:\n");
    printFileContent("/home/user/docs/file1.txt");

    // 파일 내용 수정
    if (updateFileContent("/home/user/docs/file1.txt", "This is the modified content of file1.") == 0) {
        printf("\nFile content modified successfully\n");
    } else {
        printf("\nFailed to modify file content\n");
    }

    // 수정된 파일 내용 출력
    printf("\nReading modified file content:\n");
    printFileContent("/home/user/docs/file1.txt");

    return 0;
}

