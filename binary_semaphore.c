#include "binary_semaphore.h"

/* Initialize a binary semaphore with a given value */
void binary_semaphore_init(binary_semaphore *sem, int value)
{
    if (value < 0 || value > 1) {
        fprintf(stderr, "Error: semaphore value must be either 0 or 1\n");
        exit(1);
    }
    pthread_mutex_init(&(sem->mutex), NULL);
    pthread_cond_init(&(sem->cond), NULL);
    sem->value = value;
}

/* Reset the value of a binary semaphore to 0 */
void binary_semaphore_reset(Pbinary_semaphore sem) {
    binary_semaphore_init(sem, 0);
}

/* Signal a binary semaphore, waking up at least one thread waiting on it */
void binary_semaphore_post(Pbinary_semaphore sem) {
    LOCK_MUTEX(&sem->mutex);
    sem->value = 1;
    pthread_cond_signal(&sem->cond);
    UNLOCK_MUTEX(&sem->mutex);
}

/* Signal a binary semaphore, waking up all threads waiting on it */
void binary_semaphore_post_all(Pbinary_semaphore sem) {
    LOCK_MUTEX(&sem->mutex);
    sem->value = 1;
    pthread_cond_broadcast(&sem->cond);
    UNLOCK_MUTEX(&sem->mutex);
}

/* Wait on a binary semaphore until its value is 0 */
void binary_semaphore_wait(Pbinary_semaphore sem) {
    LOCK_MUTEX(&sem->mutex);
    while (sem->value != 1) {
        pthread_cond_wait(&sem->cond, &sem->mutex);
    }
    sem->value = 0;
    UNLOCK_MUTEX(&sem->mutex);
}

void binary_semaphore_destroy(Pbinary_semaphore sem) {
    int mutex_destroy_result = pthread_mutex_destroy(&sem->mutex);
    if (mutex_destroy_result != 0) {
        fprintf(stderr, "Error: failed to destroy mutex\n");
        exit(1);
    }

    int cond_destroy_result = pthread_cond_destroy(&sem->cond);
    if (cond_destroy_result != 0) {
        fprintf(stderr, "Error: failed to destroy condition variable\n");
        exit(1);
    }
}