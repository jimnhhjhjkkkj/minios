#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "system.h"
void print_minios(char* str);

int main() {
    print_minios("[MiniOS SSU] Hello, World!");

    char *input;

    while(1) {
        // readline을 사용하여 입력 받기
        input = readline("커맨드를 입력하세요(종료:exit) : ");

        if (strcmp(input,"exit") == 0) {
            break;
        }

        if (strcmp(input,"minisystem") == 0){
            minisystem();
        }
        else if (strcmp(input,"myfunc") == 0){
            myfunc();
        }
        else if (strcmp(input,"ipc") == 0){
            ipc();
        }
        else if (strcmp(input,"pi") == 0){
            pi();
        }
        else if (strcmp(input,"rr") == 0){
            rrobin();
        }
        else if (strcmp(input,"sema") == 0){
            fork_f();
        }
        else if (strcmp(input,"counter1") == 0){
            main3();
        }
        else if (strcmp(input,"counter2") == 0){
            main4();
        }
        else if (strcmp(input,"asdf") == 0){
            main5();
        }
        else system(input);
    }

    // 메모리 해제
    free(input);
    print_minios("[MiniOS SSU] MiniOS Shutdown........");

    return(1);
}

void print_minios(char* str) {
        printf("%s\n",str);
}
