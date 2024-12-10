#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#define LABEL_SIZE 100
#define MAX_BOXES 10
#define QUEUE_SIZE 10

// 定义 Box 结构体，存储对象信息
typedef struct
{
    char ok;
    int x, y, w, h;
    float prop;
    char label[LABEL_SIZE];
} Box;

// 定义 QueueData 结构体，存储帧及检测到的对象信息
typedef struct
{
    // 假设 AVFrame 是一个已经定义的结构体，用于存储解码后的帧
    // AVFrame *frame;
    Box boxes[MAX_BOXES];
} QueueData;

// 定义队列结构体，包含数据、队列头、尾、元素计数和同步机制
typedef struct
{
    QueueData data[QUEUE_SIZE];
    int front;
    int rear;
    int count;
    pthread_mutex_t mutex;
    pthread_cond_t not_full;
    pthread_cond_t not_empty;
} Queue;

// 初始化队列
void queue_init(Queue *q)
{
    q->front = 0;
    q->rear = 0;
    q->count = 0;
    pthread_mutex_init(&q->mutex, NULL);
    pthread_cond_init(&q->not_full, NULL);
    pthread_cond_init(&q->not_empty, NULL);
}

// 销毁队列
void queue_destroy(Queue *q)
{
    pthread_mutex_destroy(&q->mutex);
    pthread_cond_destroy(&q->not_full);
    pthread_cond_destroy(&q->not_empty);
}

// 入队操作
void queue_push(Queue *q, QueueData item)
{
    pthread_mutex_lock(&q->mutex);
    // 当队列满时，等待 not_full 条件变量
    while (q->count == QUEUE_SIZE)
    {
        pthread_cond_wait(&q->not_full, &q->mutex);
    }
    q->data[q->rear] = item;
    q->rear = (q->rear + 1) % QUEUE_SIZE;
    q->count++;
    // 通知 not_empty 条件变量
    pthread_cond_signal(&q->not_empty);
    pthread_mutex_unlock(&q->mutex);
}

// 出队操作
QueueData queue_pop(Queue *q)
{
    pthread_mutex_lock(&q->mutex);
    // 当队列空时，等待 not_empty 条件变量
    while (q->count == 0)
    {
        pthread_cond_wait(&q->not_empty, &q->mutex);
    }
    QueueData item = q->data[q->front];
    q->front = (q->front + 1) % QUEUE_SIZE;
    q->count--;
    // 通知 not_full 条件变量
    pthread_cond_signal(&q->not_full);
    pthread_mutex_unlock(&q->mutex);
    return item;
}

// 生产者线程函数
void *producer(void *arg)
{
    Queue *queue = (Queue *)arg;
    QueueData data;
    for (int i = 0; i < 50; ++i)
    {
        // 填充数据，这里简单示例，可根据实际需求修改
        for (int j = 0; j < MAX_BOXES; ++j)
        {
            data.boxes[j].ok = 'Y';
            data.boxes[j].x = i + j;
            data.boxes[j].y = i + j;
            data.boxes[j].w = i + j;
            data.boxes[j].h = i + j;
            data.boxes[j].prop = (float)(i + j);
            snprintf(data.boxes[j].label, LABEL_SIZE, "Object %d", i + j);
        }
        queue_push(queue, data);
        printf("Producer: Pushed data %d\n", i);
    }
    return NULL;
}

// 消费者线程函数
void *consumer(void *arg)
{
    Queue *queue = (Queue *)arg;
    QueueData data;
    for (int i = 0; i < 50; ++i)
    {
        data = queue_pop(queue);
        // 处理数据，这里简单示例，可根据实际需求修改
        for (int j = 0; j < MAX_BOXES; ++j)
        {
            printf("Consumer: Received data - x: %d, y: %d, w: %d, h: %d, prop: %f, label: %s\n",
                   data.boxes[j].x, data.boxes[j].y, data.boxes[j].w, data.boxes[j].h, data.boxes[j].prop, data.boxes[j].label);
        }
    }
    return NULL;
}

int main()
{
    Queue queue;
    queue_init(&queue);

    pthread_t producer_thread, consumer_thread;
    // 创建生产者线程
    if (pthread_create(&producer_thread, NULL, producer, &queue) != 0)
    {
        perror("pthread_create producer");
        exit(EXIT_FAILURE);
    }
    // 创建消费者线程
    if (pthread_create(&consumer_thread, NULL, consumer, &queue) != 0)
    {
        perror("pthread_create consumer");
        exit(EXIT_FAILURE);
    }

    // 等待生产者线程结束
    if (pthread_join(producer_thread, NULL) != 0)
    {
        perror("pthread_join producer");
        exit(EXIT_FAILURE);
    }
    // 等待消费者线程结束
    if (pthread_join(consumer_thread, NULL) != 0)
    {
        perror("pthread_join consumer");
        exit(EXIT_FAILURE);
    }

    queue_destroy(&queue);
    return 0;
}