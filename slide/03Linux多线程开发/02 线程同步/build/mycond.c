/*
    条件变量的类型 pthread_cond_t
    int pthread_cond_init(pthread_cond_t *restrict cond, const pthread_condattr_t *restrict attr);
    int pthread_cond_destroy(pthread_cond_t *cond);
    int pthread_cond_wait(pthread_cond_t *restrict cond, pthread_mutex_t *restrict mutex);
        - 等待，调用了该函数，线程会阻塞。
    int pthread_cond_timedwait(pthread_cond_t *restrict cond, pthread_mutex_t *restrict mutex, const struct timespec *restrict abstime);
        - 等待多长时间，调用了这个函数，线程会阻塞，直到指定的时间结束。
    int pthread_cond_signal(pthread_cond_t *cond);
        - 唤醒一个或者多个等待的线程
    int pthread_cond_broadcast(pthread_cond_t *cond);
        - 唤醒所有的等待的线程
*/
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

// 创建条件变量
pthread_cond_t cond;
// 创建互斥量
pthread_mutex_t mutex;

struct Node
{
    int num;
    struct Node *next;
};

struct Node *head = NULL;

void *producer(void *arg)
{
    while (1)
    {
        pthread_mutex_lock(&mutex);
        struct Node * newNode = (struct Node*)malloc(sizeof(head));
        newNode->next = head;
        head = newNode;
        newNode->num = rand() % 1000;
        printf("add node : %d, ptid %ld\n", newNode->num, pthread_self());
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }
    
}

void *customer(void *arg)
{
    while(1){
        pthread_mutex_lock(&mutex);
        // 当这个函数调用阻塞的时候，会对互斥锁进行解锁，以便生产者进行生产，当不阻塞时，继续向下执行，会重新加锁。
        pthread_cond_wait(&cond, &mutex);
        struct Node * temp = head;
        head = head->next;
        printf("del node : %d, ctid %ld\n", temp->num, pthread_self());
        free(temp);
        pthread_mutex_unlock(&mutex);
    }
}

int main()
{
    // 初始化条件变量
    pthread_cond_init(&cond, NULL);
    // 初始化互斥量
    pthread_mutex_init(&mutex, NULL);

    pthread_t ptid[5], ctid[5];

    // 创建线程
    for (int i = 0; i < 5; ++i)
    {
        pthread_create(&ptid[i], NULL, producer, NULL);
        pthread_create(&ctid[i], NULL, customer, NULL);
    }
    // 分离线程
    for (int i = 0; i < 5; ++i)
    {
        pthread_detach(ptid[i]);
        pthread_detach(ctid[i]);
    }
    while(1);

    // 回收条件变量
    pthread_cond_destroy(&cond);
    // 回收互斥量
    pthread_mutex_destroy(&mutex);

    // 退出主线程
    pthread_exit(NULL);

    return 0;
}