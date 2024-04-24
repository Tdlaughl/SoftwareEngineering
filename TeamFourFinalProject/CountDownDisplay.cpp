// CountDownDisplay.cpp

#include "CountDownDisplay.h"
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
#include "response.h"
#include "trafficGenerator.h"

CountdownDisplay::CountdownDisplay(sf::RenderWindow& win) : window(win), countdown(30) {
    // Load textures
    for (int i = 0; i <= 30; ++i) {
        std::string filename = "countdownImages/" + std::to_string(i) + ".jpg";
        if (!textures[i].loadFromFile(filename)) {
            std::cout << "Error loading texture: " << filename << std::endl;
            return;
        }
    }
    sprite.setTexture(textures[countdown]);
}

void CountdownDisplay::setAckMessage(const std::string& attacker_player, const std::string& target_player) {
    attackerPlayer = attacker_player;
    targetPlayer = target_player;
}

std::string CountdownDisplay::getAttackerPlayer()
{
    return attackerPlayer;
}

std::string CountdownDisplay::getTargetPlayer()
{
    return targetPlayer;
}

void CountdownDisplay::updateAndRender() {
    if (!ackMessage.empty()) {
        // Render acknowledgment message
        sf::Font font;
        if (!font.loadFromFile("arial.ttf")) {
            std::cerr << "Error loading font file" << std::endl;
            return;
        }
        sf::Text text;
        text.setFont(font);
        text.setString(ackMessage);
        text.setCharacterSize(24);
        text.setFillColor(sf::Color::White);
        text.setPosition(50, 50); // Adjust position as needed
        // Render acknowledgment message on the window
        window.draw(text);
    }
}

void CountdownDisplay::run() {
    while (window.isOpen() && countdown >= 0) {
        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed)
                window.close();
        }
        if (clock.getElapsedTime().asSeconds() >= 1.0f) {
            sprite.setTexture(textures[countdown]);
            window.clear();
            window.draw(sprite);
            window.display();
            --countdown;
            clock.restart();
        }
        if (countdown == 0) {
            // std::thread trafficThread(generateTraffic);
            // sf::sleep(sf::seconds(10));
            // std::thread responseThread(response);
            // // Join the threads with the main thread
            // trafficThread.join();
            // responseThread.join();
        }
        updateAndRender();
    }
}
