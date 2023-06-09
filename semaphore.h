#include <stdlib.h>
#include <pthread.h>
#include <sys/prctl.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>


#define LOCK_MUTEX(m)   pthread_mutex_lock(m)
#define UNLOCK_MUTEX(m) pthread_mutex_unlock(m)

// Define a struct for a semaphore, containing a mutex and condition variable
typedef struct semaphore {
    int value;                // The value of the semaphore
    int count;                // Number of threads waiting on the semaphore
    pthread_cond_t cond;      // Condition variable for thread synchronization
    pthread_mutex_t mutex;    // Mutex for protecting shared data
    
} semaphore, *Psemaphore;

// Function to initialize a semaphore with a given value
void semaphore_init(semaphore *PSemaphore, int value);

// Function to increment the value of a semaphore (i.e., signal)
void semaphore_reset(Psemaphore PSemaphore);

// This function signals the semaphore, allowing one waiting thread to proceed. 
void release_semaphore(Psemaphore PSemaphore);

// This function signals the semaphore, allowing all waiting threads to proceed. 
void release_all_semaphores(Psemaphore PSemaphore);

// Function to get the current value of the semaphore
int semaphore_get_value(Psemaphore PSemaphore);

// Function to decrement the value of a semaphore without waiting
int semaphore_try_decrement(Psemaphore PSemaphore);

// Function to decrement the value of a semaphore and wait if the value is currently 0 (i.e. wait)
void semaphore_wait(Psemaphore PSemaphore);

// Function to destroy the semaphore
void semaphore_destroy(Psemaphore PSemaphore);


