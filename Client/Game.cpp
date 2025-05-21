/*
** EPITECH PROJECT, 2024
** B-NWP-400-NCY-4-1-jetpack-corentin.pierrat
** File description:
** Game.cpp
*/

#include "Game.hpp"
#include "Error.hpp"
#include "Player.hpp"

Game::Game()
{
    Pos_Backgound1 = {0.f, -180.f};
    Pos_Backgound2 = {6904.f, -180.f};

    Background = sfmlLib.createSprite("Client/assets/background.png", {Pos_Backgound1.x, Pos_Backgound1.y}, {4, 4});
    Background2 = sfmlLib.createSprite("Client/assets/background.png", {Pos_Backgound2.x, Pos_Backgound2.y}, {4, 4});

    Coins = sfmlLib.createSprite("Client/assets/coins_sprite_sheet.png", {0, 0}, {0.4, 0.4});

    AnimationCoins.setFrameSize(191, 171);
    AnimationCoins.setFrameCount(6);
    AnimationCoins.setSwitchTime(0.2f);

    FireBalls = sfmlLib.createSprite("Client/assets/FireBall.png", {0, 0}, {0.5, 0.5});

    AnimationFireBalls.setFrameSize(204, 256);
    AnimationFireBalls.setFrameCount(5);
    AnimationFireBalls.setSwitchTime(0.2f);

    cameraView.setSize(1920, 1080);
    cameraView.setCenter(player.getPosition());

    getSound().SetBackgroundSound(sfmlLib.createMusic("Client/assets/theme.ogg", 40, true));
    Youloose = sfmlLib.createText("Client/assets/jetpack_font.ttf", "You loose !", {500, 500}, 40, &sf::Color::White);
    YouWon = sfmlLib.createText("Client/assets/jetpack_font.ttf", "You won !", {500, 500}, 40, &sf::Color::White);
}

void Game::update(float deltaTime)
{
    sf::Time elapsed = clock_back.getElapsedTime();
    sf::Vector2f playerPos = player.getPosition();

    if (elapsed.asSeconds() > 0.004f) {
        const float backgroundWidth = 6904.f;

        if (playerPos.x - cameraView.getSize().x / 2 > Pos_Backgound1.x + backgroundWidth)
            Pos_Backgound1.x = Pos_Backgound2.x + backgroundWidth;
        if (playerPos.x - cameraView.getSize().x / 2 > Pos_Backgound2.x + backgroundWidth)
            Pos_Backgound2.x = Pos_Backgound1.x + backgroundWidth;
        Background.setPosition(Pos_Backgound1.x, Pos_Backgound1.y);
        Background2.setPosition(Pos_Backgound2.x, Pos_Backgound2.y);
        clock_back.restart();
    }
    cameraView.setCenter(playerPos.x + 750, 540);

    for (auto & [key, Player] : OtherPlayer)
        Player.update(deltaTime);
}

void Game::render(sf::RenderWindow& window)
{
    window.draw(Background);
    window.draw(Background2);

    for (Objects &OBJ : this->_mapObjects) {
        if (OBJ.type == 'c') {
            Coins.setPosition(OBJ.x, OBJ.y);
            window.draw(Coins);
        }
        if (OBJ.type == 'e') {
            FireBalls.setPosition(OBJ.x, OBJ.y);
            window.draw(FireBalls);
        }
    }

    for (auto & [key, Player] : OtherPlayer)
        window.draw(Player.getPlayerSprite());

    window.draw(player.getScoreText());
    window.draw(player.getscoreSPrite());

    if (player.getDead() == Player::GameState::WIN) {
        window.draw(YouWon);
    } else if (player.getDead() == Player::GameState::LOSE) {
        window.draw(Youloose);
    }
}

sf::Sprite& Game::getFireBall()
{
    return FireBalls;
}

sf::Sprite& Game::getCoin()
{
    return Coins;
}

sf::View& Game::getCameraView()
{
    return cameraView;
}

std::vector<Game::Objects>& Game::getmapObject()
{
    return _mapObjects;
}

Animation& Game::getAnimationCoins()
{
    return AnimationCoins;
}

Animation& Game::getAnimationFireBalls()
{
    return AnimationFireBalls;
}

Player& Game::getPlayer()
{
    return player;
}

SfmlLib& Game::getSfmlLib()
{
    return sfmlLib;
}

Sound& Game::getSound()
{
    return sound;
}

std::vector<Game::Objects>& Game::removeCoin(int x, int y)
{
    for (auto it = _mapObjects.begin(); it != _mapObjects.end(); ) {
        if (it->type == 'c' && it->x == x && it->y == y)
            it = _mapObjects.erase(it);
        else
            ++it;
    }
    return _mapObjects;
}

void Game::setMapObjects(const std::vector<Objects> &mapObjects)
{
    _mapObjects = mapObjects;
}

std::map<int, Player>& Game::getOtherPlayer()
{
    return OtherPlayer;
}

void Game::setOtherPlayer(int id, const Player &player)
{
    OtherPlayer[id] = player;
}

bool Game::removeOtherPlayer(int id)
{
    return OtherPlayer.erase(id) > 0;
}

void Game::run(sf::RenderWindow &window)
{
    sf::Clock clock;

    sf::Font font;
    if (!font.loadFromFile("Client/assets/jetpack_font.ttf"))
        throw std::runtime_error("Error loading font");

    sf::Text waitingText;
    waitingText.setFont(font);
    waitingText.setString("Waiting for another player...");
    waitingText.setCharacterSize(30);
    waitingText.setFillColor(sf::Color::White);
    waitingText.setPosition(
        window.getSize().x / 2.f - waitingText.getGlobalBounds().width / 2.f,
        window.getSize().y / 2.f - waitingText.getGlobalBounds().height / 2.f
    );

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (!isGameStarted()) {
            window.clear();
            window.draw(waitingText);
        } else {
            sf::Time deltaTime = clock.restart();
            player.update(deltaTime.asSeconds());
            player.getPlayerSprite().setTextureRect(player.getAnimationPlayer().getRect());
            update(deltaTime.asSeconds());
            window.setView(cameraView);
            player.getAnimationPlayer().update();
            player.getPlayerSprite().setTextureRect(player.getAnimationPlayer().getRect());
            AnimationCoins.update();
            Coins.setTextureRect(AnimationCoins.getRect());
            AnimationFireBalls.update();
            FireBalls.setTextureRect(AnimationFireBalls.getRect());
            Youloose.setPosition(player.getPosition().x + 650 , 540);
            YouWon.setPosition(player.getPosition().x + 650 , 540);
            window.clear();
            render(window);
            player.render(window);
        }
        window.display();
    }
}

sf::Text& Game::getYouloose()
{
    return Youloose;
}

sf::Text& Game::getYouwon()
{
    return YouWon;
}

Game::~Game()
{}
