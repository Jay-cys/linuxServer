#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

int main()
{

    // 判断有名管道文件是否存在
    int ret = access("fifo1", F_OK);
    if (ret == -1)
    {
        // 文件不存在
        printf("管道不存在，创建对应的有名管道\n");
        ret = mkfifo("fifo1", 0664);
        if (ret == -1)
        {
            perror("mkfifo");
            exit(0);
        }
    }

    ret = access("fifo2", F_OK);
    if (ret == -1)
    {
        // 文件不存在
        printf("管道不存在，创建对应的有名管道\n");
        ret = mkfifo("fifo2", 0664);
        if (ret == -1)
        {
            perror("mkfifo");
            exit(0);
        }
    }

    char buf[128];

    pid_t pid = fork();
    if (pid > 0)
    {
        // 父进程
        // 以只读的方式打开管道fifo1
        int fdr = open("fifo1", O_RDONLY);
        if (fdr == -1)
        {
            perror("open");
            exit(0);
        }
        printf("打开管道fifo1成功，等待读取...\n");


        // 循环读数据
        while (1)
        {
            // 读管道数据
            memset(buf, 0, 128);
            ret = read(fdr, buf, 128);
            if (ret <= 0)
            {
                perror("read");
                break;
            }
            printf("buf: %s\n", buf);
        }

        // 关闭文件描述符
        close(fdr);
    }
    else if (pid == 0)
    {
        // 子进程
        // 以只写的方式打开管道fifo2
        int fdw = open("fifo2", O_WRONLY);
        if (fdw == -1)
        {
            perror("open");
            exit(0);
        }
        printf("打开管道fifo2成功，等待写入...\n");

        // 循环写数据
        while (1)
        {
            memset(buf, 0, 128);
            // 获取标准输入的数据
            fgets(buf, 128, stdin);
            // 写数据
            ret = write(fdw, buf, strlen(buf));
            if (ret == -1)
            {
                perror("write");
                exit(0);
            }
        }

        // 关闭文件描述符
        close(fdw);
    }

    return 0;
}