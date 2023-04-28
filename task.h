#include "semaphore.h"

/*struct task: A structure that represents a task to be executed by the TaskQueue. It contains a function pointer to the task function and a pointer to the arguments of the task function.*/
typedef struct task {
    struct task* previous_task;     // Pointer to the previous task in the queue
    void (*func)(void* arg);    // Function pointer to the task function
    void* arg;    // Pointer to the arguments of the task function
} task, *Ptask;

/*struct TaskQueue: A structure that represents a queue of tasks. It contains a mutex to synchronize access to the queue, pointers to the head and tail of the queue, a semaphore to signal when there are tasks in the queue, and the number of tasks in the queue.*/
typedef struct TaskQueue {
	int size;    // Number of tasks in the queue
	Ptask tail;    // Pointer to the tail (last) task in the queue
	Ptask head;    // Pointer to the head (first) task in the queue    
    Psemaphore has_tasks;    // Semaphore to signal when there are tasks in the queue
	pthread_mutex_t mutex;    // Mutex to synchronize access to the queue
    
} TaskQueue, *PTaskQueue;

/*Initializes a TaskQueue. It takes a pointer to a TaskQueue as an argument and returns an integer indicating success or failure.*/
int   TaskQueue_init(PTaskQueue P_task);

/* Adds a task to the end of the queue. It takes a pointer to a TaskQueue and a pointer to a task as arguments.*/
void  TaskQueue_push(PTaskQueue P_task, Ptask Pnew_task);

/*Removes and returns the first task from the queue. It takes a pointer to a TaskQueue as an argument and returns a pointer to the removed task.*/
Ptask TaskQueue_pull(PTaskQueue P_task);

/*Destroys a TaskQueue and frees all memory associated with it. It takes a pointer to a TaskQueue as an argument.*/
void  TaskQueue_destroy(PTaskQueue P_task);

