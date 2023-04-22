#ifndef THREADPOOL_H
#define THREADPOOL_H

typedef struct threadpool ThreadPool;

ThreadPool *threadpool_create(int num_threads);

void threadpool_destroy(ThreadPool *pool);

void threadpool_submit(ThreadPool *pool, void (*function)(void *), void *data);

#endif /* THREADPOOL_H */
