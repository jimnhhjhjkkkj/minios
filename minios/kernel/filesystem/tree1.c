#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHILDREN 100 // 최대 자식 수

typedef struct FileSystemNode {
    char name[256];
    int isDirectory;
    struct FileSystemNode *children[MAX_CHILDREN];  // 자식 노드 배열
    int childCount;  // 자식 노드 수
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
    char tempPath[256];
    strcpy(tempPath, path);
    char *token = strtok(tempPath, "/");
    
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

// 디렉토리 존재 확인 함수
FileSystemNode *findDirectory(const char *path) {
    if (strcmp(path, "/") == 0) {
        return fs.root;
    }

    FileSystemNode *current = fs.root;
    char tempPath[256];
    strcpy(tempPath, path);
    char *token = strtok(tempPath, "/");

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

// 파일 시스템 트리 구조 출력 함수
void printFileSystem(FileSystemNode *node, int depth) {
    for (int i = 0; i < depth; i++) {
        printf("  ");
    }
    printf("%s\n", node->name);
    for (int i = 0; i < node->childCount; i++) {
        printFileSystem(node->children[i], depth + 1);
    }
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

    if (createDirectory("/home/user/docs") == 0) {
        printf("Directory /home/user/docs created successfully\n");
    } else {
        printf("Failed to create directory /home/user/docs\n");
    }

    // 파일 시스템 트리 구조 출력
    printf("\nFile System Structure:\n");
    printFileSystem(fs.root, 0);

    return 0;
}

