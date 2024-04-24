#include "CountDownDisplay.h"
#include "response.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <list>
#include <stdio.h>
#include <stdlib.h>     
#include <time.h>  
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> // Add this header for inet_pton
#include <thread>    
#include <chrono>

int response(CountdownDisplay& countdownDisplay){
const int bufferSize = 1024;
const int serverPort = 7500; // Port to send to
const int clientPort = 7501; // Port to listen on
const char* serverIPAddress = "127.0.0.1";
const int durationSeconds = 360; // Time limit of game (For testing 30, for finished project 360)

    // Create UDP socket
    int clientSocket;
    struct sockaddr_in serverAddr, clientAddr;

    clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (clientSocket < 0) {
        std::cerr << "Error in socket creation" << std::endl;
        return 1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(serverPort);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    clientAddr.sin_family = AF_INET;
    clientAddr.sin_port = htons(clientPort);
    inet_pton(AF_INET, serverIPAddress, &clientAddr.sin_addr);

    // Bind client socket
    if (bind(clientSocket, (struct sockaddr*)&clientAddr, sizeof(clientAddr)) < 0) {
        std::cerr << "Error in binding" << std::endl;
        close(clientSocket);
        return 1;
    }

    std::string start_code = "202";

    // Send start code "202" to the first program
    sendto(clientSocket, start_code.c_str(), start_code.length(), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

    // Close socket
    close(clientSocket);

    time_t startTime = time(nullptr);
    time_t currentTime;

    while (true) {
        // Get current time
        currentTime = time(nullptr);

        // Create UDP socket for receiving messages
        int serverSocket;
        struct sockaddr_in recvServerAddr;
        socklen_t serverAddrLen = sizeof(recvServerAddr);

        serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
        if (serverSocket < 0) {
            std::cerr << "Error in socket creation" << std::endl;
            return 1;
        }

        recvServerAddr.sin_family = AF_INET;
        recvServerAddr.sin_port = htons(clientPort);
        recvServerAddr.sin_addr.s_addr = INADDR_ANY;

        // Bind server socket
        if (bind(serverSocket, (struct sockaddr*)&recvServerAddr, sizeof(recvServerAddr)) < 0) {
            std::cerr << "Error in binding" << std::endl;
            close(serverSocket);
            return 1;
        }

        std::cout << "Listening for incoming messages..." << std::endl;

        // Receive message from client
        char buffer[bufferSize];
        ssize_t received_bytes = recvfrom(serverSocket, buffer, bufferSize, 0, (struct sockaddr*)&recvServerAddr, &serverAddrLen);
        if (received_bytes < 0) {
            std::cerr << "Error in receiving message" << std::endl;
            close(serverSocket);
            continue;
        }

        // Parse received message
        std::string receivedMessage(buffer, received_bytes);
        std::cout << "Received message from first program: " << receivedMessage << std::endl;

        // Process the received message (if needed)

        // Check if the message is in the correct format (e.g., "4:2")
        std::size_t delimiter_pos = receivedMessage.find(":");
        if (delimiter_pos == std::string::npos) {
            std::cerr << "Invalid message format: " << receivedMessage << std::endl;
            close(serverSocket);
            continue;
        }

        std::string attacker_player = receivedMessage.substr(0, delimiter_pos);
        std::string target_player = receivedMessage.substr(delimiter_pos + 1);

        std::string ack_message = "";
        // Acknowledge the message
        if (target_player != "43" && target_player != "53")
            ack_message = "Received hit from player " + attacker_player + " on player " + target_player;
        else
        {
            if (target_player == "43")
                ack_message = "Received hit from player " + attacker_player + " on the green base";
            else
                ack_message = "Received hit from player " + attacker_player + " on the red base";
        }
        countdownDisplay.setAckMessage(attacker_player, target_player);

        sendto(serverSocket, target_player.c_str(), target_player.length(), 0, (struct sockaddr*)&serverAddr, serverAddrLen);
        ack_message = "";

        // Check if time limit has elapsed
        if (currentTime - startTime >= durationSeconds)
        {
            std::string endCode = "221";
            sendto(clientSocket, endCode.c_str(), endCode.length(), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
            break;
        }

        // Close server socket
        close(serverSocket);

        // Simulate some processing time
        sleep(1); // Sleep for 1 second
    }

    std::cout << "Program completed." << std::endl;
    
    return 0;
}
