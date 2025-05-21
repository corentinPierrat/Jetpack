/*
** EPITECH PROJECT, 2024
** B-NWP-400-NCY-4-1-jetpack-corentin.pierrat
** File description:
** Sound.hpp
*/

#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include "Error.hpp"
#include <SFML/Audio.hpp>
#include "SfmlLib.hpp"

class Sound {
    public:
        Sound();
        ~Sound();
        sf::Music* GetBackgroundSound();
        sf::Sound& GetjetpackSound();
        void SetBackgroundSound(sf::Music* music);

    private:
        sf::Music* BackgroundSound;
        sf::Sound JetpackSound;
        SfmlLib sfmllib;

};
