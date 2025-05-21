/*
** EPITECH PROJECT, 2024
** B-NWP-400-NCY-4-1-jetpack-corentin.pierrat
** File description:
** Server.cpp
*/

#include "Server.hpp"
#include <iostream>
#include <iomanip>
#include <ctime>
#include <algorithm>


Server::Server(int port, const std::string& mapPath, bool debug) : _port(port), _mapPath(mapPath), _debug(debug)
{
    _serverFd = socket(AF_INET, SOCK_STREAM, 0);
    if (_serverFd < 0)
        throw Error("Socket creation failed");

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(_port);

    if (bind(_serverFd, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
        throw Error("Bind failed");

    if (listen(_serverFd, 10) < 0)
        throw Error("Listen failed");

    _pollFds.push_back({_serverFd, POLLIN, 0});
    loadMap();

    std::cout << "Server listening on port " << _port << std::endl;

    if (_debug) {
        std::cout << "[DEBUG MODE] Debug mode is enabled. All sent/received packets will be logged." << std::endl;
    }
}

Server::~Server() {
    if (_updateThread.joinable())
        _updateThread.join();
    close(_serverFd);
    for (auto& fd : _pollFds)
        close(fd.fd);
}

void Server::run() {
    _updateThread = std::thread(&Server::updateLoop, this);
    serverLoop();
}

void Server::serverLoop() {
    while (true) {
        poll(_pollFds.data(), _pollFds.size(), 10);
        for (size_t i = 0; i < _pollFds.size(); ++i) {
            if (_pollFds[i].revents & POLLIN) {
                if (_pollFds[i].fd == _serverFd)
                    handleNewConnection();
                else
                    handleClientInput(i);
            }
        }
    }
}

void Server::handleNewConnection() {
    int clientFd = accept(_serverFd, nullptr, nullptr);
    if (clientFd < 0)
        return;

    PlayerState p;
    p.id = _nextId++;
    p.fd = clientFd;
    p.listeCoin = _mapCoin;
    _allPlayers++;
    {
        std::lock_guard<std::mutex> lock(_playersMutex);
        _players[clientFd] = p;
        _pollFds.push_back({clientFd, POLLIN, 0});
        std::string msg = "ID " + std::to_string(p.id) + "\n";
        send(clientFd, msg.c_str(), msg.length(), 0);

        if (_debug) logDebugMessage("Sent to client " + std::to_string(clientFd) + ": " + msg);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        sendMap(clientFd);

        if (_players.size() >= 2 && !_countdownStarted) {
            _countdownStarted = true;
            _countdownStart = std::chrono::high_resolution_clock::now();
        }
    }
    std::cout << "Client connected: ID " << p.id << std::endl;
}

void Server::handleClientInput(size_t index) {
    int fd = _pollFds[index].fd;
    char buffer[1024] = {0};
    int val = read(fd, buffer, sizeof(buffer) - 1);

    if (val <= 0) {
        std::cout << "Client disconnected\n";
        removeClient(index);
        return;
    }
    std::string msg(buffer);
    if (_debug) logDebugMessage("Received from client " + std::to_string(fd) + ": " + msg);
    std::lock_guard<std::mutex> lock(_playersMutex);
    if (!_gameStarted)
        return;
    if (_players.count(fd)) {
        if (msg.find("_ON") != std::string::npos)
            _players[fd].jetpack = true;
        else if (msg.find("_OFF") != std::string::npos)
            _players[fd].jetpack = false;
    }
}

void Server::logDebugMessage(const std::string& message) {
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);
    std::cout << "[DEBUG] " << std::put_time(std::localtime(&now_c), "%Y-%m-%d %H:%M:%S") << " - " << message << std::endl;
}

void Server::removeClient(size_t index) {
    int fd = _pollFds[index].fd;
    close(fd);
    {
        std::lock_guard<std::mutex> lock(_playersMutex);
        _players.erase(fd);
        if (_gameStarted && _players.size() < 2) {
            _gameStarted = false;
            std::string stopMsg = "STOP\n";
            for (const auto& [clientFd, player] : _players)
                send(clientFd, stopMsg.c_str(), stopMsg.length(), 0);
        }
    }
    _pollFds.erase(_pollFds.begin() + index);
}

void Server::updateLoop()
{
    using clock = std::chrono::high_resolution_clock;
    auto lastUpdate = clock::now();

    const float gravity = 880.0f;
    const float jetpackForce = 1100.0f;
    const float maxVelocity = 900.0f;
    const float minY = 0.0f;
    const float maxY = 780.0f;
    const float targetFrameTime = 1.0f / 60.0f;
    const float speed = 400.0f;

    while (true) {
        if (_countdownStarted) {
            auto now = clock::now();
            std::chrono::duration<float> elapsed = now - _countdownStart;

            if (elapsed.count() >= 10.0f) {
                _countdownStarted = false;
                _gameStarted = true;
                std::string startMsg = "START\n";
                std::lock_guard<std::mutex> lock(_playersMutex);
                for (const auto& [fd, player] : _players) {
                    send(fd, startMsg.c_str(), startMsg.length(), 0);
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            lastUpdate = clock::now();
            continue;
        }
        if (!_gameStarted) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }
        auto now = clock::now();
        std::chrono::duration<float> deltaTime = now - lastUpdate;
        if (deltaTime.count() < targetFrameTime) {
            std::this_thread::sleep_for(std::chrono::milliseconds(
                static_cast<int>((targetFrameTime - deltaTime.count()) * 1000)
            ));
            continue;
        }
        lastUpdate = now;
        float dt = deltaTime.count();
        {
            std::lock_guard<std::mutex> lock(_playersMutex);
            for (auto& [fd, player] : _players) {
                if (player.dead) {
                    player.velocity += gravity * dt;
                    if (player.velocity > maxVelocity)
                        player.velocity = maxVelocity;
                    player.y += player.velocity * dt;
                    if (player.y >= maxY) {
                        player.y = maxY;
                        player.velocity = 0;
                    continue;
                }
                }
                player.x += speed * dt;
                if (player.x > _maxX)
                    checkEndMap(player);
                if (player.jetpack && player.y > 100.0f)
                    player.velocity -= jetpackForce * dt;
                else
                    player.velocity += gravity * dt;
                if (player.velocity > maxVelocity)
                    player.velocity = maxVelocity;
                if (player.velocity < -maxVelocity)
                    player.velocity = -maxVelocity;

                player.y += player.velocity * dt;

                if (player.y < minY) {
                    player.y = minY;
                    player.velocity = 0;
                }
                if (player.y > maxY) {
                    player.y = maxY;
                    player.velocity = 0;
                }
                handleCollisions(player);
            }
            broadcastPositions();
            checkOnePlayer();
        }
    }
}

void Server::broadcastPositions()
{
    std::stringstream ss;
    for (const auto& [fd, player] : _players)
        ss << "POS " << player.id << " " << player.x << " " << player.y << "\n";
    std::string msg = ss.str();
    for (const auto& [fd, player] : _players)
        send(fd, msg.c_str(), msg.size(), 0);
}

void Server::loadMap()
{
    std::ifstream mapFile(_mapPath);
    if (!mapFile.is_open())
        throw Error("Failed to open map file");

    std::string line;
    int y = 0;
    int maxX = 0;
    while (std::getline(mapFile, line)) {
        if (line.length() > maxX)
            maxX = line.length();
        for (int x = 0; x < line.length(); ++x) {
            if (line[x] == 'e')
                _mapFireBall.push_back({x * 100, y * 97, 'e'});
            else if (line[x] == 'c')
                _mapCoin.push_back({x * 100, y * 97, 'c'});
        }
        y++;
    }
    mapFile.close();
    _maxX = maxX * 100;
    if (_debug)
        logDebugMessage("Map loaded with : " + std::to_string(_mapFireBall.size() + _mapCoin.size()) + " objects");
    if (_debug)
        logDebugMessage("Map size: " + std::to_string(_maxX) + "x" + std::to_string(y * 97));
}


void Server::sendMap(int fd)
{
    std::stringstream ss;
    for (const auto& obj : _mapCoin)
        ss << "OBJ " << obj.x << " " << obj.y << " " << obj.type << "\n";
    for (const auto& obj : _mapFireBall)
        ss << "OBJ " << obj.x << " " << obj.y << " " << obj.type << "\n";
    std::string msg = ss.str();
    send(fd, msg.c_str(), msg.size(), 0);
}

bool Server::checkCoin(const PlayerState& player, const Objects& obj)
{
    const float playerWidth = 170.0f;
    const float playerHeight = 160.0f;
    const float objWidth = 70.0f;
    const float objHeight = 60.0f;

    float playerCenterX = player.x + (playerWidth / 2.0f);
    float playerCenterY = player.y + (playerHeight / 2.0f);
    float objCenterX = obj.x + 50.0f + (objWidth / 2.0f);
    float objCenterY = obj.y + (objHeight / 2.0f);

    float dx = std::abs(playerCenterX - objCenterX);
    float dy = std::abs(playerCenterY - objCenterY);

    return dx < (playerWidth + objWidth) / 4.0f &&
           dy < (playerHeight + objHeight) / 1.2f;
}

bool Server::checkFireBall(const PlayerState& player, const Objects& obj)
{
    const float playerWidth = 170.0f;
    const float playerHeight = 160.0f;
    const float objWidth = 102.0f;
    const float objHeight = 138.0f;

    float playerCenterX = player.x + (playerWidth / 2.0f);
    float playerCenterY = player.y + (playerHeight / 2.0f);
    float objCenterX = obj.x + 50.0f + (objWidth / 2.0f);
    float objCenterY = obj.y + (objHeight / 2.0f);

    float dx = std::abs(playerCenterX - objCenterX);
    float dy = std::abs(playerCenterY - objCenterY);

    return dx < (playerWidth + objWidth) / 4.0f &&
           dy < (playerHeight + objHeight) / 3.0f;
}

void Server::handleCollisions(PlayerState& player)
{
    for (auto it = player.listeCoin.begin(); it != player.listeCoin.end();) {
        if (it->x > player.x + 1920 || it->x < player.x - 100) {
            ++it;
            continue;
        }
        if (checkCoin(player, *it)) {
            player.score++;
            if (_debug)
                logDebugMessage("Player " + std::to_string(player.id) + " collected a coin. Score: " + std::to_string(player.score));
            std::string msg = "COIN " + std::to_string(it->x) + " " + std::to_string(it->y) + "\n" +
                             "SCORE " + std::to_string(player.score) + "\n";
            send(player.fd, msg.c_str(), msg.length(), 0);
            it = player.listeCoin.erase(it);
        } else
            ++it;
    }
    for (const auto& fireball : _mapFireBall) {
        if (fireball.x > player.x + 1920 || fireball.x < player.x - 100)
            continue;
        if (checkFireBall(player, fireball)) {
            _allPlayers--;
            player.dead = true;
            if (_debug)
                logDebugMessage("Player " + std::to_string(player.id) + " hit by fireball. Score: " + std::to_string(player.score));
            std::string msg = "DEAD " + std::to_string(player.id) + "\n";
            for (const auto& [fd, p] : _players)
                send(fd, msg.c_str(), msg.length(), 0);
            checkOnePlayer();
            return;
        }
    }
}

void Server::checkOnePlayer()
{
    int aliveCount = 0;
    PlayerState* winner = nullptr;

    for (auto& [fd, p] : _players) {
        if (!p.dead) {
            aliveCount++;
            winner = &p;
        }
    }

    if (aliveCount == 1 && winner) {
        std::cout << "Player " << winner->id << " won with score: " << winner->score << std::endl;
        std::string msg = "WIN\n";
        send(winner->fd, msg.c_str(), msg.length(), 0);
        _gameStarted = false;
    }
}

void Server::checkEndMap(PlayerState& player)
{
    int bestScore = -1;
    std::vector<int> bestPlayers;

    for (const auto& [fd, p] : _players) {
        if (p.score > bestScore) {
            bestScore = p.score;
            bestPlayers.clear();
            bestPlayers.push_back(p.id);
        } else if (p.score == bestScore) {
            bestPlayers.push_back(p.id);
        }
    }
    bool tie = bestPlayers.size() > 1;
    for (const auto& [fd, p] : _players) {
        std::string msg;
        if (!tie && p.id == bestPlayers[0]) {
            msg = "WIN\n";
            if (_debug)
                logDebugMessage("Player " + std::to_string(p.id) + " won with score: " + std::to_string(p.score));
        } else
            msg = "LOOSE\n";
        send(fd, msg.c_str(), msg.length(), 0);
    }
    _gameStarted = false;
}
