/*
** EPITECH PROJECT, 2024
** Jetpack
** File description:
** Player header
*/

#ifndef PLAYER_HPP
    #define PLAYER_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include "SfmlLib.hpp"
#include "Animation.hpp"
#include "Sound.hpp"

class Player {
    public:
        enum class GameState
        {
            INGAME,
            WIN,
            LOSE,
            DEAD
        };
        Player(float x = 250.0f, float y = 780.0f);

        void update(float deltaTime);
        void render(sf::RenderWindow& window);

        void setPosition(float x, float y);

        bool getSpacePressed() const;
        sf::Vector2f getPosition() const;
        SfmlLib& getSfmlLib();
        Animation& getAnimationPlayer();
        sf::Sprite& getPlayerSprite();
        sf::Text& getScoreText();
        int getScore() const;
        void setscore(int value);
        int getId() const;
        void setId(int value);
        sf::Sprite& getscoreSPrite();
        sf::Sprite& setColor(sf::Sprite &sprite);
        void setDead (GameState value);
        GameState getDead() const;


    private:
        sf::Texture texture;
        sf::Sprite Player_sprite;

        int id;
        int score;
        sf::Text Text_score;
        sf::Sprite ScoreSprite;
        sf::IntRect RectScore;
        bool Space_pressed;
        bool Jetpack_sound_bool;
        GameState dead;
        SfmlLib sfmlLib;
        Animation AnimationPlayer;
        Sound sound;
        sf::Vector2f _targetPosition;
        sf::Vector2f _currentPosition;
};

#endif
