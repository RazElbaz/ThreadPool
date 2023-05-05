CC=gcc
CFLAGS=-Wall -pthread -lpthread
OBJ=threadpool.o task.o semaphore.o

export LD_LIBRARY_PATH=./bin:$LD_LIBRARY_PATH
all: coder

coder: codec.h $(OBJ)
	$(CC) $(CFLAGS) main.c -L. -lCodec -o coder $(OBJ)

semaphore.o: semaphore.c semaphore.h
	$(CC) -c semaphore.c $(CFLAGS)

task.o: task.c task.h
	$(CC) -c task.c $(CFLAGS)

threadpool.o: threadpool.c threadpool.h
	$(CC) -c threadpool.c $(CFLAGS)

main.o: main.c threadpool.h task.h semaphore.h thread.h
	$(CC) -c main.c $(CFLAGS)

clean:
	rm -f coder $(OBJ)
