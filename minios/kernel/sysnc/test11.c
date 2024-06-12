#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/times.h>
#include <unistd.h>
#include <sys/resource.h> 
#include "local_sema.h"

#define NUM_THREADS 5
#define NUM_MESSAGES 10
#define LOG_FILE "log.txt"

long clock_ticks_per_second;

void *thread_function(void *arg) {
    int thread_id = *((int *)arg);
    struct timeval start_tv, end_tv;
    struct tms start_tms, end_tms;
    struct rusage start_usage, end_usage; // 리소스 사용량 정보를 저장할 구조체
    FILE *log_file;

    for (int i = 0; i < NUM_MESSAGES; i++) {
        gettimeofday(&start_tv, NULL);
        getrusage(RUSAGE_SELF, &start_usage); // 시작 시점의 리소스 사용량 측정

        usleep(100000); // 0.1초 지연

        gettimeofday(&end_tv, NULL);
        getrusage(RUSAGE_SELF, &end_usage); // 종료 시점의 리소스 사용량 측정

        long response_time_sec = end_tv.tv_sec - start_tv.tv_sec;
        long response_time_usec = end_tv.tv_usec - start_tv.tv_usec;
        if (response_time_usec < 0) {
            response_time_usec += 1000000;
            response_time_sec -= 1;
        }

        // 사용자 시간 및 시스템 시간 계산
        long user_time = (end_usage.ru_utime.tv_sec - start_usage.ru_utime.tv_sec) * 1000000 + (end_usage.ru_utime.tv_usec - start_usage.ru_utime.tv_usec);
        long sys_time = (end_usage.ru_stime.tv_sec - start_usage.ru_stime.tv_sec) * 1000000 + (end_usage.ru_stime.tv_usec - start_usage.ru_stime.tv_usec);

        // CPU 사용 시간: 사용자 시간 + 시스템 시간
        long cpu_time = user_time + sys_time;

        // 메모리 사용량: 최대 상주 집합 크기
        long memory_usage = end_usage.ru_maxrss;

        log_file = fopen(LOG_FILE, "a");
        if (log_file == NULL) {
            perror("Error opening log file");
            exit(EXIT_FAILURE);
        }
        fprintf(log_file, "Thread %d: Message %d, Address = %p, Iteration = %d, Response Time = %ld.%06ld seconds, User Time = %ld microseconds, System Time = %ld microseconds, CPU Time = %ld microseconds, Memory Usage = %ld KB\n",
                thread_id, i, &i, i, response_time_sec, response_time_usec, user_time, sys_time, cpu_time, memory_usage);
        fclose(log_file);
    }

    pthread_exit(NULL);
}

int test11_main() {
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    clock_ticks_per_second = sysconf(_SC_CLK_TCK);

    FILE *log_file = fopen(LOG_FILE, "w");
    if (log_file != NULL) {
        fclose(log_file);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        int rc = pthread_create(&threads[i], NULL, thread_function, (void *)&thread_ids[i]);
        if (rc) {
            printf("Error:unable to create thread, %d\n", rc);
            exit(-1);
        }
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}

