#include <stdlib.h>
#include <pthread.h>
#include <sys/prctl.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>

#define LOCK_MUTEX(m)   pthread_mutex_lock(m)
#define UNLOCK_MUTEX(m) pthread_mutex_unlock(m)

typedef struct binary_semaphore {
    pthread_mutex_t mutex;  // mutex for critical sections
    pthread_cond_t cond;    // condition variable for threads to wait on
    int value;              // semaphore value (0 or 1)
} binary_semaphore, *Pbinary_semaphore;

/* Wait on a binary semaphore until its value is 1 */
void binary_semaphore_wait(Pbinary_semaphore sem);

/* Initialize a binary semaphore with a given value */
void binary_semaphore_init(binary_semaphore *sem, int value);

/* Reset the value of a binary semaphore to 0 */
void binary_semaphore_reset(Pbinary_semaphore sem);

/* Signal a binary semaphore, waking up at least one thread waiting on it */
void binary_semaphore_post(Pbinary_semaphore sem);

/* Signal a binary semaphore, waking up all threads waiting on it */
void binary_semaphore_post_all(Pbinary_semaphore sem);

void binary_semaphore_destroy(Pbinary_semaphore sem);
