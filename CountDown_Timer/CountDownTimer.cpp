
#include <SFML/Graphics.hpp>
#include <iostream>
#include "countdown.h"
using namespace std;



int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Countdown Timer");
    sf::Clock clock;

    CountdownTimer timer(10); // 10 seconds countdown timer
    bool timerStart = false;

    while (window.isOpen()) 
    {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F5)
            {
                //press f5 key to start countdown.
                timerStart = true;
                timer.Restart();
                cout << "timer started!" << endl;
            }
        }


        float deltaTime = clock.restart().asSeconds();

        
        if (timerStart) 
        {
            timer.Update(deltaTime);
            if (!timer.IsRunning()) {
                timerStart = false;
                //std::cout << "Timer reached zero!" << std::endl;
                // Do something when the timer reaches zero
                // For example, stop the game or trigger an event.
                // You can also restart the timer if needed.
                // timer.Restart();
                // break;
                
            }

        }
            window.clear();
            if (timer.IsRunning())
            {
                timer.Draw(window);
            }
            window.display();
        

       
    }

    return 0;
}