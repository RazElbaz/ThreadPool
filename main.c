#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "threadpool.h"

#define MAX_BUFFER_SIZE 1024

void encode_data(void *data)
{
    /* Add code to perform encryption */
}

void decode_data(void *data)
{
    /* Add code to perform decryption */
}

int main(int argc, char **argv)
{
    int key, i;
    char buffer[MAX_BUFFER_SIZE];
    size_t len;

    /* Parse command-line arguments */
    if (argc != 3 || (strcmp(argv[1], "-e") != 0 && strcmp(argv[1], "-d") != 0)) {
        fprintf(stderr, "Usage: %s {-e|-d} key\n", argv[0]);
        return 1;
    }

    key = atoi(argv[2]);

    /* Create threadpool */
    ThreadPool *pool = threadpool_create(8);

    /* Submit tasks to threadpool */
    while ((len = fread(buffer, 1, MAX_BUFFER_SIZE, stdin)) > 0) {
        for (i = 0; i < len; i++) {
            if (strcmp(argv[1], "-e") == 0) {
                threadpool_submit(pool, encode_data, (void *)((unsigned long)buffer[i] + key));
            } else {
                threadpool_submit(pool, decode_data, (void *)((unsigned long)buffer[i] - key));
            }
        }
    }

    /* Destroy threadpool */
    threadpool_destroy(pool);

    return 0;
}
