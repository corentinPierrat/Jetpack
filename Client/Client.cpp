/*
** EPITECH PROJECT, 2024
** B-NWP-400-NCY-4-1-jetpack-corentin.pierrat
** File description:
** Client.cpp
*/


#include "Client.hpp"
#include "Error.hpp"
#include "Player.hpp"
#include "Game.hpp"
#include <iostream>
#include <thread>
#include <chrono>
#include <SFML/Graphics.hpp>
#include <sstream>
#include <cstdlib>
#include <iomanip>
#include <ctime>



Client::Client(const std::string& ip, int port, bool debug) : ip(ip), port(port), debug(debug) {
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
        throw Error("Error creating socket!");

    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    if (inet_pton(AF_INET, ip.c_str(), &server.sin_addr) <= 0)
        throw Error("Invalid IP address");
}

void Client::connectToServer() {
    if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0)
        throw Error("Connection failed");
    std::cout << "Connected to " << ip << " on port " << port << std::endl;
}

void Client::sendMessage(const std::string& message) {
    send(sock, message.c_str(), message.length(), 0);
    if (debug) logDebugMessage("Sent to server: " + message);
}

void Client::receiveUpdates(Game &game) {
    char buffer[1024];

    while (true) {
        int bytesReceived = recv(this->getSocket(), buffer, sizeof(buffer) - 1, 0);

        if (bytesReceived <= 0)
            continue;

        buffer[bytesReceived] = '\0';
        std::string message(buffer);
        if (debug) logDebugMessage("Received from server: " + message);
        std::stringstream ss(buffer);
        std::string command;

        while (ss >> command) {
            handleCommand(ss, command, game);
        }
    }
}

int Client::getSocket() const { return sock; }


void Client::logDebugMessage(const std::string& message)
{
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);
    std::cout << "[DEBUG] " << std::put_time(std::localtime(&now_c), "%Y-%m-%d %H:%M:%S") << " - " << message << std::endl;
}


void Client::handleCommand(std::stringstream &ss, const std::string &command, Game &game)
{
    if (command == "ID")
        handleId(ss, game);
    else if (command == "POS")
        handlePosition(ss, game);
    else if (command == "OBJ")
        handleObject(ss, game);
    else if (command == "START")
        handleStart(game);
    else if (command == "COIN")
        handleCoin(ss, game);
    else if (command == "SCORE")
        handleScore(ss, game);
    else if (command == "DEAD") {
    int id;
    ss >> id;
    if (id == game.getPlayer().getId()) {
        game.getPlayer().setDead(Player::GameState::DEAD);
    } else {
        auto &otherPlayers = game.getOtherPlayer();
        if (otherPlayers.find(id) != otherPlayers.end()) {
            otherPlayers[id].setDead(Player::GameState::DEAD);
        }
    }
    bool allOthersDead = true;
    for (const auto& [otherId, player] : game.getOtherPlayer()) {
        if (player.getDead() != Player::GameState::DEAD) {
            allOthersDead = false;
            break;
        }
    }
    if (allOthersDead && game.getPlayer().getDead() != Player::GameState::DEAD) {
        game.getPlayer().setDead(Player::GameState::WIN);
    }
}

    else if (command == "LOOSE")
        game.getPlayer().setDead(Player::GameState::LOSE);
    else if (command == "WIN")
        game.getPlayer().setDead(Player::GameState::WIN);
}

void Client::handleId(std::stringstream &ss, Game &game)
{
    int id;
    ss >> id;
    game.getPlayer().setId(id);
}

void Client::handlePosition(std::stringstream &ss, Game &game)
{
    int id;
    float x, y;
    ss >> id >> x >> y;

    if (game.getPlayer().getId() == id) {
        game.getPlayer().setPosition(x, y);
    } else {
        auto &others = game.getOtherPlayer();

        if (others.find(id) == others.end()) {
            others[id] = Player();
            game.getPlayer().setColor(others[id].getPlayerSprite());
        }

        others[id].setPosition(x, y);
    }
}

void Client::handleObject(std::stringstream &ss, Game &game)
{
    float x, y;
    std::string type;
    ss >> x >> y >> type;

    game.getmapObject().push_back({static_cast<int>(x),static_cast<int>(y),type.empty() ? '_' : type[0]});
}

void Client::handleStart(Game &game) {
    game.setGameStarted(true);
    game.getSound().GetBackgroundSound()->play();
    if (debug) logDebugMessage("Game started!");
}

void Client::handleCoin(std::stringstream &ss, Game &game)
{
    float x, y;
    ss >> x >> y;
    game.removeCoin(static_cast<int>(x), static_cast<int>(y));
}

void Client::handleScore(std::stringstream &ss, Game &game)
{
    int score;
    ss >> score;
    game.getPlayer().setscore(score);
}

void Client::run()
{
    Game game;

    std::thread gameThread([&game]() {
        sf::RenderWindow window(sf::VideoMode(1920, 1080), "Jetpack");
        game.run(window);
    });

    std::thread commThread([this, &game]() {
        while (true) {
            if (game.isGameStarted()) {
                std::string messageKey = game.getPlayer().getSpacePressed() ? "JETPACK_ON\n" : "JETPACK_OFF\n";
                this->sendMessage(messageKey);
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    });

    std::thread updateThread(&Client::receiveUpdates, this, std::ref(game));

    gameThread.join();
    commThread.detach();
    updateThread.detach();
}

Client::~Client()
{
    close(sock);
}

int main(int argc, char* argv[])
{
    std::string ip;
    int port = -1;
    bool debug = false;

    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "-p" && i + 1 < argc)
            port = std::atoi(argv[++i]);
        else if (std::string(argv[i]) == "-h" && i + 1 < argc)
            ip = argv[++i];
        else if (std::string(argv[i]) == "-d")
            debug = true;
    }

    if (port == -1 || ip.empty()) {
        write(2, "Usage: ./client -h <ip> -p <port> [-d]\n", 40);
        return 84;
    }

    try {
        Client client(ip, port, debug);
        client.connectToServer();
        client.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 84;
    }

    return 0;
}