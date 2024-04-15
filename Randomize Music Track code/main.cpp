#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <ctime> // for srand
#include "MusicSelection.h"

using namespace std;

int main() {
    // Step 1: Create a list of music files
    vector<string> musicFiles = { "track01.ogg", "track02.ogg", "track03.ogg","track04.ogg","track05.ogg","track06.ogg","track07.ogg","track08.ogg" };
    //plays the music files
    // at the 9 sec mark count down start from 5
    playRandomMusic(musicFiles); 

    return 0;
}
