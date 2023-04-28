#include "task.h"

// Helper function to initialize semaphore and return an error code if allocation fails
int initialize_semaphore(PTaskQueue p_task) {
    p_task->has_tasks = (Psemaphore)calloc(1, sizeof(semaphore));
    if (p_task->has_tasks == NULL) {
        return -1;
    }
    semaphore_init(p_task->has_tasks, 0);
    return 0;
}

// Helper function to clean up allocated memory when mutex initialization fails
void cleanup_memory(PTaskQueue p_task) {
    free(p_task->has_tasks);
    p_task->has_tasks = NULL;
}

// Helper function to acquire lock on mutex and return an error code if lock fails
int acquire_lock(PTaskQueue p_task) {
    int mutex_lock_result = pthread_mutex_lock(&p_task->mutex);
    if (mutex_lock_result != 0) {
        return -1;
    }
    return 0;
}

// Helper function to release lock on mutex
void release_lock(PTaskQueue p_task) {
    pthread_mutex_unlock(&p_task->mutex);
}

// Function to initialize task queue with size 0 and empty head and tail
int TaskQueue_init(PTaskQueue p_task) {
    if (p_task == NULL) {
        return -1;
    }
    p_task->size = 0;
    p_task->head = NULL;
    p_task->tail = NULL;
    if (initialize_semaphore(p_task) != 0) {
        return -1;
    }
    if (pthread_mutex_init(&(p_task->mutex), NULL) != 0) {
        cleanup_memory(p_task);
        return -1;
    }
    return 0;
}

// Function to add a new task to the end of the task queue
void TaskQueue_push(PTaskQueue p_task, Ptask Pnew_task) {
    if (p_task == NULL || Pnew_task == NULL) {
        return;
    }
    if (acquire_lock(p_task) != 0) {
        return;
    }
    Pnew_task->previous_task = NULL;
    if (p_task->size == 0) {
        p_task->head = Pnew_task;
    } else {
        p_task->tail->previous_task = Pnew_task;
    }
    p_task->tail = Pnew_task;
    p_task->size++;
    release_semaphore(p_task->has_tasks);
    release_lock(p_task);
}


Ptask TaskQueue_pull(PTaskQueue p_task)
{
    // Check if the input pointer is NULL
    if (p_task == NULL) {
        return NULL;
    }
    // Acquire lock on the mutex
    int mutex_lock_result = pthread_mutex_lock(&p_task->mutex);

    // Check if the mutex lock failed
    if (mutex_lock_result != 0) {
        return NULL;
    }
    // Get the current task from the head of the queue
    Ptask curr_task = p_task->head;

    // Check if there are no tasks in the queue
    if (p_task->size == 0) {
        // Release the lock and return NULL
        pthread_mutex_unlock(&p_task->mutex);
        return NULL;
    }

    // Check if there is only one task in the queue
    if (p_task->size == 1) {
        // Update the head, tail, and size of the queue
        p_task->head = NULL;
        p_task->tail = NULL;
        p_task->size = 0;
    } else {
        // If there are more than one tasks in the queue
        // Update the head and size of the queue
        p_task->head = curr_task->previous_task;
        p_task->size--;
        // Signal that there are more tasks in the queue
        release_semaphore(p_task->has_tasks);
    }

    // Release the lock
    pthread_mutex_unlock(&p_task->mutex);

    // Return the current task
    return curr_task;
}

void TaskQueue_destroy(PTaskQueue p_task)
{

    // Free all the tasks in the queue
    while (p_task->size) {
        Ptask task = TaskQueue_pull(p_task);
        if (task == NULL) {
            // Handle the error and continue freeing the remaining tasks in the queue
            fprintf(stderr, "Error: Failed to pull task from queue.\n");
            continue;
        }
        free(task);
    }

    // Reset the head, tail, size, and semaphore
    p_task->head = NULL;
    p_task->tail = NULL;

    semaphore_reset(p_task->has_tasks);
    p_task->size = 0;

    // Free the semaphore memory
    free(p_task->has_tasks);
}
