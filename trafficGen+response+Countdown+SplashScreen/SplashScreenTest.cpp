#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> // Add this header for inet_pton
#include <thread>
#include "CountDownDisplay.h"
#include "response.cpp"
#include "trafficGenerator.cpp"

using namespace std;

int main() {
	//Starts the generateTraffic code which starts the "game"
	std::thread trafficThread(generateTraffic);
	
    // Window initialization
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "Splash Screen", sf::Style::Close | sf::Style::Resize);

    // Load splash screen image
    sf::Texture logo;
    if (!logo.loadFromFile("logo.jpg")) {
        cout << "Error: Image not found" << endl;
        return 1;
    }
    sf::RectangleShape ImgArea(sf::Vector2f(1000.0f, 1000.0f));
    ImgArea.setTexture(&logo);

    // Timer initialization
    sf::Clock clock;

    // Display splash screen for 5 seconds
    while (window.isOpen() && clock.getElapsedTime().asSeconds() < 5) {
        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed)
                window.close();
        }
        window.clear();
        window.draw(ImgArea);
        window.display();
    }

    // Display countdown
    CountdownDisplay countdownDisplay(window);
    countdownDisplay.run();

    // Create a thread for the response
    std::thread responseThread(response, std::ref(countdownDisplay));

    // Join the response thread with the main thread
	trafficThread.join();
    responseThread.join();

    return 0;
}
