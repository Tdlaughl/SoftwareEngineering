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
    void setAckMessage(const std::string& attacker_player, const std::string& target_player);
    std::string getAttackerPlayer();
    std::string getTargetPlayer();
    void updateAndRender();
    void run();
    std::string attackerPlayer, targetPlayer;
};

#endif // COUNTDOWN_DISPLAY_H
