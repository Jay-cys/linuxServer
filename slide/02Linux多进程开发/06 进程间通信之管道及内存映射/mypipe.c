#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int main(){

    int pipefd[2];
    int ret = pipe(pipefd);
    if (ret == -1){
        perror("pipe");
        exit(0);
    }

    __pid_t pid = fork();
    if (pid > 0){
        char buff[1024];
        int len = read(pipefd[0], buff, sizeof(buff));
        printf("parent recv : %s, pid %d\n", buff, getpid());
    }
    else if (pid == 0)
    {
        char *str = "hello, i am a child";
        write(pipefd[1], str, strlen(str));
    }

    return 0;
}