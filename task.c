#include "task.h"

// Helper function to initialize semaphore and return an error code if allocation fails
int initialize_semaphore(PTaskQueue PTask) {
    PTask->has_tasks = (Psemaphore)calloc(1, sizeof(semaphore));
    if (PTask->has_tasks == NULL) {
        return -1;
    }
    semaphore_init(PTask->has_tasks, 0);
    return 0;
}

// Helper function to clean up allocated memory when mutex initialization fails
void cleanup_memory(PTaskQueue PTask) {
    free(PTask->has_tasks);
    PTask->has_tasks = NULL;
}

// Helper function to acquire lock on mutex and return an error code if lock fails
int acquire_lock(PTaskQueue PTask) {
    int mutex_lock_result = pthread_mutex_lock(&PTask->mutex);
    if (mutex_lock_result != 0) {
        return -1;
    }
    return 0;
}

// Helper function to release lock on mutex
void release_lock(PTaskQueue PTask) {
    pthread_mutex_unlock(&PTask->mutex);
}

// Function to initialize task queue with size 0 and empty head and tail
int TaskQueue_init(PTaskQueue PTask) {
    if (PTask == NULL) {
        return -1;
    }
    PTask->size = 0;
    PTask->head = NULL;
    PTask->tail = NULL;
    if (initialize_semaphore(PTask) != 0) {
        return -1;
    }
    if (pthread_mutex_init(&(PTask->mutex), NULL) != 0) {
        cleanup_memory(PTask);
        return -1;
    }
    return 0;
}

// Function to add a new task to the end of the task queue
void TaskQueue_push(PTaskQueue PTask, Ptask PTask_new) {
    if (PTask == NULL || PTask_new == NULL) {
        return;
    }
    if (acquire_lock(PTask) != 0) {
        return;
    }
    PTask_new->previous_task = NULL;
    if (PTask->size == 0) {
        PTask->head = PTask_new;
    } else {
        PTask->tail->previous_task = PTask_new;
    }
    PTask->tail = PTask_new;
    PTask->size++;
    release_semaphore(PTask->has_tasks);
    release_lock(PTask);
}


// Acquires the lock on the mutex and returns an error code if it fails
int acquire_mutex_lock(PTaskQueue PTask) {
    return pthread_mutex_lock(&PTask->mutex);
}

// Releases the lock on the mutex
void release_mutex_lock(PTaskQueue PTask) {
    pthread_mutex_unlock(&PTask->mutex);
}

// Checks if the task queue is empty
int is_queue_empty(PTaskQueue PTask) {
    return PTask->size == 0;
}

// Updates the head, tail, and size of the task queue when it has only one task
void update_queue_single_task(PTaskQueue PTask) {
    PTask->head = NULL;
    PTask->tail = NULL;
    PTask->size = 0;
}

// Updates the head and size of the task queue when it has more than one task
void update_queue_multiple_tasks(PTaskQueue PTask, Ptask curr_task) {
    PTask->head = curr_task->previous_task;
    PTask->size--;
    release_semaphore(PTask->has_tasks);
}

// Pulls a task from the head of the task queue
Ptask pull_task(PTaskQueue PTask) {
    Ptask curr_task = PTask->head;

    if (is_queue_empty(PTask)) {
        return NULL;
    }

    if (PTask->size == 1) {
        update_queue_single_task(PTask);
    } else {
        update_queue_multiple_tasks(PTask, curr_task);
    }

    return curr_task;
}

// Function to pull a task from the task queue
Ptask TaskQueue_pull(PTaskQueue PTask) {
    if (PTask == NULL) {
        return NULL;
    }

    int mutex_lock_result = acquire_mutex_lock(PTask);
    if (mutex_lock_result != 0) {
        return NULL;
    }

    Ptask curr_task = pull_task(PTask);

    release_mutex_lock(PTask);

    return curr_task;
}

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

// Frees all tasks in the queue
void free_all_tasks(PTaskQueue PTask) {
    while (PTask->size) {
        free_task(PTask);
    }
}

// Resets the head, tail, size, and semaphore
void reset_queue_state(PTaskQueue PTask) {
    PTask->head = NULL;
    PTask->tail = NULL;
    semaphore_reset(PTask->has_tasks);
    PTask->size = 0;
}

// Frees the semaphore memory
void free_semaphore(PTaskQueue PTask) {
    free(PTask->has_tasks);
}

// Destroys the task queue
void TaskQueue_destroy(PTaskQueue PTask) {
    free_all_tasks(PTask);
    reset_queue_state(PTask);
    free_semaphore(PTask);
}

