#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

// 链表作为容器
struct Node
{
    int val;
    struct Node *next;
};

// 头结点
struct Node *head = NULL;

// 互斥量
pthread_mutex_t mutex;

// 头插法增加元素
void *producter(void *arg)
{
    while (1)
    {
        pthread_mutex_lock(&mutex);
        struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
        newNode->val = rand() % 1000;
        newNode->next = head;
        head = newNode;
        printf("add node, num : %d, tid : %ld\n", newNode->val, pthread_self());
        pthread_mutex_unlock(&mutex);
        usleep(100);
    }
    return NULL;
}

// 头删法减少元素
void *consumer(void *arg)
{
    while (1)
    {
        pthread_mutex_lock(&mutex);

        // 如果只在头结点不为空的情况下使用互斥锁会产生段错误，暂未找到原因
        if (head != NULL)
        {
            struct Node *tmp = head;
            head = head->next;
            printf("del node, num : %d, tid : %ld\n", tmp->val, pthread_self());
            free(tmp);
            tmp = NULL;
            usleep(100);
        }
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main()
{
    // 初始化互斥锁
    pthread_mutex_init(&mutex, NULL);
    // 创建5个生产者线程，和5个消费者线程
    pthread_t products[5], consumes[5];
    for (int i = 0; i < 5; i++)
    {
        pthread_create(&products[i], NULL, producter, NULL);
        pthread_create(&consumes[i], NULL, consumer, NULL);
    }

    // 分离，回收线程资源
    for (int i = 0; i < 5; i++)
    {
        pthread_detach(products[i]);
        pthread_detach(consumes[i]);
    }

    // 加while循环即可在虚拟机中生成core文件
    while (1) {
        sleep(10);
    }
    // 回收互斥锁
    pthread_mutex_destroy(&mutex);
    pthread_exit(NULL); // 回收主线程
    return 0;
}