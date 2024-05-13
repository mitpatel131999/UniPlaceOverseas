#!/bin/bash

# Install g++ if not installed
if ! command -v g++ &> /dev/null
then
    echo "g++ could not be found, installing g++"
    sudo apt-get update && sudo apt-get install g++ -y
fi

# Install Boost Libraries required by WebSocket++
if ! dpkg -s libboost-all-dev &>/dev/null; then
    echo "Boost libraries could not be found, installing Boost"
    sudo apt-get install libboost-all-dev -y
fi

# Check if WebSocket++ is already downloaded
if [ ! -d "websocketpp" ]; then
    echo "Downloading WebSocket++"
    git clone https://github.com/zaphoyd/websocketpp.git
fi

# Compile the C++ code
echo "Compiling the signaling server..."
g++ -std=c++11 -o SignalingServer SignalingServer.cpp -I websocketpp -lboost_system -pthread

# Check if compilation was successful
if [ $? -eq 0 ]; then
    echo "Compilation successful. Running the server..."
    ./SignalingServer
else
    echo "Compilation failed. Please check the errors."
fi
