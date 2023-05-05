#include "task.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Function to initialize a task queue with size 0 and empty head and tail
int TaskQueue_init(PTaskQueue PTask) {
    // Check if the provided task queue pointer is NULL
    if (PTask == NULL) {
        return -1; // Return an error code to indicate failure
    }
    
    // Update the queue with a single task (assuming it adds a default task)
    update_queue_single_task(PTask);
    
    // Initialize the semaphore for the task queue
    if (initialize_semaphore(PTask) != 0) {
        return -1; // Return an error code if semaphore initialization fails
    }

    // Initialize the mutex for the task queue
    if (pthread_mutex_init(&(PTask->mutex), NULL) != 0) {
        cleanup_memory(PTask); // Clean up memory to avoid leaks
        return -1; // Return an error code if mutex initialization fails
    }

    return 0; // Return 0 to indicate successful initialization
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Function to add a new task to the end of the task queue
void TaskQueue_push(PTaskQueue PTask, Ptask PTask_new) {
    // Check if the provided task queue or new task pointers are NULL
    if (PTask == NULL || PTask_new == NULL) {
        return; // Exit the function if either pointer is NULL
    }
    
    // Acquire the lock to ensure thread-safety
    if (acquire_lock(PTask) != 0) {
        return; // Exit the function if lock acquisition fails
    }
    
    // Set the previous_task pointer of the new task to NULL
    PTask_new->previous_task = NULL;
    
    // Check if the task queue is empty
    if (PTask->size == 0) {
        PTask->head = PTask_new; // If empty, set the new task as the head
    } else {
        PTask->tail->previous_task = PTask_new; // If not empty, update the previous_task pointer of the current tail
    }
    
    PTask->tail = PTask_new; // Set the new task as the tail
    PTask->size++; // Increment the size of the task queue
    release_semaphore(PTask->has_tasks); // Release the semaphore to signal the availability of tasks
    release_lock(PTask); // Release the lock
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Function to pull a task from the task queue
Ptask TaskQueue_pull(PTaskQueue PTask) {
    // Check if the provided task queue pointer is NULL
    if (PTask == NULL) {
        return NULL; // Return NULL to indicate an error condition
    }

    int mutex_lock_result = acquire_mutex_lock(PTask); // Acquire the mutex lock for thread-safety
    if (mutex_lock_result != 0) {
        return NULL; // Return NULL if mutex lock acquisition fails
    }

    Ptask curr_task = pull_task(PTask); // Pull a task from the task queue

    release_mutex_lock(PTask); // Release the mutex lock

    return curr_task; // Return the pulled task
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Pulls a task from the head of the task queue
Ptask pull_task(PTaskQueue PTask) {
    Ptask curr_task = PTask->head; // Store the current head of the task queue
    
    // Check if the task queue is empty
    if (is_queue_empty(PTask)) {
        return NULL; // If empty, return NULL to indicate no task is available
    }
    
    // Check if the task queue has only one task
    if (PTask->size == 1) {
        update_queue_single_task(PTask); // If only one task, update the queue accordingly
    } else {
        update_queue_multiple_tasks(PTask, curr_task); // If multiple tasks, update the queue accordingly
    }
    
    return curr_task; // Return the pulled task
}
///////////////////////////////////////////////////////////////////helper functions////////////////////////////////////////////////////////////////////

// Helper function to initialize semaphore and return an error code if allocation fails
int initialize_semaphore(PTaskQueue PTask) {
    PTask->has_tasks = (Psemaphore)calloc(1, sizeof(semaphore));
    if (PTask->has_tasks == NULL) {
        return -1;
    }
    semaphore_init(PTask->has_tasks, 0);
    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Helper function to clean up allocated memory when mutex initialization fails
void cleanup_memory(PTaskQueue PTask) {
    free(PTask->has_tasks);
    PTask->has_tasks = NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Helper function to acquire lock on mutex and return an error code if lock fails
int acquire_lock(PTaskQueue PTask) {
    int mutex_lock_result = pthread_mutex_lock(&PTask->mutex);
    if (mutex_lock_result != 0) {
        return -1;
    }
    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Helper function to release lock on mutex
void release_lock(PTaskQueue PTask) {
    pthread_mutex_unlock(&PTask->mutex);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Acquires the lock on the mutex and returns an error code if it fails
int acquire_mutex_lock(PTaskQueue PTask) {
    return pthread_mutex_lock(&PTask->mutex);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Releases the lock on the mutex
void release_mutex_lock(PTaskQueue PTask) {
    pthread_mutex_unlock(&PTask->mutex);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Checks if the task queue is empty
int is_queue_empty(PTaskQueue PTask) {
    return PTask->size == 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Updates the head, tail, and size of the task queue when it has only one task
void update_queue_single_task(PTaskQueue PTask) {
    PTask->head = NULL;
    PTask->tail = NULL;
    PTask->size = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Updates the head and size of the task queue when it has more than one task
void update_queue_multiple_tasks(PTaskQueue PTask, Ptask curr_task) {
    PTask->head = curr_task->previous_task;
    PTask->size--;
    release_semaphore(PTask->has_tasks);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Frees a single task from the queue
void free_task(PTaskQueue PTask) {
    Ptask task = TaskQueue_pull(PTask);
    if (task == NULL) {
        // Handle the error and continue freeing the remaining tasks in the queue
        fprintf(stderr, "Error: Failed to pull task from queue.\n");
        return;
    }
    free(task);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Frees all tasks in the queue
void free_all_tasks(PTaskQueue PTask) {
    while (PTask->size) {
        free_task(PTask);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Resets the head, tail, size, and semaphore
void reset_queue_state(PTaskQueue PTask) {
    PTask->head = NULL;
    PTask->tail = NULL;
    semaphore_reset(PTask->has_tasks);
    PTask->size = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Frees the semaphore memory
void free_semaphore(PTaskQueue PTask) {
    free(PTask->has_tasks);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Destroys the task queue
void TaskQueue_destroy(PTaskQueue PTask) {
    free_all_tasks(PTask);
    reset_queue_state(PTask);
    free_semaphore(PTask);
}

