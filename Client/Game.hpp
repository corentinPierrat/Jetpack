/*
** EPITECH PROJECT, 2024
** B-NWP-400-NCY-4-1-jetpack-corentin.pierrat
** File description:
** Game.hpp
*/

#ifndef GAME_HPP
    #define GAME_HPP

#include <SFML/Graphics.hpp>
#include "Player.hpp"
#include "SfmlLib.hpp"
#include "Animation.hpp"
#include <SFML/Audio.hpp>
#include "Sound.hpp"

class Game
{
    public:
        struct Objects {
        int x;
        int y;
        char type;
    };

        Game();
        ~Game();

        void update(float deltaTime);
        void render(sf::RenderWindow& window);

        bool isGameStarted() const { return _gameStarted; }
        void setGameStarted(bool started) { _gameStarted = started; }

        sf::Sprite& getFireBall();
        sf::Sprite& getCoin();
        sf::Text& getYouwon();
        sf::Text& getYouloose();
        sf::View& getCameraView();
        std::vector<Objects>& getmapObject();
        Animation& getAnimationCoins();
        Animation& getAnimationFireBalls();
        Player& getPlayer();
        SfmlLib& getSfmlLib();
        Sound& getSound();
        std::vector<Objects>& removeCoin(int x, int y);
        void setMapObjects(const std::vector<Objects>& mapObjects);
        std::map<int, Player>& getOtherPlayer();
        void setOtherPlayer(int id, const Player &player);
        bool removeOtherPlayer(int id);
        void run(sf::RenderWindow &window);


    private:
        sf::Sprite Background;
        sf::Sprite Background2;
        sf::Vector2f Pos_Backgound1;
        sf::Vector2f Pos_Backgound2;
        sf::Clock clock_back;

        sf::Sprite FireBalls;

        sf::Sprite Coins;

        sf::Text YouWon;
        sf::Text Youloose;
        bool _gameStarted = false;
        sf::View cameraView;
        std::vector<Objects> _mapObjects;
        std::map<int, Player> OtherPlayer;
        Animation AnimationCoins;
        Animation AnimationFireBalls;
        Player player;
        SfmlLib sfmlLib;
        Sound sound;
};

#endif
