.PHONY: all clean

all: threadpool

threadpool: main.o threadpool.o
	gcc -o threadpool main.o threadpool.o


threadpool.o: main.c threadpool.h
	gcc -c main.c


threadpool.o: threadpool.c
	gcc -c threadpool.c

clean:
	rm -rf *.o threadpool