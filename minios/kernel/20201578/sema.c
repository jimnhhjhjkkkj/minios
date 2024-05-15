#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <time.h>
#include <stdlib.h>

#define BUFFER_SIZE 10

sem_t semaphore[3]; // 전역 변수로 세마포어 배열 선언
int shared_variable[3] = {0}; // 공유 변수 배열
int buffer[BUFFER_SIZE]; // 버퍼

void produce(int resource) {
    printf("Child %d produced data: %d\n", getpid(), resource);
    buffer[resource] = resource;
}

void consume() {
    printf("Child %d consumed data: %d\n", getpid(), buffer[0]);
}

void fork_f() {
    int num_processes;
    fprintf(stderr, "프로세스 생성 개수 :\n");
    scanf("%d", &num_processes); 

    pid_t pid;
    time_t start_time[num_processes];
    time_t end_time[num_processes];

    printf("Parent pid: %d\n", getpid());

    // 세마포어 초기화
    for (int i = 0; i < 3; i++) {
        sem_init(&semaphore[i], 0, 3); // 자원 개수를 3개로 설정
    }

    for (int i = 0; i < num_processes; i++) {
        pid = fork();
        if (pid < 0) {
            fprintf(stderr, "Fork Failed\n");
        }
        else if (pid == 0) {
            int resource1 = i % 3; // 프로세스마다 다른 자원을 요청하도록 설정
            int resource2 = (i + 1) % 3;
            printf("Child %d pid: %d, start time: %ld\n", i+1, getpid(), time(NULL));
            start_time[i] = time(NULL);

            // 우선순위를 부여하여 자원을 할당받음
            if (i % 2 == 0) {
                sem_wait(&semaphore[resource1]);
                printf("Child %d acquired resource %d\n", i+1, resource1);
                sem_wait(&semaphore[resource2]);
                printf("Child %d acquired resource %d\n", i+1, resource2);
            } else {
                sem_wait(&semaphore[resource2]);
                printf("Child %d acquired resource %d\n", i+1, resource2);
                sem_wait(&semaphore[resource1]);
                printf("Child %d acquired resource %d\n", i+1, resource1);
            }

            // 임계 구역
            produce(resource1); // 데이터를 생산하여 버퍼에 씀
            consume(); // 버퍼에서 데이터를 소비

            // 임계 구역 종료

            // 자원 반납
            sem_post(&semaphore[resource1]);
            printf("Child %d released resource %d\n", i+1, resource1);
            sem_post(&semaphore[resource2]);
            printf("Child %d released resource %d\n", i+1, resource2);

            printf("Child %d pid: %d, end time: %ld\n", i+1, getpid(), time(NULL));
            end_time[i] = time(NULL);
            printf("Child %d 실행 시간: %ld초\n", i+1, end_time[i] - start_time[i]);
            return;
        }
    }

    // 부모 프로세스는 모든 자식 프로세스의 종료를 기다림
    for (int i = 0; i < num_processes; i++) {
        wait(NULL);
        printf("Child %d Complete\n", i+1);
    }

    // 세마포어 삭제
    for (int i = 0; i < 3; i++) {
        sem_destroy(&semaphore[i]);
    }
}


