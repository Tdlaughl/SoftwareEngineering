#pragma once
#include <SFML/Graphics.hpp>
#include <sstream>
using namespace std;

class CountdownTimer {
public:
    CountdownTimer(int seconds) : timeRemaining(seconds), isRunning(false) {
        font.loadFromFile("arial.ttf"); // Load a font
        text.setFont(font);             // Set the font to our text
        text.setCharacterSize(50);      // Set the text size
        text.setFillColor(sf::Color::White);
        Restart();
    }

    void Restart() {
        isRunning = true;
        timeRemaining = startTime;
        UpdateText();
    }

    void Update(float deltaTime) {
        if (isRunning) {
            timeRemaining -= deltaTime;
            if (timeRemaining <= 0) {
                timeRemaining = 0;
                isRunning = false;
            }
            UpdateText();
        }
    }

    void UpdateText() {
        int seconds = static_cast<int>(timeRemaining);
        text.setString("CountDown: " + to_string(seconds));

        if (seconds == 0)
        {
            text.setString("Game Start");
        }

    }

    void Draw(sf::RenderWindow& window) {
        sf::Vector2u windowSize = window.getSize();
        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        text.setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);
        window.draw(text);
    }

    bool IsRunning() const {
        return isRunning;
    }

private:
    sf::Font font;
    sf::Text text;
    bool isRunning;
    float timeRemaining;
    const float startTime = 10.0f; // Initial time in seconds
};