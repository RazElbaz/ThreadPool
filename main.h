#define MAX_DATA_COUNT 1024

typedef struct input {
    int index;          // Index of the data in the input stream
    char *value;        // The actual value read from input
} input, *Pinput;

void new_task(void *arg);


