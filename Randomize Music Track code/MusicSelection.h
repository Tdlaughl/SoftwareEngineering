//#pragma once
#ifndef RANDOM_MUSIC_PLAYER_H
#define RANDOM_MUSIC_PLAYER_H

#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <ctime>
using namespace std;

void playRandomMusic(const vector<string>& musicFiles) {
    sf::Music music;

    // Load music
    for (const string& filename : musicFiles) {
        if (!music.openFromFile(filename)) {
            cerr << "Failed to load " << filename << endl;
            return; // Exit the function if loading fails
        }
    }

    // Seed random number generator
    srand(static_cast<unsigned int>(time(nullptr)));

    // Play random music
    int randomIndex = rand() % musicFiles.size(); // Generate random index
    music.openFromFile(musicFiles[randomIndex]); // Load random music
    music.play(); // Play the random music

    // Wait until the music finishes
    while (music.getStatus() == sf::Music::Playing) {
        sf::sleep(sf::milliseconds(100)); // Sleep for a short duration
    }
}

#endif // RANDOM_MUSIC_PLAYER_H
