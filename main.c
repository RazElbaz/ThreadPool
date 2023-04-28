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

// The task that will be executed by the threadpool
void new_task(void *arg)
{
    Pinput data = (Pinput) arg;
    if (programData.mode == 'e') {
        encrypt(data->value, programData.key);
    } else if (programData.mode == 'd') {
        decrypt(data->value, programData.key);
    }
    // Wait until the current data item is the one that needs to be processed
    pthread_mutex_lock(&mutex);
    while (data->index != programData.data_index) {
        pthread_cond_wait(&condition_variable, &mutex);
    }
    printf("%s", data->value);
    programData.data_index++;
    // Broadcast to all waiting threads that the next data item can be processed
    pthread_cond_broadcast(&condition_variable);
    pthread_mutex_unlock(&mutex);
    free(data->value);
    free(data);
}


int main(int argc, char *argv[]) {
    

    programData.key = atoi(argv[1]); // Parse the encryption key and the encryption/decryption mode
    programData.mode = argv[2][1];
    programData.current_index = 0;
    programData.data_index = 0;
    // Check if the program was run with the correct arguments
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <key> <-e/-d>\n", argv[0]);
        exit(1);
    }


    if (programData.mode != 'e' && programData.mode != 'd') {
        fprintf(stderr, "Invalid mode: %c, must be -e or -d\n", programData.mode);
        exit(1);
    }

    // Initialize the thread pool with the number of available CPUs
    int num_threads = sysconf(_SC_NPROCESSORS_ONLN);
    threadpool thpool = threadpool_init(num_threads);

    // Read data from stdin and add tasks to the thread pool
    while (1) {
        Pinput data = (Pinput) calloc(1, sizeof(data));
        if (!data) {
            perror("calloc");
            exit(1);
        }
        data->index = programData.current_index;
        data->value = (char *) malloc(MAX_DATA_COUNT);
        if (!data->value) {
            perror("malloc");
            free(data);
            exit(1);
        }
        if (!fgets(data->value, MAX_DATA_COUNT, stdin)) {
            break;
        }
        threadpool_new_task(thpool, new_task, data);
        programData.current_index++;
    }

    // Wait for all tasks to complete and destroy the thread pool
    threadpool_wait(thpool);
    threadpool_destroy(thpool);
    return 0;
}
