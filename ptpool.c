#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define THREAD_POOL_SIZE 5

typedef struct
{
    void (*function)(void *arg);
    void *arg;
} task_t;

typedef struct
{
    task_t *tasks;
    int head;
    int tail;
    int size;
    int count;
    pthread_mutex_t lock;
    pthread_cond_t not_empty;
    pthread_cond_t not_full;
} thread_pool_t;

void *thread_pool_worker(void *arg)
{
    thread_pool_t *pool = (thread_pool_t *)arg;
    task_t task;
    while (1)
    {
        pthread_mutex_lock(&pool->lock);
        while (pool->count == 0)
        {
            pthread_cond_wait(&pool->not_empty, &pool->lock);
        }
        task = pool->tasks[pool->head];
        pool->head = (pool->head + 1) % pool->size;
        pool->count--;
        pthread_cond_signal(&pool->not_full);
        pthread_mutex_unlock(&pool->lock);
        task.function(task.arg);
    }
    return NULL;
}

void thread_pool_init(thread_pool_t *pool, int size)
{
    pool->tasks = (task_t *)malloc(sizeof(task_t) * size);
    pool->head = 0;
    pool->tail = 0;
    pool->size = size;
    pool->count = 0;
    pthread_mutex_init(&pool->lock, NULL);
    pthread_cond_init(&pool->not_empty, NULL);
    pthread_cond_init(&pool->not_full, NULL);
    for (int i = 0; i < size; i++)
    {
        pthread_t thread;
        pthread_create(&thread, NULL, thread_pool_worker, pool);
        pthread_detach(thread);
    }
}

void thread_pool_submit(thread_pool_t *pool, void (*function)(void *arg), void *arg)
{
    pthread_mutex_lock(&pool->lock);
    while (pool->count == pool->size)
    {
        pthread_cond_wait(&pool->not_full, &pool->lock);
    }
    pool->tasks[pool->tail].function = function;
    pool->tasks[pool->tail].arg = arg;
    pool->tail = (pool->tail + 1) % pool->size;
    pool->count++;
    pthread_cond_signal(&pool->not_empty);
    pthread_mutex_unlock(&pool->lock);
}

void task_function(void *arg)
{
    int *num = (int *)arg;
    printf("Task %d started\n", *num);
    sleep(1);
    printf("Task %d finished\n", *num);
}

int main()
{
    thread_pool_t pool;
    thread_pool_init(&pool, THREAD_POOL_SIZE);
    for (int i = 0; i < 10; i++)
    {
        int *num = (int *)malloc(sizeof(int));
        *num = i;
        thread_pool_submit(&pool, task_function, num);
    }
    sleep(5);
    return 0;
}