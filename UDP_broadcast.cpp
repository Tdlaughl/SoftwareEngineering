#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <arpa/inet.h>

using namespace std;

#define PORT 7500 // Port Number for broadcasting
#define NETWORK_ADDRESS "192.168.1.100" // Network address from PP
                                        // Introduction

int main() {
    int sockfd;
    struct sockaddr_in broadcast_addr;
    char broadcast_message[] = "Hello from UDP Broadcast.";

    // Create a UDP socket
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("unable to create socket\n");
        exit(1);
    }

    // For network Broadcasting, it makes more sense to use setsockopt()
    // For more local addresses, bind() generally works
    int broadcast_enable = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcast_enable, sizeof(broadcast_enable)) == -1 ) {
        perror("unable to enable broadcasts\n");
        exit(1);
    }

    // Intializing broadcast address structure
    memset(&broadcast_addr, 0, sizeof(broadcast_addr));
    broadcast_addr.sin_family = AF_INET;
    broadcast_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, NETWORK_ADDRESS, &broadcast_addr.sin_addr) <= 0) {
        perror("inet_pton");
        exit(1);
    }
    
    // Sending broadcast message
    ssize_t bytes_sent = sendto(sockfd, broadcast_message, strlen(broadcast_message), 0,
                                (struct sockaddr*)&broadcast_addr, sizeof(broadcast_addr));

    if (bytes_sent == -1) {
        perror("sendto");
    } else {
        printf("Sent %ld bytes to %s:%d\n", bytes_sent, NETWORK_ADDRESS, PORT);
    }
    
    // Closing socket
    close(sockfd);

    return 0;
}