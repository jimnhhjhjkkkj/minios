#include <pthread.h>
#include <errno.h>
#include <stdio.h>
#include <time.h>

// 뮤텍스 구조체 정의
typedef struct {
    pthread_mutex_t lock;
    pthread_mutexattr_t attr;
} local_mutex_t;

// 뮤텍스 초기화 함수
int local_mutex_init(local_mutex_t *mutex) {
    int ret;

    // 재진입 가능한 뮤텍스 속성 설정
    if (pthread_mutexattr_init(&mutex->attr) != 0) {
        return errno;
    }
    if (pthread_mutexattr_settype(&mutex->attr, PTHREAD_MUTEX_RECURSIVE) != 0) {
        return errno;
    }

    // 뮤텍스 초기화
    ret = pthread_mutex_init(&mutex->lock, &mutex->attr);
    if (ret != 0) {
        return ret;
    }

    return 0;
}

// 뮤텍스 잠금 함수
int local_mutex_lock(local_mutex_t *mutex) {
    return pthread_mutex_lock(&mutex->lock);
}

// 뮤텍스 잠금 해제 함수
int local_mutex_unlock(local_mutex_t *mutex) {
    return pthread_mutex_unlock(&mutex->lock);
}

// 뮤텍스 제거 함수
int local_mutex_destroy(local_mutex_t *mutex) {
    int ret;

    ret = pthread_mutex_destroy(&mutex->lock);
    if (ret != 0) {
        return ret;
    }

    ret = pthread_mutexattr_destroy(&mutex->attr);
    if (ret != 0) {
        return ret;
    }

    return 0;
}

// 타임아웃을 이용한 뮤텍스 잠금 시도 함수
int local_mutex_trylock_with_timeout(local_mutex_t *mutex, long timeout_ns) {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);

    // 현재 시간에 타임아웃 시간을 더한다.
    ts.tv_nsec += timeout_ns;
    while (ts.tv_nsec >= 1000000000L) {
        ts.tv_nsec -= 1000000000L;
        ts.tv_sec += 1;
    }

    return pthread_mutex_timedlock(&mutex->lock, &ts);
}

int mutex() {
    local_mutex_t mutex;

    // 뮤텍스 초기화
    if (local_mutex_init(&mutex) != 0) {
        fprintf(stderr, "Mutex initialization failed.\n");
        return 1;
    }

    // 뮤텍스 잠금 시도
    if (local_mutex_trylock_with_timeout(&mutex, 5000000000) == 0) { // 5초 타임아웃
        printf("Mutex locked.\n");

        // 뮤텍스 작업 수행...

        // 뮤텍스 잠금 해제
        local_mutex_unlock(&mutex);
    } else {
        printf("Mutex lock failed due to timeout.\n");
    }

    // 뮤텍스 제거
    local_mutex_destroy(&mutex);

    return 0;
}
