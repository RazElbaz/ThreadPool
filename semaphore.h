#ifndef _SEMAPHORE_
#define _SEMAPHORE_

#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/prctl.h>

#define LOCK_MUTEX(m)   pthread_mutex_lock(m)
#define UNLOCK_MUTEX(m) pthread_mutex_unlock(m)

// Define a struct for a semaphore, containing a mutex and condition variable
typedef struct semaphore 
{
	pthread_mutex_t mutex;
	pthread_cond_t   cond;
	int value; // The value of the semaphore
} semaphore,*Psemaphore;

// Function to initialize a semaphore with a given value
void semaphore_init(semaphore *Psem, int value);

// Function to reset a semaphore to its initial value
void semaphore_reset(Psemaphore Psem);

// Function to increment the value of a semaphore (i.e. signal)
void semaphore_post(Psemaphore Psem);

// Function to increment the value of a semaphore for all waiting threads (i.e. broadcast)
void semaphore_post_all(Psemaphore Psem);

// Function to decrement the value of a semaphore and wait if the value is currently 0 (i.e. wait)
void semaphore_wait(Psemaphore Psem);

#endif
