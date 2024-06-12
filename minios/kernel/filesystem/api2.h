
#define MAX_FILES 100
#define MAX_FILENAME 256
#define MAX_CONTENT 1024
#define MAX_DIRECTORIES 100
#define MAX_DIR_NAME 256

struct File {
    char name[MAX_FILENAME];
    char content[MAX_CONTENT];
};

struct Directory {
    char name[MAX_DIR_NAME];
    struct File* files;
    int fileCount;
    int maxFiles;
};

struct FileSystem {
    struct Directory* directories;
    int dirCount;
};

    

void initFileSystem(struct FileSystem* fs) {

    fs->dirCount = 0;
    fs->directories = (struct Directory*)malloc(MAX_DIRECTORIES * sizeof(struct Directory));
    for (int i = 0; i < MAX_DIRECTORIES; i++) {
        fs->directories[i].files = NULL;
        fs->directories[i].maxFiles = 0;
    }
}

int createDirectory(char* name) {
struct FileSystem* fs;
    if (fs->dirCount >= MAX_DIRECTORIES) {
        return -1; // No space left
    }

    for (int i = 0; i < fs->dirCount; i++) {
        if (strcmp(fs->directories[i].name, name) == 0) {
            return -2; // Directory already exists
        }
    }

    struct Directory* dir = &fs->directories[fs->dirCount];
    strcpy(dir->name, name);
    dir->fileCount = 0;
    dir->maxFiles = MAX_FILES;
    dir->files = (struct File*)malloc(dir->maxFiles * sizeof(struct File));
    fs->dirCount++;
    return 0; // Success
}
int createFile( char* dirName, char* fileName, char* content) {
struct File* fs1;
    int dirIndex = -1;
    for (int i = 0; i < fs1->dirCount; i++) {
        if (strcmp(fs1->directories[i].name, dirName) == 0) {
            dirIndex = i;
            break;
        }
    }

    if (dirIndex == -1) {
        return -1; // Directory not found
    }

    struct Directory* dir = &fs1->directories[dirIndex];

    if (dir->fileCount >= MAX_FILES) {
        return -2; // No space left
    }

    for (int i = 0; i < dir->fileCount; i++) {
        if (strcmp(dir->files[i].name, fileName) == 0) {
            return -3; // File already exists
        }
    }
    strncpy(dir->files[dir->fileCount].name, fileName, MAX_FILENAME - 1);
    dir->files[dir->fileCount].name[MAX_FILENAME - 1] = '\0';
    strncpy(dir->files[dir->fileCount].content, content, MAX_CONTENT - 1);
    dir->files[dir->fileCount].content[MAX_CONTENT - 1] = '\0';
    dir->fileCount++;
    return 0; // Success
}


/*char* readFile(struct FileSystem* fs, char* dirName, char* fileName) {
    int dirIndex = -1;
    for (int i = 0; i < fs->dirCount; i++) {
        if (strcmp(fs->directories[i].name, dirName) == 0) {
            dirIndex = i;
            break;
        }
    }

    if (dirIndex == -1) {
        return NULL; // Directory not found
    }

    struct Directory* dir = &fs->directories[dirIndex];

    for (int i = 0; i < dir->fileCount; i++) {
        if (strcmp(dir->files[i].name, fileName) == 0) {
            return dir->files[i].content;
        }
    }
    return NULL; // File not found
}

int updateFile(struct FileSystem* fs, char* dirName, char* fileName, char* newContent) {
    int dirIndex = -1;
    for (int i = 0; i < fs->dirCount; i++) {
        if (strcmp(fs->directories[i].name, dirName) == 0) {
            dirIndex = i;
            break;
        }
    }

    if (dirIndex == -1) {
        return -1; // Directory not found
    }

    struct Directory* dir = &fs->directories[dirIndex];

    for (int i = 0; i < dir->fileCount; i++) {
        if (strcmp(dir->files[i].name, fileName) == 0) {
            strncpy(dir->files[i].content, newContent, MAX_CONTENT -1);
            dir ->files[i].content[MAX_CONTENT-1]='\0';
            return 0; // Success
        }
    }

    return -2; // File not found
}

int deleteFile(struct FileSystem* fs, char* dirName, char* fileName) {
    int dirIndex = -1;
    for (int i = 0; i < fs->dirCount; i++) {
        if (strcmp(fs->directories[i].name, dirName) == 0) {
            dirIndex = i;
            break;
        }
    }

    if (dirIndex == -1) {
        return -1; // Directory not found
    }

    struct Directory* dir = &fs->directories[dirIndex];

    for (int i = 0; i < dir->fileCount; i++) {
        if (strcmp(dir->files[i].name, fileName) == 0) {
            dir->files[i].name[0] = '\0'; // Set the file name to an empty string
            dir->files[i].content[0] = '\0'; // Set the file content to an empty string
            return 0; // Success
        }
    }

    return -2; // File not found
}

void listFiles(struct FileSystem* fs, char* dirName) {
    int dirIndex = -1;
    for (int i = 0; i < fs->dirCount; i++) {
        if (strcmp(fs->directories[i].name, dirName) == 0) {
            dirIndex = i;
            break;
        }
    }

    if (dirIndex == -1) {
        printf("Directory not found.\n");
        return;
    }

    struct Directory* dir = &fs->directories[dirIndex];

    printf("Files in directory '%s':\n", dirName);
    for (int i = 0; i < dir->fileCount; i++) {
        printf("%s\n", dir->files[i].name);
    }
}

int getFileSize(struct FileSystem* fs, char* dirName, char* fileName) {
    int dirIndex = -1;
    for (int i = 0; i < fs->dirCount; i++) {
        if (strcmp(fs->directories[i].name, dirName) == 0) {
            dirIndex = i;
            break;
        }
    }

    if (dirIndex == -1) {
        return -1; // Directory not found
    }

    struct Directory* dir = &fs->directories[dirIndex];

    for (int i = 0; i < dir->fileCount; i++) {
        if (strcmp(dir->files[i].name, fileName) == 0) {
            return strlen(dir->files[i].content);
        }
    }

    return -2; // File not found
}

void searchFiles(struct FileSystem* fs, char* dirName, char* keyword) {
    int dirIndex = -1;
    for (int i = 0; i < fs->dirCount; i++) {
        if (strcmp(fs->directories[i].name, dirName) == 0) {
            dirIndex = i;
            break;
        }
    }

    if (dirIndex == -1) {
        printf("Directory not found.\n");
        return;
    }

    struct Directory* dir = &fs->directories[dirIndex];
    bool found = false;

    printf("Files containing '%s' in directory '%s':\n", keyword, dirName);
    for (int i = 0; i < dir->fileCount; i++) {
        if (strstr(dir->files[i].content, keyword) != NULL) {
            printf("%s\n", dir->files[i].name);
            found = true;
        }
    }

    if (!found) {
        printf("No files found containing '%s'.\n", keyword);
    }
}

int copyFile(struct FileSystem* fs, char* srcDirName, char* srcFileName, char* destDirName, char* destFileName) {
    int srcDirIndex = -1;
    for (int i = 0; i < fs->dirCount; i++) {
        if (strcmp(fs->directories[i].name, srcDirName) == 0) {
            srcDirIndex = i;
            break;
        }
    }

    if (srcDirIndex == -1) {
    return -1; // Source directory not found
}

int destDirIndex = -1;
for (int i = 0; i < fs->dirCount; i++) {
    if (strcmp(fs->directories[i].name, destDirName) == 0) {
        destDirIndex = i;
        break;
    }
}

if (destDirIndex == -1) {
    return -2; // Destination directory not found
}

struct Directory* srcDir = &fs->directories[srcDirIndex];
struct Directory* destDir = &fs->directories[destDirIndex];

for (int i = 0; i < srcDir->fileCount; i++) {
    if (strcmp(srcDir->files[i].name, srcFileName) == 0) {
        if (destDir->fileCount >= MAX_FILES) {
            return -3; // No space left in destination directory
        }

        for (int j = 0; j < destDir->fileCount; j++) {
            if (strcmp(destDir->files[j].name, destFileName) == 0) {
                return -4; // File already exists in destination directory
            }
        }

        strncpy(destDir->files[destDir->fileCount].name, destFileName, MAX_FILENAME-1);
        destDir->files[destDir->fileCount].name[MAX_FILENAME-1]='\0';
        strncpy(destDir->files[destDir->fileCount].content, srcDir->files[i].content,MAX_CONTENT-1);
        destDir->files[destDir->fileCount].content[MAX_CONTENT-1]='\0';
        destDir->fileCount++;
        return 0; // Success
    }
}

return -5; // Source file not found
}
*/




