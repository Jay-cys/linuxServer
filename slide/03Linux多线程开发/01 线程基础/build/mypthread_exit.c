/*
    #include <pthread.h>
    int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
    void *(*start_routine) (void *), void *arg);

        - 功能：创建一个子线程
        - 参数：
            - thread：传出参数，线程创建成功后，子线程的线程ID被写到该变量中。
            - attr : 设置线程的属性，一般使用默认值，NULL
            - start_routine : 函数指针，这个函数是子线程需要处理的逻辑代码
            - arg : 给第三个参数使用，传参
        - 返回值：
            成功：0
            失败：返回错误号。这个错误号和之前errno不太一样。
            获取错误号的信息：  char * strerror(int errnum);

    #include <pthread.h>
    void pthread_exit(void *retval);
        功能：终止一个线程，在哪个线程中调用，就表示终止哪个线程
        参数：
            retval:需要传递一个指针，作为一个返回值，可以在pthread_join()中获取到。

    pthread_t pthread_self(void);
        功能：获取当前的线程的线程ID

    int pthread_equal(pthread_t t1, pthread_t t2);
        功能：比较两个线程ID是否相等
        不同的操作系统，pthread_t类型的实现不一样，有的是无符号的长整型，有的
        是使用结构体去实现的。
*/

#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

void * myfun(void *arg){
    printf("child pthread : %ld\n", pthread_self());
    printf("num is %d", *(int *)arg);
    // exit(0);
    // int ret;
    // pthread_exit((void *)&ret);
    while(1)
        ;
}

int main(){
    // 创建一个线程
    pthread_t tid;
    int num = 10;
    int ret = pthread_create(&tid, NULL, myfun, (void *)&num);
    if(ret != 0){
        char *err = strerror(ret);
        printf("%s\n", err);
    }

    for (int i = 0; i < 5; ++i){
        printf("%d\n", i);
    }
    while(1);

    return 0;
}