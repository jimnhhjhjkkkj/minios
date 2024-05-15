#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 5
#define LOG_FILE "log.txt"

pthread_mutex_t log_mutex;

void *thread_func(void *arg) {
    int thread_id = *(int *)arg;
    free(arg);

    for (int i = 0; i < 10; i++) {
        // 뮤텍스 잠금
        pthread_mutex_lock(&log_mutex);

        // 로그 파일에 메시지 작성
        FILE *fp = fopen(LOG_FILE, "a");
        fprintf(fp, "Thread %d: Message %d\n", thread_id, i);
        fclose(fp);

        // 뮤텍스 해제
        pthread_mutex_unlock(&log_mutex);

        sleep(1); // 1초 대기
    }

    return NULL;
}

int main3() {
    pthread_t threads[NUM_THREADS];
    pthread_mutex_init(&log_mutex, NULL);

    // 스레드 생성
    for (int i = 0; i < NUM_THREADS; i++) {
        int *thread_id = malloc(sizeof(int));
        *thread_id = i;
        pthread_create(&threads[i], NULL, thread_func, thread_id);
    }

    // 스레드 종료 대기
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&log_mutex);

    return 0;
}

