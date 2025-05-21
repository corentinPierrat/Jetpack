/*
** EPITECH PROJECT, 2025
** jetpack
** File description:
** Server
*/

#ifndef SERVER_HPP_
#define SERVER_HPP_

#include <map>
#include <vector>
#include <poll.h>
#include <string>
#include <thread>
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <sstream>
#include <fstream>
#include <chrono>
#include "Error.hpp"
#include <mutex>
#include <cmath>

struct Objects {
    int x;
    int y;
    char type;
};

struct PlayerState {
    int id;
    int fd;
    int score = 0;
    float x = 100.0f;
    float y = 780.0f;
    float velocity = 0.0f;
    bool jetpack = false;
    std::vector<Objects> listeCoin;
    bool dead = false;
};

class Server {
public:
    Server(int port, const std::string& mapPath, bool debug);
    ~Server();

    void run();

private:
    int _port;
    int _serverFd;
    int _nextId = 0;
    std::vector<pollfd> _pollFds;
    std::map<int, PlayerState> _players;
    std::vector<Objects> _mapCoin;
    std::vector<Objects> _mapFireBall;
    std::thread _updateThread;
    std::mutex _playersMutex;
    std::string _mapPath;
    bool _debug = false;
    bool _gameStarted = false;
    int _maxX = 0;
    bool _countdownStarted = false;
    std::chrono::time_point<std::chrono::high_resolution_clock> _countdownStart;
    int _allPlayers = 0;

    void serverLoop();
    void updateLoop();
    void handleNewConnection();
    void handleClientInput(size_t index);
    void removeClient(size_t index);
    void broadcastPositions();
    void sendMap(int fd);
    void loadMap();
    bool checkCoin(const PlayerState& player, const Objects& obj);
    bool checkFireBall(const PlayerState& player, const Objects& obj);
    void handleCollisions(PlayerState& player);
    void logDebugMessage(const std::string& message);
    void checkEndMap(PlayerState& player);
    void checkOnePlayer();
};

#endif // SERVER_HPP_