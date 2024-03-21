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
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define NUM_SERVERS 3
#define PORT_BASE 7000
#define BUFFER_SIZE 1024

// Function to handle UDP server
void *udp_server(void *port_ptr)
{
    int port = *((int *)port_ptr);
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE];
    int bytes_received;
    socklen_t client_addr_len = sizeof(client_addr);

    // Create UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Initialize server address struct
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    // Bind socket to port
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("Error binding socket");
        exit(EXIT_FAILURE);
    }

    printf("UDP server running on port %d...\n", port);

    while (1)
    {
        // Receive data from client
        bytes_received = recvfrom(sockfd, buffer, BUFFER_SIZE, 0,
                                  (struct sockaddr *)&client_addr, &client_addr_len);
        if (bytes_received == -1)
        {
            perror("Error receiving data");
            exit(EXIT_FAILURE);
        }

        // Print received data
        buffer[bytes_received] = '\0'; // Null-terminate the received data
        printf("Server on port %d received: %s", port, buffer);
    }

    // Close socket
    close(sockfd);

    return NULL;
}

int main()
{
    pthread_t threads[NUM_SERVERS];
    int ports[NUM_SERVERS];

    // Initialize ports
    for (int i = 0; i < NUM_SERVERS; ++i)
    {
        ports[i] = PORT_BASE + i;
    }

    // Create UDP server threads
    for (int i = 0; i < NUM_SERVERS; ++i)
    {
        if (pthread_create(&threads[i], NULL, udp_server, &ports[i]) != 0)
        {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_SERVERS; ++i)
    {
        if (pthread_join(threads[i], NULL) != 0)
        {
            perror("pthread_join");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}
