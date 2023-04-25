#include "semaphore.h"

// Initializes a semaphore with a given value
void semaphore_init(semaphore *Psem, int value)
{
    // Check if semaphore pointer is NULL
    if (Psem == NULL) {
        fprintf(stderr, "Error: semaphore pointer is NULL\n");
        exit(1);
    }
    // Check if the value is either 0 or 1
    if (value != 0 && value != 1) {
        fprintf(stderr, "Error: semaphore value must be either 0 or 1\n");
        exit(1);
    }
    // Initialize mutex and condition variable
    pthread_mutex_init(&(Psem->mutex), NULL);
    pthread_cond_init(&(Psem->cond), NULL);
    // Set semaphore value
    Psem->value = value;
}

// Resets a semaphore to 0
void semaphore_reset(Psemaphore Psem) {
    // Check if semaphore pointer is NULL
    if (Psem == NULL) {
        fprintf(stderr, "Error: semaphore pointer is NULL\n");
        exit(1);
    }
    // Reset semaphore to 0
    semaphore_init(Psem, 0);
}

// Signals the semaphore
void semaphore_post(Psemaphore Psem) {
    // Check if semaphore pointer is NULL
    if (Psem == NULL) {
        fprintf(stderr, "Error: semaphore pointer is NULL\n");
        exit(1);
    }
    // Lock the mutex
    LOCK_MUTEX(&Psem->mutex);
    // Set semaphore value to 1
    Psem->value = 1;
    // Signal one waiting thread on the condition variable
    pthread_cond_signal(&Psem->cond);
    // Unlock the mutex
    UNLOCK_MUTEX(&Psem->mutex);
}

// Signals all waiting threads on the semaphore
void semaphore_post_all(Psemaphore Psem) {
    // Check if semaphore pointer is NULL
    if (Psem == NULL) {
        fprintf(stderr, "Error: semaphore pointer is NULL\n");
        exit(1);
    }
    // Lock the mutex
    LOCK_MUTEX(&Psem->mutex);
    // Set semaphore value to 1
    Psem->value = 1;
    // Signal all waiting threads on the condition variable
    pthread_cond_broadcast(&Psem->cond);
    // Unlock the mutex
    UNLOCK_MUTEX(&Psem->mutex);
}

// Waits on the semaphore until its value is 1
void semaphore_wait(Psemaphore Psem) {
    // Check if semaphore pointer is NULL
    if (Psem == NULL) {
        fprintf(stderr, "Error: semaphore pointer is NULL\n");
        exit(1);
    }
    // Lock the mutex
    LOCK_MUTEX(&Psem->mutex);
    // Wait on the condition variable until semaphore value is 1
    while (Psem->value != 1) {
        pthread_cond_wait(&Psem->cond, &Psem->mutex);
    }
    // Set semaphore value to 0
    Psem->value = 0;
    // Unlock the mutex
    UNLOCK_MUTEX(&Psem->mutex);
}

// Destroys the semaphore and frees any associated resources
void semaphore_destroy(Psemaphore Psem) {
    // Check if semaphore pointer is NULL
    if (Psem == NULL) {
        fprintf(stderr, "Error: semaphore pointer is NULL\n");
        exit(1);
    }
    // Destroy the mutex and check for errors
    int mutex_destroy_result = pthread_mutex_destroy(&Psem->mutex);
    if (mutex_destroy_result != 0) {
        fprintf(stderr, "Error: failed to destroy condition variable\n");
        exit(1);
    }
}