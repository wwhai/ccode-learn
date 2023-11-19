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

    // ��ʼ�� Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        perror("�޷���ʼ�� Winsock");
        return EXIT_FAILURE;
    }

    // �����׽���
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET)
    {
        perror("�޷������׽���");
        WSACleanup();
        return EXIT_FAILURE;
    }

    // ���÷�������ַ
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(PORT);

    // ���׽��ֵ�ָ���˿�
    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
    {
        perror("���׽���ʧ��");
        closesocket(serverSocket);
        WSACleanup();
        return EXIT_FAILURE;
    }

    // ����ָ���˿�
    if (listen(serverSocket, 5) == SOCKET_ERROR)
    {
        perror("�����˿�ʧ��");
        closesocket(serverSocket);
        WSACleanup();
        return EXIT_FAILURE;
    }

    printf("�����������������ڼ����˿� %d ...\n", PORT);

    while (1)
    {
        // ���ܿͻ�������
        clientLength = sizeof(clientAddress);
        clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientLength);
        if (clientSocket == INVALID_SOCKET)
        {
            perror("���ܿͻ�������ʧ��");
            closesocket(serverSocket);
            WSACleanup();
            return EXIT_FAILURE;
        }

        printf("�ͻ������ӳɹ���IP: %s���˿�: %d\n", inet_ntoa(clientAddress.sin_addr), ntohs(clientAddress.sin_port));

        // ���ղ���ӡ����
        int bytesRead;
        while ((bytesRead = recv(clientSocket, buffer, BUFFER_SIZE, 0)) > 0)
        {
            printf("Received: %.*s\n", bytesRead, buffer);
        }
        if (bytesRead == SOCKET_ERROR)
        {
            perror("��������ʧ��");
            closesocket(clientSocket);
            closesocket(serverSocket);
            WSACleanup();
            return EXIT_FAILURE;
        }

        // �رտͻ����׽���
        closesocket(clientSocket);

        printf("�ͻ��������ѹر�\n");
    }

    // �رշ������׽���
    closesocket(serverSocket);

    // ���� Winsock
    WSACleanup();

    return 0;
}