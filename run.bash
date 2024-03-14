#!/bin/bash

# Update package lists
sudo apt update
sleep 5

# Install SFML
sudo apt-get install -y libsfml-dev
sleep 5

# Install nlohmann-json library
sudo apt install -y nlohmann-json3-dev
sleep 5

# Install g++
sudo apt install -y g++
sleep 5

# Install libcurl
sudo apt-get install -y libcurl4-openssl-dev
sleep 5

# Install Microsoft TrueType core fonts
sudo apt install -y ttf-mscorefonts-installer
sudo fc-cache -f
sleep 5
