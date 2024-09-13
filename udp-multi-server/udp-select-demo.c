/**
 * Copyright (C) 2024 wwhai
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>

#define MAX_PORT 3
#define BUFFER_SIZE 1024

int main()
{
    int ports[MAX_PORT] = {7000, 7001, 7002};
    int sockets[MAX_PORT];
    struct sockaddr_in server_addr[MAX_PORT];
    fd_set readfds;
    int max_sd, activity, i, sd, recv_len;
    char buffer[BUFFER_SIZE];
    socklen_t addr_len = sizeof(struct sockaddr_in);

    // Create UDP sockets and bind to ports
    for (i = 0; i < MAX_PORT; i++)
    {
        if ((sockets[i] = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
        {
            perror("socket");
            exit(EXIT_FAILURE);
        }

        memset(&server_addr[i], 0, sizeof(server_addr[i]));
        server_addr[i].sin_family = AF_INET;
        server_addr[i].sin_addr.s_addr = htonl(INADDR_ANY);
        server_addr[i].sin_port = htons(ports[i]);

        if (bind(sockets[i], (struct sockaddr *)&server_addr[i], sizeof(server_addr[i])) == -1)
        {
            perror("bind");
            exit(EXIT_FAILURE);
        }
    }

    printf("UDP Server started\n");

    while (1)
    {
        FD_ZERO(&readfds);

        // Add sockets to set
        max_sd = 0;
        for (i = 0; i < MAX_PORT; i++)
        {
            FD_SET(sockets[i], &readfds);
            if (sockets[i] > max_sd)
            {
                max_sd = sockets[i];
            }
        }

        // Wait for activity on sockets
        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
        if (activity == -1)
        {
            perror("select");
            exit(EXIT_FAILURE);
        }

        // Check each socket for activity
        for (i = 0; i < MAX_PORT; i++)
        {
            if (FD_ISSET(sockets[i], &readfds))
            {
                // Receive data from client
                if ((recv_len = recvfrom(sockets[i], buffer, BUFFER_SIZE, 0,
                                         (struct sockaddr *)&server_addr[i], &addr_len)) == -1)
                {
                    perror("recvfrom");
                    exit(EXIT_FAILURE);
                }

                buffer[recv_len] = '\0';
                Type = buffer [0:2];
                if (Type == 0x0001)
                {
                    pthread_create(handler_1, buffer);
                }
                if (Type == 0x0001)
                {
                    pthread_create(handler_2, buffer);
                }
            }
        }
    }

    // Close sockets
    for (i = 0; i < MAX_PORT; i++)
    {
        close(sockets[i]);
    }

    return 0;
}
