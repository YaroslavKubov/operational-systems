
#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#define THREADS 4
#define MAX_ACTIVE 2

static sem_t gate;
static int active = 0;
static int max_active = 0;
static pthread_mutex_t stat_lock = PTHREAD_MUTEX_INITIALIZER;

void *worker(void *arg) {
    long id = (long)(uintptr_t)arg;

    sem_wait(&gate);

    int cur;
    pthread_mutex_lock(&stat_lock);
    active++;

    if (active > max_active)
        max_active = active;

    cur = active;
    pthread_mutex_unlock(&stat_lock);

    printf("Потік %ld увійшов, активних: %d\n", id, cur);

    usleep(50000);

    pthread_mutex_lock(&stat_lock);
    active--;
    pthread_mutex_unlock(&stat_lock);

    sem_post(&gate);
    return NULL;
}

int main(void) {
    pthread_t threads[THREADS];

    sem_init(&gate, 0, MAX_ACTIVE);

    for (long i = 0; i < THREADS; i++) {
        pthread_create(&threads[i], NULL, worker, (void *)(uintptr_t)i);
    }

    for (int i = 0; i < THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    sem_destroy(&gate);

    printf("Максимальна кількість одночасних активних: %d (межа %d)\n",
           max_active, MAX_ACTIVE);

    return 0;
}
