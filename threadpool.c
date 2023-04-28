
#include "threadpool.h"
pthread_mutex_t hold_mutex = PTHREAD_MUTEX_INITIALIZER;  // Mutex for hold condition
pthread_cond_t hold_cond = PTHREAD_COND_INITIALIZER;  // Condition variable for hold condition

// Lock the given mutex
void lock_mutex(pthread_mutex_t* mutex) {
    /*
    Locks the given mutex.

    Parameters:
        - mutex: Pointer to the mutex to be locked

    Note: The function handles error checking and exits the program if an error occurs.
    */

    int result = pthread_mutex_lock(mutex);
    if (result != 0) {
        perror("lock_mutex: pthread_mutex_lock");
        exit(1);
    }
}

// Unlock the given mutex
void unlock_mutex(pthread_mutex_t* mutex) {
    /*
    Unlocks the given mutex.

    Parameters:
        - mutex: Pointer to the mutex to be unlocked

    Note: The function handles error checking and exits the program if an error occurs.
    */

    int result = pthread_mutex_unlock(mutex);
    if (result != 0) {
        perror("unlock_mutex: pthread_mutex_unlock");
        exit(1);
    }
}


struct ThreadPool* threadpool_init(int num_threads);
PThreadPool allocateThreadPool(int num_threads);
void initializeThreadPool(PThreadPool Pthreadpool, int num_threads);

// Initialize and set up the thread pool
struct ThreadPool* threadpool_init(int num_threads) {
    /*
    Initialize and set up the thread pool by allocating memory for the ThreadPool structure, initializing the thread pool, and returning the initialized thread pool.

    Parameters:
        - num_threads: Number of threads to allocate in the thread pool

    Returns:
        - Pointer to the initialized ThreadPool structure

    Note: The returned ThreadPool structure should be freed using the appropriate cleanup functions when no longer needed.
    */

    // Initialize the PendingThreads flag to 0 and the RunningThreads flag to 1
    PendingThreads = 0;
    RunningThreads = 1;

    if (num_threads < 0) {
        perror("threadpool_init(): Invalid num_threads\n"); // Print error message if num_threads is negative
        exit(1);
    }

    // Allocate memory for the thread pool
    PThreadPool Pthreadpool = allocateThreadPool(num_threads);

    // Initialize the thread pool
    initializeThreadPool(Pthreadpool, num_threads);

    return Pthreadpool; // Return the initialized thread pool
}



// Allocate memory for the thread pool and initialize it
PThreadPool allocateThreadPool(int num_threads) {
    /*
    Allocate and initialize the ThreadPool structure, including the task queue and threads.

    Parameters:
        - num_threads: Number of threads to allocate in the thread pool

    Returns:
        - Pointer to the allocated and initialized ThreadPool structure

    Note: The returned ThreadPool structure should be freed using the appropriate cleanup functions when no longer needed.
    */

    PThreadPool Pthreadpool;

    // Allocate memory for the ThreadPool structure
    Pthreadpool = (PThreadPool)calloc(1, sizeof(ThreadPool));
    if (Pthreadpool == NULL) {
        // Error handling for memory allocation failure
        perror("allocateThreadPool(): Failed to allocate memory for ThreadPool");
        exit(1);
    }

    // Initialize runningThreadCount and workingThreadCount
    Pthreadpool->runningThreadCount = 0;
    Pthreadpool->workingThreadCount = 0;

    // Initialize the task queue
    if (TaskQueue_init(&Pthreadpool->taskQueue) == -1) {
        // Error handling for task queue initialization failure
        perror("allocateThreadPool(): Failed to initialize task queue");
        free(Pthreadpool);
        exit(1);
    }

    // Allocate memory for threads
    Pthreadpool->threads = (Pthread*)calloc(num_threads, sizeof(Pthread));
    if (Pthreadpool->threads == NULL) {
        // Error handling for memory allocation failure
        perror("allocateThreadPool(): Failed to allocate memory for threads");
        TaskQueue_destroy(&Pthreadpool->taskQueue);
        free(Pthreadpool);
        return NULL;
    }

    // Initialize the thread count mutex and threadsAllIdleCondition condition variable
    pthread_mutex_init(&(Pthreadpool->threadCountMutex), NULL);
    pthread_cond_init(&Pthreadpool->threadsAllIdleCondition, NULL);

    return Pthreadpool;
}


void initializeThreadPool(PThreadPool Pthreadpool, int num_threads) {
    /*
    Initialize the individual threads in the thread pool.
    
    Parameters:
        - Pthreadpool: Pointer to the thread pool structure
        - num_threads: Number of threads to initialize
    
    Note: The thread pool should be properly allocated and initialized before calling this function.
    */

    int threadIndex;

    // Initialize each thread in the thread pool
    for (threadIndex = 0; threadIndex < num_threads; threadIndex++) {
        thread_init(Pthreadpool, &(Pthreadpool->threads[threadIndex]), threadIndex);
    }

    // Wait until all threads have started running
    while (Pthreadpool->runningThreadCount != num_threads) {
        // Do nothing and wait until the running thread count matches the desired number of threads
    }
}


int threadpool_new_task(ThreadPool* Pthreadpool, void (*function)(void*), void* argument) {
    /*
    Add a new task to the thread pool's task queue.
    
    Parameters:
        - Pthreadpool: Pointer to the thread pool structure
        - function: Pointer to the function to be executed as a task
        - argument: Pointer to the argument to be passed to the function
    
    Returns:
        - 0 if the task is successfully added
        - -1 if there was an error
    
    Note: The function pointer and argument should be valid.
    */

    // Check if the thread pool pointer is valid
    if (Pthreadpool == NULL) {
        perror("Invalid thread pool pointer");
        return -1;  // Or any appropriate error code
    }

    // Check if the function pointer is valid
    if (function == NULL) {
        perror("Invalid function pointer");
        return -1;  // Or any appropriate error code
    }

    // Allocate memory for the new task
    task* newTask = malloc(sizeof(task));
    if (newTask == NULL) {
        perror("Failed to allocate memory");
        return -1;  // Or any appropriate error code
    }

    // Assign the function and argument to the new task
    newTask->func = function;
    newTask->arg = argument;

    // Add the new task to the thread pool's task queue
    TaskQueue_push(&(Pthreadpool->taskQueue), newTask);

    return 0;
}


void threadpool_wait(PThreadPool Pthreadpool) {
    /*
    Wait for the thread pool to finish all tasks and become idle.
    
    Parameters:
        - Pthreadpool: Pointer to the thread pool structure
    */

    // Check if the thread pool pointer is valid
    if (Pthreadpool == NULL) {
        perror("Invalid thread pool pointer");
        return;
    }

    // Lock the thread count mutex to ensure thread safety
    lock_mutex(&Pthreadpool->threadCountMutex);

    // Wait until both the task queue is empty and all threads are idle
    while (Pthreadpool->taskQueue.size || Pthreadpool->workingThreadCount) {
        pthread_cond_wait(&Pthreadpool->threadsAllIdleCondition, &Pthreadpool->threadCountMutex);
    }

    // Unlock the thread count mutex
    unlock_mutex(&Pthreadpool->threadCountMutex);
}



void wait_for_threads_to_finish(PThreadPool Pthreadpool) {
    /*
    Wait for all threads in the thread pool to finish executing their tasks.
    
    Parameters:
        - Pthreadpool: Pointer to the thread pool structure
    */

    const double TIMEOUT = 1.0;  // Timeout in seconds

    time_t start_time, current_time;
    double elapsed_time = 0.0;

    time(&start_time);

    // Wait until either the timeout is reached or all threads have finished
    while (elapsed_time < TIMEOUT && Pthreadpool->runningThreadCount) {
        // Release all semaphores to unblock waiting threads
        release_all_semaphores(Pthreadpool->taskQueue.has_tasks);

        time(&current_time);
        elapsed_time = difftime(current_time, start_time);
    }

    // If threads are still running, wait with a sleep interval of 1 second
    while (Pthreadpool->runningThreadCount) {
        // Release all semaphores to unblock waiting threads
        release_all_semaphores(Pthreadpool->taskQueue.has_tasks);

        sleep(1);
    }
}


void free_threads(PThreadPool Pthreadpool) {
	/*
	* This function frees the thread objects and threads array in the thread pool.
	* It takes a PThreadPool pointer Pthreadpool as a parameter.
	*/
    int i;
    // Free each thread object in the threads array
    for (i = 0; i < Pthreadpool->runningThreadCount; i++) {
        free(Pthreadpool->threads[i]); // Free the memory allocated for each thread object
    }
    // Free the threads array itself
    free(Pthreadpool->threads); // Free the memory allocated for the threads array
}

void threadpool_destroy(PThreadPool Pthreadpool) {
	/*
	* This function destroys the thread pool and releases all associated resources.
	* It takes a PThreadPool pointer Pthreadpool as a parameter.
	*/
    if (Pthreadpool == NULL) return;

    RunningThreads = 0; // Assuming RunningThreads is a global variable used to control thread execution

    wait_for_threads_to_finish(Pthreadpool); // Wait for threads to finish executing tasks

    TaskQueue_destroy(&Pthreadpool->taskQueue); // Destroy the task queue

    free_threads(Pthreadpool); // Free the thread objects and threads array

    free(Pthreadpool); // Free the thread pool object itself
}


int thread_init(PThreadPool Pthreadpool, Pthread* PThread, int id) {
    /*
    Initialize a thread within a thread pool.
    
    Parameters:
        - Pthreadpool: Pointer to the thread pool structure
        - PThread: Pointer to the thread structure
        - id: ID of the thread

    Returns:
        - 0 on success, or an error code if an error occurs during thread initialization
    */
    
    // Allocate memory for the thread structure
    *PThread = (Pthread) calloc(1, sizeof(struct Thread));
    if (!*PThread) {
        perror("calloc");
        exit(1);
    }

    (*PThread)->threadPoolPtr = Pthreadpool;
    (*PThread)->id = id;

    // Create the thread
    int result = pthread_create(&(*PThread)->pthread, NULL, (void * (*)(void *)) thread_execute, (*PThread));
    if (result != 0) {
        perror("pthread_create");
        free(*PThread); // Cleanup allocated memory
        exit(1);
    }

    // Detach the thread
    result = pthread_detach((*PThread)->pthread);
    if (result != 0) {
        perror("pthread_detach");
        free(*PThread); // Cleanup allocated memory
        exit(1);
    }

    return 0;
}



void thread_hold(int signum) {
	/*
	The code uses a mutex (hold_mutex) and a condition variable (hold_cond) to synchronize threads. It waits until PendingThreads flag becomes false, indicating that the thread is no longer pending. The mutex is locked before setting the PendingThreads flag and released within the loop in case an error occurs.
	*/
    (void)signum; // Suppress unused parameter warning
    
    lock_mutex(&hold_mutex); // Acquire the lock for the mutex
    PendingThreads = 1; // Set PendingThreads flag to 1 to indicate thread is pending
    
    while (PendingThreads) {
        int result = pthread_cond_wait(&hold_cond, &hold_mutex); // Wait for condition variable signal
        if (result != 0) {
            perror("pthread_cond_wait"); // Print error message if pthread_cond_wait fails
            unlock_mutex(&hold_mutex); // Unlock the mutex before exiting
            exit(1);
        }
    }
    
    unlock_mutex(&hold_mutex); // Unlock the mutex after the loop
}


void execute_task(ThreadPool* Pthreadpool, task* Ptask) {
    /*
	* This function executes a task in the thread pool.
	* It takes a ThreadPool pointer Pthreadpool and a task pointer Ptask as parameters.
	*/
	// Extract the function pointer and argument from the task
    void (*task_function)(void*) = Ptask->func; // Get the function pointer from the task
    void* task_argument = Ptask->arg; // Get the argument from the task

    // Execute the task function with the provided argument
    task_function(task_argument); // Execute the task function with the argument

    // Free the task memory
    free(Ptask); // Free the memory allocated for the task
}


void increment_working_count(ThreadPool* Pthreadpool) {
	/*
	* This function increments the count of working threads in the thread pool.
	* It takes a ThreadPool pointer Pthreadpool as a parameter.
	*/
    lock_mutex(&Pthreadpool->threadCountMutex); // Lock the thread count mutex to prevent race conditions
    Pthreadpool->workingThreadCount++; // Increment the count of working threads
    unlock_mutex(&Pthreadpool->threadCountMutex); // Unlock the thread count mutex
}


void decrement_working_count(ThreadPool* Pthreadpool) {
	/*
	* This function decrements the count of working threads in the thread pool.
	* It takes a ThreadPool pointer Pthreadpool as a parameter.
	*/
    lock_mutex(&Pthreadpool->threadCountMutex); // Lock the thread count mutex to prevent race conditions
    Pthreadpool->workingThreadCount--; // Decrement the count of working threads

    // Signal if all threads have finished their tasks
    if (Pthreadpool->workingThreadCount == 0) {
        pthread_cond_signal(&Pthreadpool->threadsAllIdleCondition); // Signal the threadsAllIdleCondition condition variable
    }

    unlock_mutex(&Pthreadpool->threadCountMutex); // Unlock the thread count mutex
}



void set_thread_name(pthread_t thread_id) {
	/*
	* This function sets the name of the specified thread using its thread ID.
	* It takes a pthread_t variable thread_id as a parameter.
	*/
    // Generate thread name based on thread ID
    char name[128] = {0}; // Initialize a character array to store the thread name
    snprintf(name, 128, "%ld", thread_id); // Format the thread ID as a string and copy it to the name array

    // Set the thread name using prctl
    if (prctl(PR_SET_NAME, name) != 0) { // Set the thread name using the prctl function and check for errors
        perror("Error setting thread name"); // Print an error message if prctl fails
        exit(1); // Exit the program with an error status
	}
}

void set_up_signal_handling() {
	/*
	* This function sets up signal handling for SIGUSR1.
	* It registers a signal handler function called thread_hold.
	*/

    struct sigaction signal_action;
    signal_action.sa_handler = thread_hold;

    // Clear the signal mask
    if (sigemptyset(&signal_action.sa_mask) != 0) {
        perror("Error setting signal mask");
        exit(1);
    }

    signal_action.sa_flags = 0;

    // Set the signal handler for SIGUSR1
    if (sigaction(SIGUSR1, &signal_action, NULL) == -1) {
        perror("Error setting signal handler for SIGUSR1");
        exit(1);
    }
}


void* thread_execute(void* threadData) {
	/* 
	This function is executed by each thread in the thread pool to execute tasks from the task queue.
	  It takes a void pointer to threadData as a parameter and returns a void pointer.
 	 threadData contains information about the thread and the thread pool it belongs to.
	
	Parameters:
	threadData: Pointer to the thread data structure.
	
	Returns:
		NULL.
	*/
    Pthread PThread = (Pthread) threadData;
    ThreadPool* Pthreadpool = PThread->threadPoolPtr;

    // Set the thread name and signal handling
    set_thread_name(pthread_self());
    set_up_signal_handling();

    // Increment the count of running threads
    lock_mutex(&Pthreadpool->threadCountMutex);
    Pthreadpool->runningThreadCount += 1;
    unlock_mutex(&Pthreadpool->threadCountMutex);

    int runningThreads = RunningThreads;  // Use a local variable

    while (runningThreads) {
        // Wait for a task in the task queue
        semaphore_wait(Pthreadpool->taskQueue.has_tasks);

        // Increment the working count
        increment_working_count(Pthreadpool);

        task* task_ptr;
        while ((task_ptr = TaskQueue_pull(&Pthreadpool->taskQueue))) {
            // Execute tasks pulled from the task queue
            execute_task(Pthreadpool, task_ptr);
        }

        // Decrement the working count
        decrement_working_count(Pthreadpool);

        // Check if there are still running threads
        runningThreads = RunningThreads;
    }

    // Decrement the count of running threads
    lock_mutex(&Pthreadpool->threadCountMutex);
    Pthreadpool->runningThreadCount--;
    unlock_mutex(&Pthreadpool->threadCountMutex);

    return NULL;
}







