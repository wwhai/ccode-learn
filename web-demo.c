#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFFER_SIZE 4096

void handle_client(int client_sock);

int main(int argc, char *argv[])
{
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    unsigned int client_len;

    // 创建套接字
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // 设置服务器地址
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(8080);

    // 绑定套接字到地址
    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    // 开始监听
    if (listen(server_sock, 5) == -1)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("HTTP server listening on port %d...\n", ntohs(server_addr.sin_port));

    // 接收客户端连接并处理请求
    while (1)
    {
        client_len = sizeof(client_addr);
        client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_len);
        if (client_sock == -1)
        {
            perror("accept");
            continue;
        }
        handle_client(client_sock);
        close(client_sock);
    }

    close(server_sock);
    return 0;
}

void handle_client(int client_sock)
{
    char buffer[BUFFER_SIZE];
    int bytes_read;

    // 读取客户端请求
    bytes_read = read(client_sock, buffer, BUFFER_SIZE - 1);
    if (bytes_read == -1)
    {
        perror("read");
        return;
    }

    // 添加字符串结束符
    buffer[bytes_read] = '\0';

    // 打印请求
    printf("Client request:\n%s\n", buffer);

    // 发送响应
    const char *response = "HTTP/1.1 200 OK\r\n"
                           "Content-Type: text/plain\r\n"
                           "Content-Length: 12\r\n"
                           "\r\n"
                           "Hello, world";
    if (write(client_sock, response, strlen(response)) == -1)
    {
        perror("write");
        return;
    }
}