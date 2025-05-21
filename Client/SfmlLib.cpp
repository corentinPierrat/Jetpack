/*
** EPITECH PROJECT, 2024
** B-NWP-400-NCY-4-1-jetpack-corentin.pierrat
** File description:
** SfmlLib.cpp
*/

#include "SfmlLib.hpp"
#include <stdexcept>
#include <memory.h>

SfmlLib::SfmlLib()
{
}

sf::Sprite SfmlLib::createSprite(const std::string& path, sf::Vector2f position, sf::Vector2f Size, const sf::IntRect* rect)
{
    sf::Texture* texture = new sf::Texture();
    if (!texture->loadFromFile(path))
        throw Error("Failed to load texture: " + path);
    sf::Sprite sprite;
    sprite.setTexture(*texture);
    sprite.setPosition(position);
    sprite.setScale(Size);
    if (rect)
        sprite.setTextureRect(*rect);
    return sprite;
}

sf::Sound SfmlLib::createSound(const std::string& path, float volume, bool loop)
{
    sf::SoundBuffer* buffer = new sf::SoundBuffer();

    if (!buffer->loadFromFile(path)) {
        throw Error("Failed to load sound file: " + path);
    }

    sf::Sound sound;
    sound.setBuffer(*buffer);
    sound.setVolume(volume);
    sound.setLoop(loop);

    return sound;
}


sf::Music* SfmlLib::createMusic(const std::string& path, float volume, bool loop)
{
    sf::Music* music = new sf::Music();

    if (!music->openFromFile(path)) {
        delete music;
        throw std::runtime_error("Failed to load music from: " + path);
    }

    music->setVolume(volume);
    music->setLoop(loop);

    return music;
}

sf::Text SfmlLib::createText(const std::string& fontPath, const std::string& content, sf::Vector2f position, unsigned int charSize, const sf::Color* color)
{
    sf::Font* font = new sf::Font();
    if (!font->loadFromFile(fontPath))
        throw Error("Failed to load font: " + fontPath);

    sf::Text text;
    text.setFont(*font);
    text.setString(content);
    text.setPosition(position);
    text.setCharacterSize(charSize);

    if (color)
        text.setFillColor(*color);

    return text;
}

SfmlLib::~SfmlLib()
{
}
