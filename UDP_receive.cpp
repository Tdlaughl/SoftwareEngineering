#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <arpa/inet.h>

using namespace std;

#define PORT 7501 // Port Number for receiving

int main() {
    int sockfd;
    struct sockaddr_in my_addr;
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(struct sockaddr_in);
    char buffer[1024];

    // Create a UDP socket
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("unable to create socket\n");
        exit(1);
    }

    // Intializing server's sockaddr_in address structure
    memset(&my_addr, 0, sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(PORT);
	my_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind socket to address
    if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(my_addr)) < 0) {
        perror("Bind failed");
        exit(1);
    }

    printf("Listening for UDP broacasts on %d...\n", PORT);

    // Listening Loop
    while (1)
    {
        // Receiving data from clients
        ssize_t num_bytes = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&client_addr, &addr_len);
        if (num_bytes < 0)
        {
            perror("Recieve Failed");
            exit(1);
        }

        // Printing said data
        buffer[num_bytes] = '\0'; // Null-terminate the received data
        printf("Received from %s:%d: %s\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), buffer);
    }

    // Closing socket
    close(sockfd);

    return 0;
}