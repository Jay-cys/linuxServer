/*
    信号量的类型 sem_t
    int sem_init(sem_t *sem, int pshared, unsigned int value);
        - 初始化信号量
        - 参数：
            - sem : 信号量变量的地址
            - pshared : 0 用在线程间 ，非0 用在进程间
            - value : 信号量中的值

    int sem_destroy(sem_t *sem);
        - 释放资源

    int sem_wait(sem_t *sem);
        - 对信号量加锁，调用一次对信号量的值-1，如果值为0，就阻塞

    int sem_trywait(sem_t *sem);

    int sem_timedwait(sem_t *sem, const struct timespec *abs_timeout);
    int sem_post(sem_t *sem);
        - 对信号量解锁，调用一次对信号量的值+1

    int sem_getvalue(sem_t *sem, int *sval);

*/
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

struct Node
{
    int num;
    struct Node *next;
};

struct Node *head = NULL;

// 创建互斥量
pthread_mutex_t mutex;
// 创建信号量
sem_t psem, csem;

void *producer(void *arg)
{
    while (1)
    {
        // 必须先判断信号量再上锁，否则会出现死锁现象
        sem_wait(&psem);
        pthread_mutex_lock(&mutex);
        struct Node *newNode = (struct Node *)malloc(sizeof(head));
        newNode->next = head;
        newNode->num = rand() % 10000;
        head = newNode;
        printf("add Node %d, ptid %ld\n", newNode->num, pthread_self());
        pthread_mutex_unlock(&mutex);
        sem_post(&csem);
    }
}

void *custumer(void *arg)
{
    while (1)
    {
        // 必须先判断信号量再上锁，否则会出现死锁现象
        sem_wait(&csem);
        pthread_mutex_lock(&mutex);
        struct Node *temp = head;
        head = head->next;
        printf("del Node %d, ctid %ld\n", temp->num, pthread_self());
        free(temp);
        pthread_mutex_unlock(&mutex);
        sem_post(&psem);
        // usleep(100);
    }
}

int main()
{
    // 初始化互斥量
    pthread_mutex_init(&mutex, NULL);
    // 初始化信号量
    sem_init(&psem, 0, 8);
    sem_init(&csem, 0, 0);

    pthread_t ptid[5], ctid[5];

    // 创建5个生产者线程和消费者线程
    for (int i = 0; i < 5; ++i)
    {
        pthread_create(&ptid[i], NULL, producer, NULL);
        pthread_create(&ctid[i], NULL, custumer, NULL);
    }

    // 线程脱离
    for (int i = 0; i < 5; ++i)
    {
        pthread_detach(ptid[i]);
        pthread_detach(ctid[i]);
    }

    // 死循环，确保互斥量不被回收，子线程正常运行
    while (1)
        ;

    // 回收信号量
    sem_destroy(&psem);
    sem_destroy(&csem);
    // 回收互斥量
    pthread_mutex_destroy(&mutex);

    // 主线程退出
    pthread_exit(NULL);

    return 0;
}