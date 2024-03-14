#!/bin/bash

# Update package lists
sudo apt update

# Install SFML
sudo apt-get install -y libsfml-dev

# Install nlohmann-json library
sudo apt install -y nlohmann-json3-dev

# Install g++ (if not installed)
sudo apt install -y g++

# Install libcurl (if not installed)
sudo apt-get install -y libcurl4-openssl-dev

# Install Microsoft TrueType core fonts (if not installed)
sudo apt install -y ttf-mscorefonts-installer
sudo fc-cache -f
