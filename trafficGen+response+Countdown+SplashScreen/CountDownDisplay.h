#ifndef COUNTDOWN_DISPLAY_H
#define COUNTDOWN_DISPLAY_H

#include <SFML/Graphics.hpp>
#include <string>

class CountdownDisplay {
private:
    sf::RenderWindow& window;
    sf::Texture textures[31];
    sf::Sprite sprite;
    sf::Clock clock;
    int countdown;
    std::string ackMessage;

public:
    CountdownDisplay(sf::RenderWindow& win);
    void setAckMessage(const std::string& message);
    void updateAndRender();
    void run();
    std::string getAckMessage();
};

#endif // COUNTDOWN_DISPLAY_H
