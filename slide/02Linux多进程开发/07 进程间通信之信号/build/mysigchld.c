#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/wait.h>

void myfun(int num){
    printf("捕捉到的信号编号为：%d\n", num);
    while(1){
        int ret = waitpid(-1, NULL, WNOHANG);
        if (ret > 0){
            printf("child die, pid = %d\n", ret);
        }else if(ret == 0){
            // 还有子进程活着
            break;
        }else if(ret == -1){
            // 发生错误
            break;
        }
    }
}

int main(){

    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGCHLD);
    sigprocmask(SIG_BLOCK, &set, NULL);

    pid_t pid;
    for (int i = 0; i < 20; ++i)
    {
        pid = fork();
        if (pid == 0){
            break;
        }
    }

    if(pid > 0){
        // 父进程
        // 注册捕捉信号
        struct sigaction act;
        act.sa_flags = 0;
        act.sa_handler = myfun;
        sigemptyset(&act.sa_mask);
        sigaction(SIGCHLD, &act, NULL);

        // 注册完信号捕捉以后，解除阻塞
        sigprocmask(SIG_UNBLOCK, &set, NULL);

        while (1)
        {
            printf("parent process pid : %d", getpid());
            sleep(2);
        }
    }else if(pid == 0){
        // 子进程
        printf("child process pid : %d", getpid());
    }

    return 0;
}