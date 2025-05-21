/*
** EPITECH PROJECT, 2024
** Jetpack
** File description:
** Client header
*/

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <cstring>
#include <stdexcept>
#include <vector>
#include <string>

class Game;

class Client {
private:
    int sock;
    sockaddr_in server;
    std::string ip;
    int port;
    bool debug;

    void handleCommand(std::stringstream &ss, const std::string &command, Game &game);
    void handleId(std::stringstream &ss, Game &game);
    void handlePosition(std::stringstream &ss, Game &game);
    void handleObject(std::stringstream &ss, Game &game);
    void handleStart(Game &game);
    void handleCoin(std::stringstream &ss, Game &game);
    void handleScore(std::stringstream &ss, Game &game);
    void logDebugMessage(const std::string& message);

public:
    Client(const std::string &ip, int port, bool debug = false);
    void connectToServer();
    void sendMessage(const std::string &message);
    int getSocket() const;
    void receiveUpdates(Game &game);
    void run();
    ~Client();
};

#endif // CLIENT_HPP
