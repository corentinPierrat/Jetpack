/*
** EPITECH PROJECT, 2024
** B-NWP-400-NCY-4-1-jetpack-corentin.pierrat
** File description:
** Animation.hpp
*/

#pragma once

#include <SFML/Graphics.hpp>

class Animation {
public:
    Animation(int frameWidth = 0, int frameHeight = 0, int frameCount = 1, float switchTime = 0.1f);

    void update();
    sf::IntRect getRect() const;
    void setFrameSize(int width, int height);
    void setFrameCount(int count);
    void setSwitchTime(float time);
    void setTopRect(int);

private:
    sf::IntRect rect;
    sf::Clock clock;
    int frameWidth;
    int frameHeight;
    int currentFrame;
    int frameCount;
    float switchTime;
};
