#define MAX_DATA_COUNT 1024
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; 
pthread_cond_t condition_variable = PTHREAD_COND_INITIALIZER; 

typedef struct input {
    int index;          // Index of the data in the input stream
    char *value;        // The actual value read from input
} input, *Pinput;

typedef struct ProgramData{
    int key;
    char mode;             // Flag indicating whether to encrypt or decrypt
    int current_index;     // Index of the current value being read
    int data_index;        // Index of the next value to be processed
} ProgramData;

void new_task(void *arg);


