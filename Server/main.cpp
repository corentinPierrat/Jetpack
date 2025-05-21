/*
** EPITECH PROJECT, 2025
** Jetpack
** File description:
** main
*/

#include "Server.hpp"
#include <cstdlib>
#include <iostream>

int main(int argc, char* argv[])
{
    bool debug = false;
    int port = -1;
    std::string mapPath;
    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "-p" && i + 1 < argc)
            port = std::atoi(argv[++i]);
        else if (std::string(argv[i]) == "-m" && i + 1 < argc)
            mapPath = argv[++i];
        else if (std::string(argv[i]) == "-d")
            debug = true;
    }
    if (port == -1 || mapPath.empty()) {
        write(2, "Usage: ./server -p <port> -m <map> [-d]\n", 40);
        return 84;
    }
    try {
        Server server(port, mapPath, debug);
        server.run();
    } catch(const Error& e) {
        std::cerr << e.what() << '\n';
    }
    return 0;
}
