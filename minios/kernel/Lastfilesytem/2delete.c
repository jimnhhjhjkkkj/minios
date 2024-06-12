#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct File {
    char name[100]; // 파일 이름
    char content[256]; // 파일 내용
} File;

typedef struct Directory {
    char name[100]; // 디렉터리 이름
    void* children[10]; // 자식 노드 포인터 배열 (디렉터리 또는 파일), 간단한 예제를 위해 최대 10개로 제한
    int childCount; // 현재 자식 노드의 수
} Directory;

typedef enum { DIRECTORY, FILE_TYPE } NodeType;

typedef struct Node {
    NodeType type; // 노드 타입 (디렉터리 또는 파일)
    union {
        Directory dir;
        File file;
    };
    struct Node* parent; // 부모 노드 포인터
} Node;

Node* createNode(const char* name, NodeType type, Node* parent) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->type = type;
    newNode->parent = parent;
    if (type == DIRECTORY) {
        strcpy(newNode->dir.name, name);
        newNode->dir.childCount = 0;
    } else {
        strcpy(newNode->file.name, name);
        memset(newNode->file.content, 0, sizeof(newNode->file.content)); // 파일 내용 초기화
    }
    return newNode;
}

void addChild(Node* parent, Node* child) {
    if (parent->dir.childCount < 10) {
        parent->dir.children[parent->dir.childCount++] = child;
    } else {
        printf("자식 노드의 최대 개수를 초과했습니다.\n");
    }
}

void printTree(Node* node, int level) {
    for (int i = 0; i < level; i++) {
        printf("  ");
    }
    if (node->type == DIRECTORY) {
        printf("%s/\n", node->dir.name);
    } else {
        printf("%s\n", node->file.name);
    }
    if (node->type == DIRECTORY) {
        for (int i = 0; i < node->dir.childCount; i++) {
            printTree((Node*)node->dir.children[i], level + 1);
        }
    }
}

void freeTree(Node* node) {
    if (node->type == DIRECTORY) {
        for (int i = 0; i < node->dir.childCount; i++) {
            freeTree((Node*)node->dir.children[i]);
        }
    }
    free(node);
}

Node* findNode(Node* node, const char* name, NodeType type) {
    if (node->type == type && strcmp(node->dir.name, name) == 0) {
        return node;
    }
    if (node->type == DIRECTORY) {
        for (int i = 0; i < node->dir.childCount; i++) {
            Node* found = findNode((Node*)node->dir.children[i], name, type);
            if (found != NULL) {
                return found;
            }
        }
    }
    return NULL;
}

void readfile(Node* node, const char* name) {
    bool found = false;
    if (node->type == FILE_TYPE) {
        if (strcmp(node->file.name, name) == 0) {
            printf("파일의 부모 디렉토리: %s ", node->parent->dir.name);
            printf("파일 내용: %s\n", node->file.content);
            found = true;
        }
    } else if (node->type == DIRECTORY) {
        for (int i = 0; i < node->dir.childCount; i++) {
            Node* child = (Node*)node->dir.children[i];
            if (child->type == FILE_TYPE && strcmp(child->file.name, name) == 0) {
                printf("파일의 부모 디렉토리: %s ", node->dir.name);
                printf("파일 내용: %s\n", child->file.content);
                found = true;
            } else if (child->type == DIRECTORY) {
                readfile(child, name); // 재귀적으로 탐색
            }
        }
    }
    if (!found && node->parent == NULL) { // 최상위 노드에서 파일을 찾지 못했으면
        printf("'%s' 파일을 찾을 수 없습니다.\n", name);
    }
}

void updatefile(Node* parent, const char* name, const char* newContent) {
    if (parent->type != DIRECTORY) {
        printf("'%s'는 디렉터리가 아닙니다.\n", parent->dir.name);
        return;
    }

    bool found = false;
    for (int i = 0; i < parent->dir.childCount; i++) {
        Node* child = (Node*)parent->dir.children[i];
        if (child->type == FILE_TYPE && strcmp(child->file.name, name) == 0) {
            strcpy(child->file.content, newContent);
            printf("파일 '%s'의 내용이 업데이트 되었습니다.\n", name);
            found = true;
            break;
        }
    }
    if (!found) {
        printf("'%s' 파일을 찾을 수 없습니다.\n", name);
    }
}


void searchfile(Node* node, const char* keyword) {
    if (node->type == FILE_TYPE) {
        if (strstr(node->file.content, keyword) != NULL) { // 파일 내용에 키워드가 포함되어 있는지 확인
            printf("키워드 '%s'를 포함하는 파일: %s\n", keyword, node->file.name);
            printf("해당 파일의 부모 디렉토리 : %s\n", node->parent->dir.name);
        }
    } else if (node->type == DIRECTORY) {
        for (int i = 0; i < node->dir.childCount; i++) {
            searchfile((Node*)node->dir.children[i], keyword); // 디렉터리의 자식 노드에 대해 재귀적으로 탐색
        }
    }
}

int hasChildWithName(Node* parent, const char* name, int type) {
    if (parent->type != DIRECTORY) {
        return 0; // 부모가 디렉터리가 아니면 항상 0을 반환
    }
    for (int i = 0; i < parent->dir.childCount; i++) {
        Node* child = (Node*)parent->dir.children[i];
        if (type == DIRECTORY && child->type == DIRECTORY && strcmp(child->dir.name, name) == 0) {
            return 1; // 동일한 이름의 디렉터리 발견
        } else if (type == FILE_TYPE && child->type == FILE_TYPE && strcmp(child->file.name, name) == 0) {
            return 1; // 동일한 이름의 파일 발견
        }
    }
    return 0; // 동일한 이름의 자식 노드 없음
}

void renameNode(Node* parent, const char* oldName, const char* newName, NodeType type) {
    if (parent->type != DIRECTORY) {
        printf("'%s'는 디렉터리가 아닙니다.\n", parent->dir.name);
        return;
    }
    // 같은 이름을 가진 자식 노드가 있는지 확인
    if (hasChildWithName(parent, newName, type)) {
        printf("'%s' 이름을 가진 %s가 이미 존재합니다.\n", newName, type == DIRECTORY ? "디렉터리" : "파일");
        return;
    }

    bool found = false;
    for (int i = 0; i < parent->dir.childCount; i++) {
        Node* child = (Node*)parent->dir.children[i];
        if (child->type == type && strcmp(type == DIRECTORY ? child->dir.name : child->file.name, oldName) == 0) {
            // 이름 변경
            strcpy(type == DIRECTORY ? child->dir.name : child->file.name, newName);
            printf("'%s'의 이름이 '%s'(으)로 변경되었습니다.\n", oldName, newName);
            found = true;
            break;
        }
    }
    if (!found) {
        printf("'%s'를 찾을 수 없습니다.\n", oldName);
    }
}

void deleteNode(Node* parent, const char* name, NodeType type) {
    if (parent->type != DIRECTORY) {
        printf("'%s'는 디렉터리가 아닙니다.\n", parent->dir.name);
        return;
    }

    for (int i = 0; i < parent->dir.childCount; i++) {
        Node* child = (Node*)parent->dir.children[i];
        if (child->type == type && strcmp(type == DIRECTORY ? child->dir.name : child->file.name, name) == 0) {
            // 자식 노드 삭제 처리
            freeTree(child);
            // 배열에서 삭제된 노드 제거
            for (int j = i; j < parent->dir.childCount - 1; j++) {
                parent->dir.children[j] = parent->dir.children[j + 1];
            }
            parent->dir.childCount--;
            printf("'%s' %s가 삭제되었습니다.\n", name, type == DIRECTORY ? "디렉터리" : "파일");
            return;
        }
    }

    printf("'%s' %s를 찾을 수 없습니다.\n", name, type == DIRECTORY ? "디렉터리" : "파일");
}

int main() {
    Node* root = createNode("root", DIRECTORY, NULL);
    char command[100], name[100], parentName[100], content[256];

    while (1) {
        printf("명령을 입력하세요 (makedir, makefile, readfile, updatefile, searchfile, print, delete, rename, quit): ");
        scanf("%s", command);

        if (strcmp(command, "quit") == 0) {
            break;
        } else if (strcmp(command, "makedir") == 0 || strcmp(command, "makefile") == 0) {
            printf("부모 디렉터리 이름: ");
            scanf("%s", parentName);
            Node* parentNode = findNode(root, parentName, DIRECTORY);

            if (parentNode == NULL || parentNode->type != DIRECTORY) {
                printf("'%s' 디렉터리를 찾을 수 없습니다.\n", parentName);
                continue;
            }

            printf("이름: ");
            scanf("%s", name);

            // 여기에 추가된 부분: 같은 이름의 자식 노드가 있는지 검사
            if (strcmp(command, "makedir") == 0) {
                int foundDirectory = hasChildWithName(parentNode, name, DIRECTORY);
                if (foundDirectory) {
                    printf("같은 이름의 디렉터리가 이미 존재합니다: %s\n", name);
                    continue; // 같은 이름의 노드가 있으면 명령을 건너뛴다
                }
            } else if (strcmp(command, "makefile") == 0) {
                int foundFile = hasChildWithName(parentNode, name, FILE_TYPE);
                if (foundFile) {
                    printf("같은 이름의 파일이 이미 존재합니다: %s\n", name);
                    continue; // 같은 이름의 노드가 있으면 명령을 건너뛴다
                }
            }

            if (strcmp(command, "makedir") == 0) {
                Node* newDir = createNode(name, DIRECTORY, parentNode);
                addChild(parentNode, newDir);
                printf("디렉터리 '%s'가 생성되었습니다.\n", name);
            } else { // makefile
                printf("파일 내용: ");
                scanf(" %[^\n]s", content); // 공백을 포함한 내용을 받기 위해 수정
                Node* newFile = createNode(name, FILE_TYPE, parentNode);
                strcpy(newFile->file.content, content);
                addChild(parentNode, newFile);
                printf("파일 '%s'가 생성되었습니다.\n", name);
            }
        } else if (strcmp(command, "readfile") == 0) {
            printf("부모 디렉터리 이름: ");
            scanf("%s", parentName);
            Node* parentNode = findNode(root, parentName, DIRECTORY);
            if (parentNode == NULL || parentNode->type != DIRECTORY) {
                printf("'%s' 디렉터리를 찾을 수 없습니다.\n", parentName);
                continue;
            }
            printf("파일 이름: ");
            scanf("%s", name);
            readfile(parentNode, name);
        } else if (strcmp(command, "updatefile") == 0) {
            printf("부모 디렉터리 이름: ");
            scanf("%s", parentName);
            Node* parentNode = findNode(root, parentName, DIRECTORY);
            if (parentNode == NULL || parentNode->type != DIRECTORY) {
                printf("'%s' 디렉터리를 찾을 수 없습니다.\n", parentName);
                continue;
            }
            printf("파일 이름: ");
            scanf("%s", name);
            printf("새로운 파일 내용: ");
            scanf(" %[^\n]s", content); // 공백을 포함한 내용을 받기 위해 수정
            updatefile(parentNode, name, content);
        } else if (strcmp(command, "searchfile") == 0) {
            printf("검색할 키워드: ");
            scanf(" %[^\n]s", content); // 공백을 포함한 키워드를 받기 위해 수정
            searchfile(root, content);
        } else if (strcmp(command, "print") == 0) {
            printTree(root, 0);
        } else if (strcmp(command, "rename") ==0) {
            char oldName[100];
            char newName[100];
            NodeType type;
            char typeName[10];

            printf("부모 디렉터리 이름: ");
            scanf("%s", parentName);
            Node* parentNode = findNode(root, parentName, DIRECTORY);
            if (parentNode == NULL || parentNode->type != DIRECTORY) {
                printf("'%s' 디렉터리를 찾을 수 없습니다.\n", parentName);
                continue;
            }

            printf("변경할 파일/디렉터리의 이름: ");
            scanf("%s", oldName);
            printf("새 이름: ");
            scanf("%s", newName);
            printf("타입 ('file' 또는 'dir'): ");
            scanf("%s", typeName);
            type = strcmp(typeName, "dir") == 0 ? DIRECTORY : FILE_TYPE;

            renameNode(parentNode, oldName, newName, type);
        } else if (strcmp(command, "delete") == 0) {
            NodeType type;
            char typeName[10];

            printf("부모 디렉터리 이름: ");
            scanf("%s", parentName);
            Node* parentNode = findNode(root, parentName, DIRECTORY);
            if (parentNode == NULL || parentNode->type != DIRECTORY) {
                printf("'%s' 디렉터리를 찾을 수 없습니다.\n", parentName);
                continue;
            }

            printf("삭제할 파일/디렉터리의 이름: ");
            scanf("%s", name);
            printf("타입 ('file' 또는 'dir'): ");
            scanf("%s", typeName);
            type = strcmp(typeName, "dir") == 0 ? DIRECTORY : FILE_TYPE;

            deleteNode(parentNode, name, type);
        }
        else {
            printf("알 수 없는 명령입니다.\n");
        }
    }

    printTree(root, 0);
    freeTree(root);

    return 0;
}
