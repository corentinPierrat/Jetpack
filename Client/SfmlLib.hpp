/*
** EPITECH PROJECT, 2024
** B-NWP-400-NCY-4-1-jetpack-corentin.pierrat
** File description:
** SfmlLib.hpp
*/

#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include "Error.hpp"
#include <SFML/Audio.hpp>

class SfmlLib {
    public:
        SfmlLib();
        ~SfmlLib();
        static sf::Sprite createSprite(const std::string& path, sf::Vector2f position, sf::Vector2f Size, const sf::IntRect* rect = nullptr);
        sf::Sound createSound(const std::string& path, float volume = 100.f, bool loop = false);
        sf::Music* createMusic(const std::string& path, float volume = 100.f, bool loop = false);
        sf::Text createText(const std::string& fontPath, const std::string& content, sf::Vector2f position, unsigned int charSize, const sf::Color* color);
};
