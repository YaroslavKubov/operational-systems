#include <pthread.h>
#include <stdio.h>

#define ARR_SIZE 100
#define NUM_THREADS 4

typedef struct {
    int start;
    int end;
    long sum;
} Slice;

static int data[ARR_SIZE];

void *sum_slice(void *arg) {
    Slice *s = (Slice *)arg;
    s->sum = 0;
    for (int i = s->start; i < s->end; i++) {
        s->sum += data[i];
    }
    return NULL;
}

int main(void) {
    for (int i = 0; i < ARR_SIZE; i++) {
        data[i] = i + 1;
    }

    pthread_t threads[NUM_THREADS];
    Slice slices[NUM_THREADS];
    int per = ARR_SIZE / NUM_THREADS;

    for (int t = 0; t < NUM_THREADS; t++) {
        slices[t].start = t * per;
        slices[t].end = (t + 1) * per;
        slices[t].sum = 0;
        if (pthread_create(&threads[t], NULL, sum_slice, &slices[t]) != 0) {
            perror("pthread_create");
            return 1;
        }
    }

    long total = 0;
    for (int t = 0; t < NUM_THREADS; t++) {
        if (pthread_join(threads[t], NULL) != 0) {
            perror("pthread_join");
            return 1;
        }
        total += slices[t].sum;
    }

    printf("Сума 1..%d = %ld\n", ARR_SIZE, total);
    return 0;
}
