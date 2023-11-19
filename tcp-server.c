#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <locale.h>
#define PORT 6005
#define BUFFER_SIZE 1024

int main()
{
    setlocale(LC_ALL, "zh_CN.UTF-8");
    WSADATA wsaData;
    SOCKET serverSocket, clientSocket;
    struct sockaddr_in serverAddress, clientAddress;
    int clientLength;
    char buffer[BUFFER_SIZE];

    // 初始化 Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        perror("无法初始化 Winsock");
        return EXIT_FAILURE;
    }

    // 创建套接字
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET)
    {
        perror("无法创建套接字");
        WSACleanup();
        return EXIT_FAILURE;
    }

    // 设置服务器地址
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(PORT);

    // 绑定套接字到指定端口
    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
    {
        perror("绑定套接字失败");
        closesocket(serverSocket);
        WSACleanup();
        return EXIT_FAILURE;
    }

    // 监听指定端口
    if (listen(serverSocket, 5) == SOCKET_ERROR)
    {
        perror("监听端口失败");
        closesocket(serverSocket);
        WSACleanup();
        return EXIT_FAILURE;
    }

    printf("服务器已启动，正在监听端口 %d ...\n", PORT);

    while (1)
    {
        // 接受客户端连接
        clientLength = sizeof(clientAddress);
        clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientLength);
        if (clientSocket == INVALID_SOCKET)
        {
            perror("接受客户端连接失败");
            closesocket(serverSocket);
            WSACleanup();
            return EXIT_FAILURE;
        }

        printf("客户端连接成功，IP: %s，端口: %d\n", inet_ntoa(clientAddress.sin_addr), ntohs(clientAddress.sin_port));

        // 接收并打印数据
        int bytesRead;
        while ((bytesRead = recv(clientSocket, buffer, BUFFER_SIZE, 0)) > 0)
        {
            printf("Received: %.*s\n", bytesRead, buffer);
        }
        if (bytesRead == SOCKET_ERROR)
        {
            perror("接收数据失败");
            closesocket(clientSocket);
            closesocket(serverSocket);
            WSACleanup();
            return EXIT_FAILURE;
        }

        // 关闭客户端套接字
        closesocket(clientSocket);

        printf("客户端连接已关闭\n");
    }

    // 关闭服务器套接字
    closesocket(serverSocket);

    // 清理 Winsock
    WSACleanup();

    return 0;
}