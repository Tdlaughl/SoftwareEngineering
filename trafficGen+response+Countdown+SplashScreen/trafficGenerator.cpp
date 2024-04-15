
int generateTraffic(){
const int bufferSize = 1024;
const int serverPort = 7500;
const int clientPort = 7501;
const char* serverIPAddress = "127.0.0.1";
    std::cout << "This program will generate some test traffic for 2 players on the red team as well as 2 players on the green team" << std::endl;
    std::cout << std::endl;

    std::string red1, red2, green1, green2;
    std::cout << "Enter equipment id of red player 1 ==> ";
    std::cin >> red1;
    std::cout << "Enter equipment id of red player 2 ==> ";
    std::cin >> red2;
    std::cout << "Enter equipment id of green player 1 ==> ";
    std::cin >> green1;
    std::cout << "Enter equipment id of green player 2 ==> ";
    std::cin >> green2;

    // Create UDP sockets
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;

    serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (serverSocket < 0) {
        std::cerr << "Error in socket creation" << std::endl;
        return 1;
    }

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
    inet_pton(AF_INET, serverIPAddress, &clientAddr.sin_addr); // Use inet_pton instead

    // Bind server socket
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Error in binding" << std::endl;
        return 1;
    }

    std::cout << std::endl;
    std::cout << "Waiting for start from game_software" << std::endl;

    // Wait for start from game software
    std::string received_data;

    while (received_data != "202") {
        char buffer[bufferSize];
        socklen_t len = sizeof(clientAddr);
        recvfrom(serverSocket, buffer, bufferSize, 0, (struct sockaddr*)&clientAddr, &len);
        received_data = buffer;
        std::cout << "Received from game software: " << received_data << std::endl;
    }
    std::cout << std::endl;

    // Create events, random player, and order
    int counter = 0;
    srand(time(0));

    while (true) {
        sleep(1);
        std::string message;
        std::string redplayer, greenplayer;

        if (rand() % 2 == 0)
            redplayer = red1;
        else
            redplayer = red2;

        if (rand() % 2 == 0)
            greenplayer = green1;
        else
            greenplayer = green2;

        if (rand() % 2 == 0)
            message = redplayer + ":" + greenplayer;
        else
            message = greenplayer + ":" + redplayer;

        // After 10 iterations, send base hit
        if (counter == 10)
            message = redplayer + ":43";
        if (counter == 20)
            message = greenplayer + ":53";

        std::cout << "Transmitting to game: " << message << std::endl;

        sendto(clientSocket, message.c_str(), message.length(), 0, (struct sockaddr*)&clientAddr, sizeof(clientAddr));
        

        // Receive answer from game software
        char buffer[bufferSize];
        socklen_t len = sizeof(clientAddr);
        recvfrom(serverSocket, buffer, bufferSize, 0, (struct sockaddr*)&clientAddr, &len);
        received_data = buffer;
        std::cout << "Received from game software: " << received_data << std::endl;
        std::cout << std::endl;

        counter++;
        if (received_data == "221")
            break;

        usleep((rand() % 3 + 1) * 1000000); // sleep for 1-3 seconds
    }

    std::cout << "Program complete" << std::endl;

    // Close sockets
    close(serverSocket);
    close(clientSocket);

return 0;
}
