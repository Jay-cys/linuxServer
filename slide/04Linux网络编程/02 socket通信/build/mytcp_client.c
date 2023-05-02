#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(){
    // 创建套接字
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd == -1){
        perror("socket");
        exit(-1);
    }

    // 连接服务器
    struct sockaddr_in saddr;
    inet_pton(AF_INET, "127.0.0.1", &saddr.sin_addr.s_addr);
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(9999);
    int ret = connect(fd, (struct sockaddr *)&saddr, sizeof(saddr));
    if(ret == -1){
        perror("connect");
        exit(-1);
    }

    char data[1024] = {0};
    char send_buf[1024] = {0};
    while(1){
        // 写数据给服务器
        memset(send_buf, 0, 1024);
        fgets(send_buf, sizeof(send_buf), stdin);
        // 粗心写成sizeof，那么就会导致遇到空格终止
        write(fd, send_buf, strlen(send_buf) + 1);  // +1 是因为需要写入结束符\0

        // 读取数据
        memset(data, 0, 1024);
        ret = read(fd, data, sizeof(data));
        if(ret == -1){
            perror("read");
            exit(-1);
        }else if(ret > 0){
            printf("recv data is %s\n", data);
        }else{
            // 服务器断开连接
            printf("server closed...\n");
            break;
        }
    }

    // 关闭文件描述符
    close(fd);

    return 0;
}