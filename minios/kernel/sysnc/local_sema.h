// sema.h
#ifndef SEMA_H
#define SEMA_H

#include <pthread.h>

typedef struct {
    int value;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} semaphore;

void sem_init(semaphore *sem, int value);
void sem_wait(semaphore *sem);
void sem_post(semaphore *sem);
void sem_destroy(semaphore *sem);

#endif // SEMA_H

