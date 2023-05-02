/*
    #include <pthread.h>
    int pthread_detach(pthread_t thread);
        - 功能：分离一个线程。被分离的线程在终止的时候，会自动释放资源返回给系统。
          1.不能多次分离，会产生不可预料的行为。
          2.不能去连接一个已经分离的线程，会报错。
        - 参数：需要分离的线程的ID
        - 返回值：
            成功：0
            失败：返回错误号
*/

#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

void * myfun(void *arg){
    printf("child pthread : %ld\n", pthread_self());
    // return NULL;
    while(1)
        ;
}

int main(){
    pthread_t tid;
    int val = 10;
    int ret = pthread_create(&tid, NULL, myfun, (void *)&val);
    if (ret != 0){
        char *err = strerror(ret);
        printf("error : %s\n", err);
    }

    // 分离线程
    ret = pthread_detach(tid);
    if (ret != 0){
        char *err = strerror(ret);
        printf("detach error : %s\n", err);
    }

    // sleep(3);
    while(1)
        ;
    return 0;
}