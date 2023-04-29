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

// Helper function to broadcast to all waiting threads that the next data item can be processed
void notify_next_data() ;
// Helper function to wait until the current data item is the one that needs to be processed
void wait_for_data_index(Pinput data);
// Helper function to process the data (either encryption or decryption)
void process_data(Pinput data);
// The task that will be executed by the threadpool
void new_task(void *arg);
// Helper function to allocate and read data from stdin
Pinput allocate_and_read_data();
// Helper function to initialize the thread pool
threadpool initialize_threadpool();
// Helper function to parse the encryption key and mode from command-line arguments
void parse_arguments(int argc, char *argv[]);