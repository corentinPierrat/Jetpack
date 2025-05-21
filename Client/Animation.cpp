/*
** EPITECH PROJECT, 2024
** B-NWP-400-NCY-4-1-jetpack-corentin.pierrat
** File description:
** Animation.cpp
*/

#include "Animation.hpp"

Animation::Animation(int frameWidth, int frameHeight, int frameCount, float switchTime)
    : frameWidth(frameWidth), frameHeight(frameHeight), currentFrame(0), frameCount(frameCount), switchTime(switchTime)
{
    rect = sf::IntRect(0, 0, frameWidth, frameHeight);
}

void Animation::update()
{
    if (clock.getElapsedTime().asSeconds() >= switchTime) {
        currentFrame = (currentFrame + 1) % frameCount;
        rect.left = currentFrame * frameWidth;
        clock.restart();
    }
}

sf::IntRect Animation::getRect() const
{
    return rect;
}

void Animation::setFrameSize(int width, int height)
{
    frameWidth = width;
    frameHeight = height;
    rect.width = width;
    rect.height = height;
}

void Animation::setFrameCount(int count)
{
    frameCount = count;
}

void Animation::setSwitchTime(float time)
{
    switchTime = time;
}

void Animation::setTopRect(int value)
{
    rect.top = value;
}
