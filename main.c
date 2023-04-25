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

int key;
char mode;             // Flag indicating whether to encrypt or decrypt
int current_index;     // Index of the current value being read
int data_index;        // Index of the next value to be processed

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; 
pthread_cond_t condition_variable = PTHREAD_COND_INITIALIZER; 
// The task that will be executed by the thread pool
void new_task(void *arg)
{
    Pinput data = (Pinput) arg;
    if (mode == 'e') {
        encrypt(data->value, key);
    } else if (mode == 'd') {
        decrypt(data->value, key);
    }
    // Wait until the current data item is the one that needs to be processed
    pthread_mutex_lock(&mutex);
    while (data->index != data_index) {
        pthread_cond_wait(&condition_variable, &mutex);
    }
    printf("%s", data->value);
    data_index++;
    // Broadcast to all waiting threads that the next data item can be processed
    pthread_cond_broadcast(&condition_variable);
    pthread_mutex_unlock(&mutex);
    free(data->value);
    free(data);
}


int main(int argc, char *argv[]) {
    data_index = 0;
    current_index = 0;

    // Check if the program was run with the correct arguments
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <key> <-e/-d>\n", argv[0]);
        exit(1);
    }

    // Parse the encryption key and the encryption/decryption mode
    key = atoi(argv[1]);
    mode = argv[2][1];
    if (mode != 'e' && mode != 'd') {
        fprintf(stderr, "Invalid mode: %c, must be -e or -d\n", mode);
        exit(1);
    }

    // Initialize the thread pool with the number of available CPUs
    int num_threads = sysconf(_SC_NPROCESSORS_ONLN);
    threadpool thpool = thpool_init(num_threads);

    // Read data from stdin and add tasks to the thread pool
    while (1) {
        Pinput data = (Pinput) calloc(1, sizeof(data));
        if (!data) {
            perror("calloc");
            exit(1);
        }
        data->index = current_index;
        data->value = (char *) malloc(MAX_DATA_COUNT);
        if (!data->value) {
            perror("malloc");
            free(data);
            exit(1);
        }
        if (!fgets(data->value, MAX_DATA_COUNT, stdin)) {
            break;
        }
        thpool_add_work(thpool, new_task, data);
        current_index++;
    }

    // Wait for all tasks to complete and destroy the thread pool
    thpool_wait(thpool);
    thpool_destroy(thpool);
    return 0;
}
