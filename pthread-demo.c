#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *print_numbers(void *arg)
{
    int i;
    for (i = 1; i <= 10; i++)
    {
        printf("%d ", i);
    }
    printf("\n");
    return NULL;
}

int main()
{
    pthread_t thread1, thread2;

    // 创建线程1
    if (pthread_create(&thread1, NULL, print_numbers, NULL))
    {
        fprintf(stderr, "Error creating thread1\n");
        return 1;
    }

    // 创建线程2
    if (pthread_create(&thread2, NULL, print_numbers, NULL))
    {
        fprintf(stderr, "Error creating thread2\n");
        return 1;
    }

    // 等待线程1和线程2结束
    if (pthread_join(thread1, NULL))
    {
        fprintf(stderr, "Error joining thread1\n");
        return 2;
    }
    if (pthread_join(thread2, NULL))
    {
        fprintf(stderr, "Error joining thread2\n");
        return 2;
    }

    return 0;
}