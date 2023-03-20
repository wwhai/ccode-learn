#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_EVENTS 10
#define BUF_SIZE 1024

int main(int argc, char *argv[])
{
    int server_fd, client_fd, epoll_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    struct epoll_event ev, events[MAX_EVENTS];
    char buf[BUF_SIZE];

    // create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1)
    {
        perror("socket");
        exit(1);
    }

    // bind address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(8888);
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("bind");
        exit(1);
    }

    // listen for connections
    if (listen(server_fd, 5) == -1)
    {
        perror("listen");
        exit(1);
    }

    // create epoll instance
    epoll_fd = epoll_create1(0);
    if (epoll_fd == -1)
    {
        perror("epoll_create");
        exit(1);
    }

    // add server socket to epoll
    ev.events = EPOLLIN;
    ev.data.fd = server_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &ev) == -1)
    {
        perror("epoll_ctl: server_fd");
        exit(1);
    }

    while (1)
    {
        int n, i;

        // wait for events
        n = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (n == -1)
        {
            perror("epoll_wait");
            exit(1);
        }

        // handle events
        for (i = 0; i < n; i++)
        {
            if (events[i].data.fd == server_fd)
            {
                // handle new connection
                client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
                if (client_fd == -1)
                {
                    perror("accept");
                    exit(1);
                }
                printf("New client connected: %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

                // add client socket to epoll
                ev.events = EPOLLIN;
                ev.data.fd = client_fd;
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &ev) == -1)
                {
                    perror("epoll_ctl: client_fd");
                    exit(1);
                }
            }
            else
            {
                // handle client data
                int bytes = recv(events[i].data.fd, buf, BUF_SIZE, 0);
                if (bytes == -1)
                {
                    perror("recv");
                    exit(1);
                }
                else if (bytes == 0)
                {
                    printf("Client disconnected\n");
                    close(events[i].data.fd);
                }
                else
                {
                    printf("Received %d bytes from client: %.*s", bytes, bytes, buf);
                }
            }
        }
    }

    return 0;
}