#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "api2.h"

void api_main()
{

   

    int dir_count = 0;
    int file_count = 0;

    char *input;
    char input_makedir = "";
    char *input_makefile = "";

    while (1)
    {
        // readline을 사용하여 입력 받기
        input = readline("하실 것: ");

        if (strcmp(input, "exit") == 0)
        {
            break;
        }

        if (strcmp(input, "makedir") == 0)
        {
            printf("create directory\n");
            // 디렉토리 생성하기
            char *input_makedir;
            input_makedir = readline("디렉토리 이름: ");
            if (createDirectory(input_makedir) == 0)
            {
                printf("Directory created successfully.\n");
                dir_count++;
            }
            else
            {
                printf("Failed to create directory.\n");
            }
        }

        else if (strcmp(input, "makefile") == 0)
        {
    		
            if (dir_count == 0)
            {
                printf("create file failed\n");
                break;
            }
            // 파일 생성하기
            printf("create file\n");
            char *input_makefile;
            char *input_makefile_content;

            input_makefile = readline("파일 이름: ");
            input_makefile_content = readline("파일 내용: ");
            if (createFile(input_makedir, input_makefile, input_makefile_content) == 0)
            {
                printf("File created successfully.\n");
                file_count++;
            }
            else
            {
                printf("Failed to create file.\n");
            }
        }

       /* else if (strcmp(input, "readfile") == 0)
        {
            char *wantdir = "";
            char *wantfile = "";

            wantdir = readline("찾으시는 디렉토리: ");
            wantfile = readline("찾으시는 파일: ");

            char *content = readFile( wantdir, wantfile);
            if (content)
            {
                printf("File content: %s\n", content);
            }
            else
            {
                printf("File not found.\n");
            }
        }

        else
        {
            system(input);
        }
    }

    // 파일 읽기
    char *content = readFile( "root", "test.txt");
    if (content)
    {
        printf("File content: %s\n", content);
    }
    else
    {
        printf("File not found.\n");
    }

    // 파일 수정하기
    if (updateFile( root", "test.txt", "Updated content!") == 0)
    {
        printf("File updated successfully.\n");
    }
    else
    {
        printf("Failed to update file.\n");
    }

    // 파일 목록 조회하기
    listFiles( "root");

    // 파일 크기 반환하기
    int size = getFileSize( "root", "test.txt");
    if (size >= 0)
    {
        printf("File size: %d\n", size);
    }
    else
    {
        printf("Failed to get file size.\n");
    }

    // 파일 검색하기
    searchFiles("root", "Updated");

    // 파일 복사하기
    int copyResult = copyFile( "root", "test.txt", "root", "test_copy.txt");
    if (copyResult == 0)
    {
        printf("File copied successfully.\n");
    }
    else
    {
        printf("Failed to copy file. Error code: %d\n", copyResult);
    }

    // 파일 삭제하기
    if (deleteFile("root", "test.txt") == 0)
    {
        printf("File deleted successfully.\n");
    }
    else
    {
        printf("Failed to delete file.\n");
    }*/
}}

