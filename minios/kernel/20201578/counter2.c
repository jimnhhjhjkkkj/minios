#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_THREADS 5
#define LOG_FILE "log.txt"

sem_t log_sem;

void *thread_func1(void *arg) {
    int thread_id = *(int *)arg;
    free(arg);

    for (int i = 0; i < 10; i++) {
        // 세마포어 획득
        sem_wait(&log_sem);

        // 로그 파일에 메시지 작성
        FILE *fp = fopen(LOG_FILE, "a");
        fprintf(fp, "Thread %d: Message %d\n", thread_id, i);
        fclose(fp);

        // 세마포어 해제
        sem_post(&log_sem);

        sleep(1); // 1초 대기
    }

    return NULL;
}

int main4() {
    pthread_t threads[NUM_THREADS];
    sem_init(&log_sem, 0, 1); // 초기 값 1로 설정 (한 번에 한 스레드만 로그 파일에 접근 가능)

    // 스레드 생성
    for (int i = 0; i < NUM_THREADS; i++) {
        int *thread_id = malloc(sizeof(int));
        *thread_id = i;
        pthread_create(&threads[i], NULL, thread_func1, thread_id);
    }

    // 스레드 종료 대기
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    sem_destroy(&log_sem);

    return 0;
}

