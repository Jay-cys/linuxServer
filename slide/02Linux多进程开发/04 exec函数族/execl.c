#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>

int main()
{

    // 创建一个子进程，在子进程中执行exec函数族中的函数
    pid_t pid = fork();

    if (pid > 0)
    {
        // 父进程
        printf("i am parent process, pid : %d\n", getpid());
        sleep(1);
    }
    else if (pid == 0)
    {
        // 子进程
        // char* envp[] = {"/home/u/Desktop/Linux"};
        // execle("/home/jay_cy/slide/02Linux多进程开发/04 exec函数族/hello", "hello", NULL, envp);
        execl("hello", "hello", NULL);

        printf("i am child process, pid : %d\n", getpid());
    }

    for (int i = 0; i < 3; i++)
    {
        printf("i = %d, pid = %d\n", i, getpid());
    }

    return 0;
}