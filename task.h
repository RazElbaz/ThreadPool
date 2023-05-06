#include "semaphore.h"

/*struct task: A structure that represents a task to be executed by the TaskQueue. It contains a function pointer to the task function and a pointer to the arguments of the task function.*/
typedef struct task {
    int task_id; // Unique identifier for the task
    struct task* previous_task;     // Pointer to the previous task in the queue
    void (*func)(void* arg);    // Function pointer to the task function
    void* arg;    // Pointer to the arguments of the task function
} task, *Ptask;

/*struct TaskQueue: A structure that represents a queue of tasks. It contains a mutex to synchronize access to the queue, pointers to the head and tail of the queue, a semaphore to signal when there are tasks in the queue, and the number of tasks in the queue.*/
typedef struct TaskQueue {
	int size;               // Number of tasks in the queue
	Ptask tail;             // Pointer to the tail (last) task in the queue
	Ptask head;             // Pointer to the head (first) task in the queue
    Psemaphore has_tasks;   // Semaphore to signal when there are tasks in the queue
	pthread_mutex_t mutex;  // Mutex to synchronize access to the queue
    int max_capacity;       // Maximum capacity of the queue
} TaskQueue, *PTaskQueue;

// Helper function to initialize semaphore and return an error code if allocation fails
int initialize_semaphore(PTaskQueue PTask) ;

// Helper function to clean up allocated memory when mutex initialization fails
void cleanup_memory(PTaskQueue PTask);

// Helper function to release lock on mutex
void release_lock(PTaskQueue PTask);

// Helper function to acquire lock on mutex and return an error code if lock fails
int acquire_lock(PTaskQueue PTask);

//Initializes a TaskQueue. It takes a pointer to a TaskQueue as an argument and returns an integer indicating success or failure.
int   TaskQueue_init(PTaskQueue PTask);

//Adds a task to the end of the queue. It takes a pointer to a TaskQueue and a pointer to a task as arguments.
void  TaskQueue_push(PTaskQueue PTask, Ptask Pnew_task);

//Removes and returns the first task from the queue. It takes a pointer to a TaskQueue as an argument and returns a pointer to the removed task.
Ptask TaskQueue_pull(PTaskQueue PTask);

// Pulls a task from the head of the task queue
Ptask pull_task(PTaskQueue PTask);

// Updates the head and size of the task queue when it has more than one task
void update_queue_multiple_tasks(PTaskQueue PTask, Ptask curr_task);

// Releases the lock on the mutex
void release_mutex_lock(PTaskQueue PTask);

// Acquires the lock on the mutex and returns an error code if it fails
int acquire_mutex_lock(PTaskQueue PTask);

// Checks if the task queue is empty
int is_queue_empty(PTaskQueue PTask);

// Updates the head, tail, and size of the task queue when it has only one task
void update_queue_single_task(PTaskQueue PTask);

//Destroys a TaskQueue and frees all memory associated with it. It takes a pointer to a TaskQueue as an argument.
void  TaskQueue_destroy(PTaskQueue PTask);

// Frees the semaphore memory
void free_semaphore(PTaskQueue PTask);

// Frees a single task from the queue
void free_task(PTaskQueue PTask);

// Frees all tasks in the queue
void free_all_tasks(PTaskQueue PTask);

// Resets the head, tail, size, and semaphore
void reset_queue_state(PTaskQueue PTask);