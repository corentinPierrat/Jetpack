/*
** EPITECH PROJECT, 2025
** test01
** File description:
** Player
*/

#include "Player.hpp"
#include "Error.hpp"
#include <cmath>

Player::Player(float x, float y) : Jetpack_sound_bool(false)
{
    Player_sprite = sfmlLib.createSprite("Client/assets/player_sprite_sheet.png", {x, y}, {1, 1});
    Space_pressed = false;
    dead = GameState::INGAME;
    AnimationPlayer.setFrameSize(133, 134);
    AnimationPlayer.setFrameCount(4);
    AnimationPlayer.setSwitchTime(0.2f);
    score = 0;
    Text_score = sfmlLib.createText("Client/assets/jetpack_font.ttf", std::to_string(score), {500, 500}, 40, &sf::Color::White);
    _currentPosition = sf::Vector2f(x, y);
    _targetPosition = _currentPosition;
    RectScore = sf::IntRect(0, 0, 191, 171);
    ScoreSprite = sfmlLib.createSprite("Client/assets/coins_sprite_sheet.png", {500, 500}, {0.24, 0.24}, &RectScore);
}

sf::Vector2f Player::getPosition() const
{
    return Player_sprite.getPosition();
}

void Player::setPosition(float x, float y)
{
    _targetPosition = sf::Vector2f(x, y);
}

void Player::update(float deltaTime)
{
    float lerpFactor = 1.0f - std::pow(2.0f, -20.0f * deltaTime);
    _currentPosition.x += (_targetPosition.x - _currentPosition.x) * lerpFactor;
    _currentPosition.y += (_targetPosition.y - _currentPosition.y) * lerpFactor;
    Player_sprite.setTextureRect(AnimationPlayer.getRect());

    if (dead == GameState::DEAD) {
        AnimationPlayer.setTopRect(536);
        dead = GameState::LOSE;
    } else if (dead == GameState::INGAME) {
        if (_currentPosition.y >= 759.9f)
            AnimationPlayer.setTopRect(0);
        else
            AnimationPlayer.setTopRect(133);
    }

    Player_sprite.setPosition(_currentPosition);

    bool spaceIsPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && _currentPosition.y >= 100;

    if (spaceIsPressed && !Space_pressed) {
        Space_pressed = true;
        sound.GetjetpackSound().play();
        sound.GetjetpackSound().setLoop(true);
        Jetpack_sound_bool = true;
    }
    else if (!spaceIsPressed && Space_pressed) {
        Space_pressed = false;
        sound.GetjetpackSound().stop();
        Jetpack_sound_bool = false;
    }

    Text_score.setString(std::to_string(score));
    Text_score.setPosition(_currentPosition.x + -135, 108);
    ScoreSprite.setPosition(_currentPosition.x + -190, 110);
}

bool Player::getSpacePressed() const
{
    return Space_pressed;
}

void Player::render(sf::RenderWindow& window)
{
    window.draw(Player_sprite);
}

Player::GameState Player::getDead() const {
    return dead;
}


SfmlLib& Player::getSfmlLib()
{
    return sfmlLib;
}

Animation& Player::getAnimationPlayer()
{
    return AnimationPlayer;
}

sf::Sprite& Player::getPlayerSprite()
{
    return Player_sprite;
}

void Player::setId(int value)
{
    id = value;
}

int Player::getId() const
{
   return id;
}

sf::Text& Player::getScoreText()
{
   return Text_score;
}

int Player::getScore() const
{
    return score;
}

void Player::setscore(int value)
{
    score = value;
}

sf::Sprite& Player::setColor(sf::Sprite &sprite)
{
    sf::Color ghostColor(150, 150, 255, 128);
    sprite.setColor(ghostColor);
    return sprite;
}

sf::Sprite& Player::getscoreSPrite()
{
    return ScoreSprite;
}

void Player::setDead (GameState value)
{
    dead = value;
}

