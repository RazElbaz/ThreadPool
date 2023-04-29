#include <time.h>
#include "codec.h"
#include <stdio.h>
#include <pthread.h>
#include <stdint.h>
#include "threadpool.h"
#include <stdlib.h> 
#include <unistd.h> 
#include <stdbool.h>
#include "main.h"


ProgramData programData;

// Helper function to process the data (either encryption or decryption)
void process_data(Pinput data) {
    // Check the mode and call the appropriate function
    if (programData.mode == 'e') {
        encrypt(data->value, programData.key);
    } else if (programData.mode == 'd') {
        decrypt(data->value, programData.key);
    }
}

// Helper function to wait until the current data item is the one that needs to be processed
void wait_for_data_index(Pinput data) {
    // Acquire the lock to protect access to the shared variable programData.data_index
    pthread_mutex_lock(&mutex);
    // While the current data item is not the one that needs to be processed, wait on the condition variable
    while (data->index != programData.data_index) {
        pthread_cond_wait(&condition_variable, &mutex);
    }
    // Release the lock before returning
    pthread_mutex_unlock(&mutex);
}

// Helper function to broadcast to all waiting threads that the next data item can be processed
void notify_next_data() {
    // Acquire the lock to protect access to the shared variable programData.data_index
    pthread_mutex_lock(&mutex);
    // Increment the data index to indicate that the next data item can be processed
    programData.data_index++;
    // Signal all threads waiting on the condition variable that the data index has changed
    pthread_cond_broadcast(&condition_variable);
    // Release the lock before returning
    pthread_mutex_unlock(&mutex);
}

// The task that will be executed by the threadpool
void new_task(void *arg) {
    // Cast the argument to a Pinput type
    Pinput data = (Pinput)arg;
    // Process the data (either encryption or decryption)
    process_data(data);
    // Wait until the current data item is the one that needs to be processed
    wait_for_data_index(data);
    // Print the data
    printf("%s", data->value);
    // Notify all waiting threads that the next data item can be processed
    notify_next_data();
    // Free the memory allocated for the data item
    free(data->value);
    free(data);
}


// Helper function to parse the encryption key and mode from command-line arguments
void parse_arguments(int argc, char *argv[]) {
    // Parse the encryption key from the command-line argument
    programData.key = atoi(argv[1]);

    // Parse the encryption/decryption mode from the command-line argument
    programData.mode = argv[2][1];

    // Initialize the current and data indices
    programData.current_index = 0;
    programData.data_index = 0;

    // Check if the program was run with the correct number of arguments
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <key> <-e/-d>\n", argv[0]);
        exit(1);
    }

    // Check if the encryption/decryption mode is valid
    if (programData.mode != 'e' && programData.mode != 'd') {
        fprintf(stderr, "Invalid mode: %c, must be -e or -d\n", programData.mode);
        exit(1);
    }
}


// Helper function to initialize the thread pool
threadpool initialize_threadpool() {
    int num_threads = sysconf(_SC_NPROCESSORS_ONLN);
    return threadpool_init(num_threads);
}

// Helper function to allocate and read data from stdin
Pinput allocate_and_read_data() {
    // Allocate memory for the data item
    Pinput data = (Pinput)calloc(1, sizeof(data));
    if (!data) {
        perror("calloc");
        exit(1);
    }

    // Set the index of the data item
    data->index = programData.current_index;

    // Allocate memory for the data value
    data->value = (char *)malloc(MAX_DATA_COUNT);
    if (!data->value) {
        perror("malloc");
        free(data);
        exit(1);
    }

    // Read data from stdin into the data value
    if (!fgets(data->value, MAX_DATA_COUNT, stdin)) {
        // End of input, free the allocated memory and return NULL
        free(data->value);
        free(data);
        return NULL;
    }

    // Return the data item
    return data;
}

// The main function
int main(int argc, char *argv[]) {
    // Parse the encryption key and mode from command-line arguments
    parse_arguments(argc, argv);

    // Initialize the thread pool
    threadpool thpool = initialize_threadpool();

    // Read data from stdin and add tasks to the thread pool
    while (1) {
        // Allocate and read data from stdin
        Pinput data = allocate_and_read_data();
        if (data == NULL) {
            break; // End of input, break out of the loop
        }

        // Add a new task to the thread pool to process the data item
        threadpool_new_task(thpool, new_task, data);
        programData.current_index++;
    }

    // Wait for all tasks to complete and destroy the thread pool
    threadpool_wait(thpool);
    threadpool_destroy(thpool);

    return 0;
}

