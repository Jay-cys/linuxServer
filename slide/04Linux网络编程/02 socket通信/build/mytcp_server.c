#include<arpa/inet.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>


int main(){
    // 创建一个监听的套接字
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(listen_fd == -1){
        perror("socket");
        exit(-1);
    }

    // 设置本地IP和端口
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    // inet_pton(AF_INET, "127.0.0.1", &saddr.sin_addr.s_addr);
    saddr.sin_addr.s_addr = INADDR_ANY;
    saddr.sin_port = htons(9999);

    // 将监听文件描述符与本地IP和端口绑定
    int ret = bind(listen_fd, (struct sockaddr *)&saddr, sizeof(saddr));
    if(ret == -1){
        perror("bind");
        exit(-1);
    }
    // 监听
    ret = listen(listen_fd, 8);
    if(ret == -1){
        perror("listen");
        exit(-1);
    }

    // 等待连接
    struct sockaddr_in caddr;
    socklen_t len = sizeof(caddr);
    int cfd = accept(listen_fd, (struct sockaddr *)&caddr, &len);
    if(cfd == -1){
        perror("accept");
        exit(-1);
    }

    // 连接成功，获取客户端信息
    char cIP[16];
    inet_ntop(AF_INET, &caddr.sin_addr.s_addr, cIP, sizeof(cIP));
    unsigned short cport = ntohs(caddr.sin_port);
    printf("client IP is : %s, port is %d\n", cIP, cport);

    char buf[1024] = {0};
    char *data = "hello, i am server";
    // 开始通信
    while(1){
        // 读取客户端数据
        memset(buf, 0, 1024);
        ret = read(cfd, buf, sizeof(buf));
        if (ret == -1)
        {
            perror("read");
            exit(-1);
        }
        else if (ret > 0)
        {
            printf("recv data is %s\n", buf);
        }
        else
        {
            // 服务器断开连接
            printf("client closed...\n");
            break;
        }

        // 给客户端发送数据
        write(cfd, data, strlen(data));
    }


    // 关闭文件描述符
    close(cfd);
    close(listen_fd);

    return 0;
}