
#define LOCK_MUTEX(m)   pthread_mutex_lock(m)
#define UNLOCK_MUTEX(m) pthread_mutex_unlock(m)
// Structure to represent a thread
typedef struct Thread {
    int id;                        
    pthread_t pthread;              
    struct ThreadPool* threadPoolPtr;    
    int status;                    

} *Pthread;

// Structure to represent a thread pool
typedef struct ThreadPool {
    Pthread* threads;                      
    TaskQueue taskQueue;                   
    int maxThreadCount;                     
} ThreadPool, *PThreadPool;
typedef struct threadpool ThreadPool;

ThreadPool *threadpool_create(int num_threads);

void threadpool_destroy(ThreadPool *pool);

void threadpool_submit(ThreadPool *pool, void (*function)(void *), void *data);


