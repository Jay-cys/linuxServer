/*
    读写锁的类型 pthread_rwlock_t
    int pthread_rwlock_init(pthread_rwlock_t *restrict rwlock, const pthread_rwlockattr_t *restrict attr);
    int pthread_rwlock_destroy(pthread_rwlock_t *rwlock);
    int pthread_rwlock_rdlock(pthread_rwlock_t *rwlock);
    int pthread_rwlock_tryrdlock(pthread_rwlock_t *rwlock);
    int pthread_rwlock_wrlock(pthread_rwlock_t *rwlock);
    int pthread_rwlock_trywrlock(pthread_rwlock_t *rwlock);
    int pthread_rwlock_unlock(pthread_rwlock_t *rwlock);

    案例：8个线程操作同一个全局变量。
    3个线程不定时写这个全局变量，5个线程不定时的读这个全局变量
*/

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int resource = 0;

pthread_rwlock_t rwlock;
void* myfun(){
    
}

int main(){
    // 初始化读写锁
    pthread_rwlock_init(&rwlock, NULL);

    pthread_t tid1, tid2, tid3, tid4, tid5, tid6, tid7, tid8;
    pthread_create(&tid1, NULL, myfun, NULL);
    pthread_create(&tid2, NULL, myfun, NULL);
    pthread_create(&tid3, NULL, myfun, NULL);
    pthread_create(&tid4, NULL, myfun, NULL);
    pthread_create(&tid5, NULL, myfun, NULL);
    pthread_create(&tid6, NULL, myfun, NULL);
    pthread_create(&tid7, NULL, myfun, NULL);
    pthread_create(&tid8, NULL, myfun, NULL);

    // 
}