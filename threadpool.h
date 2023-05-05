#include "task.h"

// Structure to represent a thread
typedef struct Thread {
    int id;                          // Thread ID
    pthread_t pthread;                // Pointer to the thread object
    struct ThreadPool* threadPoolPtr;      // Pointer to the thread pool
    int status;                      // Additional member variable for thread status

} *Pthread;

// Structure to represent a thread pool
typedef struct ThreadPool {
    Pthread* threads;                       // Array of thread objects
    volatile int runningThreadCount;        // Number of currently running threads
    volatile int workingThreadCount;        // Number of threads currently executing tasks
    pthread_mutex_t threadCountMutex;       // Mutex for thread count synchronization
    pthread_cond_t threadsAllIdleCondition; // Condition variable to signal when all threads are idle
    TaskQueue taskQueue;                    // Task queue for holding tasks
    int maxThreadCount;                     // Maximum allowed thread count in the pool
    int activeThread;                       // Flag to indicate if the thread pool is active
} ThreadPool, *PThreadPool;



#define LOCK_MUTEX(m)   pthread_mutex_lock(m)
#define UNLOCK_MUTEX(m) pthread_mutex_unlock(m)


// Declaration of static volatile variables
// These variables are used to track the number of running, pending, and blocked threads.

static volatile int RunningThreads; // Variable to track the number of running threads
static volatile int PendingThreads; // Variable to track the number of pending threads
static volatile int BlockedThreads; // Variable to track the number of blocked threads

// Function to handle SIGUSR1 signal
void set_up_signal_handling();

// Generate thread name based on thread ID
void set_thread_name(pthread_t thread_id);

// Function to execute a task
void execute_task(ThreadPool* Pthreadpool, task* Ptask);

// Function to increment the count of working threads
void increment_working_count(ThreadPool* Pthreadpool);

// Function to decrement the count of working threads and signal if all threads have finished their tasks
void decrement_working_count(ThreadPool* Pthreadpool);

// Function to wait for all threads in the thread pool to finish their tasks
void wait_for_threads_to_finish(PThreadPool Pthreadpool);

// Function to free the memory allocated for the thread pool
void free_threads(PThreadPool Pthreadpool);

// Function to allocate memory for the thread pool and initialize it
PThreadPool allocateThreadPool(int num_threads);

// Function to initialize the individual threads in the thread pool
void initializeThreadPool(PThreadPool Pthreadpool, int num_threads);

// Structure pointer typedef for thread pool
typedef struct ThreadPool* threadpool;

// Function to unlock the specified mutex
void unlock_mutex(pthread_mutex_t* mutex);

// Function to lock the specified mutex
void lock_mutex(pthread_mutex_t* mutex);

// Function to initialize the thread pool with the specified number of threads
struct ThreadPool* threadpool_init(int num_threads);

// Function to add a new task to the thread pool
void threadpool_new_task(ThreadPool* thpool, void (*function)(void*), void* argument);

// Function to wait for all tasks in the thread pool to finish
void threadpool_wait(PThreadPool Pthreadpool);

// Function to destroy the thread pool and free associated resources
void threadpool_destroy(PThreadPool Pthreadpool);

// Function to initialize an individual thread
int thread_init(PThreadPool Pthreadpool, Pthread* PThread, int id);

// Function to handle SIGUSR1 signal in a thread
void thread_hold(int signum);

// Thread function to execute tasks
void* thread_execute(void* threadData );
