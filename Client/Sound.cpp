/*
** EPITECH PROJECT, 2024
** B-NWP-400-NCY-4-1-jetpack-corentin.pierrat
** File description:
** Sound.cpp
*/

#include "Sound.hpp"


Sound::Sound()
{
    JetpackSound = sfmllib.createSound("Client/assets/jetpack.ogg", 100, true);
}

sf::Music* Sound::GetBackgroundSound()
{
    return BackgroundSound;
}

void Sound::SetBackgroundSound(sf::Music* music)
{
    BackgroundSound = music;
}

sf::Sound& Sound::GetjetpackSound()
{
    return JetpackSound;
}

Sound::~Sound()
{
}