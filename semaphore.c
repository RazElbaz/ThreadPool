#include "semaphore.h"

// Initializes a semaphore with a given value
void semaphore_init(semaphore *PSemaphore, int value) {
    // Check if semaphore pointer is NULL
    if (PSemaphore == NULL) {
        fprintf(stderr, "Error: semaphore pointer is NULL\n");
        exit(1);
    }
    
    // Check if the value is either 0 or 1
    if (value != 0 && value != 1) {
        fprintf(stderr, "Error: semaphore value must be either 0 or 1\n");
        exit(1);
    }
    
    // Initialize mutex and check for errors
    if (pthread_mutex_init(&(PSemaphore->mutex), NULL) != 0) {
        fprintf(stderr, "Error: failed to initialize mutex\n");
        exit(1);
    }
    
    // Initialize condition variable and check for errors
    if (pthread_cond_init(&(PSemaphore->cond), NULL) != 0) {
        fprintf(stderr, "Error: failed to initialize condition variable\n");
        exit(1);
    }
    
    // Set semaphore value
    PSemaphore->value = value;
    
    // Initialize count to 0
    PSemaphore->count = 0;
}

// Resets a semaphore to 0
void semaphore_reset(Psemaphore PSemaphore) {
    // Check if semaphore pointer is NULL
    if (PSemaphore == NULL) {
        fprintf(stderr, "Error: semaphore pointer is NULL\n");
        exit(1);
    }
    // Reset semaphore to 0
    semaphore_init(PSemaphore, 0);
}


// Signals the semaphore
void release_semaphore(Psemaphore PSemaphore) {
    // Check if semaphore pointer is NULL
    if (PSemaphore == NULL) {
        fprintf(stderr, "Error: semaphore pointer is NULL\n");
        exit(1);
    }
    
    // Lock the mutex to ensure exclusive access to shared data
    LOCK_MUTEX(&PSemaphore->mutex);
    
    // Set semaphore value to 1
    PSemaphore->value = 1;
    
    // Signal one waiting thread on the condition variable
    // Check if there are any waiting threads
    if (PSemaphore->count > 0) {
        // Signal the condition variable to wake up a waiting thread
        int signal_result = pthread_cond_signal(&PSemaphore->cond);
        if (signal_result != 0) {
            fprintf(stderr, "Error: failed to signal condition variable\n");
            exit(1);
        }
    }
    
    // Unlock the mutex to allow other threads to access the shared data
    UNLOCK_MUTEX(&PSemaphore->mutex);
}


// Signals all waiting threads on the semaphore
void release_all_semaphores(Psemaphore PSemaphore) {
    // Check if semaphore pointer is NULL
    if (PSemaphore == NULL) {
        fprintf(stderr, "Error: semaphore pointer is NULL\n");
        exit(1);
    }
    
    // Lock the mutex to ensure exclusive access to shared data
    LOCK_MUTEX(&PSemaphore->mutex);
    
    // Set semaphore value to 1
    PSemaphore->value = 1;
    
    // Signal all waiting threads on the condition variable
    int broadcast_result = pthread_cond_broadcast(&PSemaphore->cond);
    if (broadcast_result != 0) {
        fprintf(stderr, "Error: failed to broadcast condition variable\n");
        exit(1);
    }
    
    // Unlock the mutex to allow other threads to access the shared data
    UNLOCK_MUTEX(&PSemaphore->mutex);
}


// Waits on the semaphore until its value is 1
void semaphore_wait(Psemaphore PSemaphore) {
    // Check if semaphore pointer is NULL
    if (PSemaphore == NULL) {
        fprintf(stderr, "Error: semaphore pointer is NULL\n");
        exit(1);
    }
    
    // Lock the mutex to ensure exclusive access to shared data
    LOCK_MUTEX(&PSemaphore->mutex);
    
    // Increment the count of waiting threads
    PSemaphore->count++;
    
    // Wait on the condition variable until the semaphore value is 1
    while (PSemaphore->value != 1) {
        int wait_result = pthread_cond_wait(&PSemaphore->cond, &PSemaphore->mutex);
        if (wait_result != 0) {
            fprintf(stderr, "Error: failed to wait on condition variable\n");
            exit(1);
        }
    }
    
    // Decrease the count of waiting threads
    PSemaphore->count--;
    
    // Set semaphore value to 0
    PSemaphore->value = 0;
    
    // Unlock the mutex to allow other threads to access the shared data
    UNLOCK_MUTEX(&PSemaphore->mutex);
}


// Function to decrement the value of a semaphore without waiting
int semaphore_try_decrement(Psemaphore PSemaphore) {
    LOCK_MUTEX(&(PSemaphore->mutex));
    if (PSemaphore->value > 0) {
        PSemaphore->value--;
        UNLOCK_MUTEX(&(PSemaphore->mutex));
        return 0;  // Successfully decremented the semaphore
    } else {
        UNLOCK_MUTEX(&(PSemaphore->mutex));
        return -1; // Semaphore value was already 0, unable to decrement
    }
}
// Function to get the current value of the semaphore
int semaphore_get_value(Psemaphore PSemaphore) {
    LOCK_MUTEX(&(PSemaphore->mutex));
    int value = PSemaphore->value;
    UNLOCK_MUTEX(&(PSemaphore->mutex));
    return value;
}

// Destroys the semaphore and frees any associated resources
void semaphore_destroy(Psemaphore PSemaphore) {
    // Check if semaphore pointer is NULL
    if (PSemaphore == NULL) {
        fprintf(stderr, "Error: semaphore pointer is NULL\n");
        exit(1);
    }

    // Destroy the mutex and check for errors
    int mutex_destroy_result = pthread_mutex_destroy(&PSemaphore->mutex);
    if (mutex_destroy_result != 0) {
        fprintf(stderr, "Error: failed to destroy condition variable\n");
        exit(1);
    }

}


