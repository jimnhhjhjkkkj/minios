#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define NUM_POINTS 10000000 
int points_in_circle = 0; 

void *generate_points(void *arg) {
    int i;
    unsigned int seed = time(NULL); 

    for (i = 0; i < NUM_POINTS; i++) {
        double x = (double)rand_r(&seed) / RAND_MAX; 
        double y = (double)rand_r(&seed) / RAND_MAX;

        if (x * x + y * y <= 1.0)
            points_in_circle++;
    }

    pthread_exit(NULL);
}

void pi() {
    pthread_t tid;
    
    srand(time(NULL)); 

    if (pthread_create(&tid, NULL, generate_points, NULL) != 0) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }

    if (pthread_join(tid, NULL) != 0) {
        perror("pthread_join");
        exit(EXIT_FAILURE);
    }

    double pi = 4.0 * (double)points_in_circle / NUM_POINTS;
    printf("Estimated value of π: %lf\n", pi);

}

