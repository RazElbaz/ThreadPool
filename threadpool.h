
#define LOCK_MUTEX(m)   pthread_mutex_lock(m)
#define UNLOCK_MUTEX(m) pthread_mutex_unlock(m)
typedef struct threadpool ThreadPool;

ThreadPool *threadpool_create(int num_threads);

void threadpool_destroy(ThreadPool *pool);

void threadpool_submit(ThreadPool *pool, void (*function)(void *), void *data);


