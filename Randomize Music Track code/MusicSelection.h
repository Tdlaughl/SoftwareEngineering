//#pragma once
#ifndef RANDOM_MUSIC_PLAYER_H
#define RANDOM_MUSIC_PLAYER_H

#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <ctime>

void playRandomMusic(const std::vector<std::string>& musicFiles) {
    sf::Music music;

    // Load music
    for (const std::string& filename : musicFiles) {
        if (!music.openFromFile(filename)) {
            std::cerr << "Failed to load " << filename << std::endl;
            return; // Exit the function if loading fails
        }
    }

    // Seed random number generator
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // Play random music
    int randomIndex = std::rand() % musicFiles.size(); // Generate random index
    music.openFromFile(musicFiles[randomIndex]); // Load random music
    music.play(); // Play the random music

    // Wait until the music finishes
    while (music.getStatus() == sf::Music::Playing) {
        sf::sleep(sf::milliseconds(100)); // Sleep for a short duration
    }
}

#endif // RANDOM_MUSIC_PLAYER_H
