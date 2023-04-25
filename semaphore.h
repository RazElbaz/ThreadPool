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
typedef struct semaphore 
{
	pthread_mutex_t mutex;
	pthread_cond_t   cond;
	int value;
} semaphore,*Psemaphore;

void semaphore_init(semaphore *Psem, int value);


void semaphore_reset(Psemaphore Psem);


void semaphore_post(Psemaphore Psem);

void semaphore_post_all(Psemaphore Psem);



void semaphore_wait(Psemaphore Psem);

#endif