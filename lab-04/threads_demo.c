#include <pthread.h>
#include <stdio.h>

#define NUM_THREADS 4

typedef struct {
    long index;
    long result;
} Job;

void *worker(void *arg) {
    Job *job = (Job *)arg;
    job->result = 0;

    for (long i = 1; i <= 10; i++) {
        job->result += i + job->index * 1000;
    }

    printf("Потік %ld: часткова сума = %ld\n", job->index, job->result);
    return NULL;
}

int main(void) {
    pthread_t threads[NUM_THREADS];
    Job jobs[NUM_THREADS];

    for (long i = 0; i < NUM_THREADS; i++) {
        jobs[i].index = i;
        jobs[i].result = 0;

        if (pthread_create(&threads[i], NULL, worker, &jobs[i]) != 0) {
            perror("pthread_create");
            return 1;
        }
    }

    long total = 0;

    for (long i = 0; i < NUM_THREADS; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("pthread_join");
            return 1;
        }

        total += jobs[i].result;
    }

    printf("Загальна сума: %ld\n", total);
    return 0;
}
