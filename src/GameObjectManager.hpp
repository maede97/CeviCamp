#ifndef GAMEOBJECTMANAGER_HPP
#define GAMEOBJECTMANAGER_HPP

#include "GameObjects/Fire.hpp"
#include "GameObjects/Grass.hpp"
#include "GameObjects/Player.hpp"
#include "Logger.hpp"
#include <SFML/Graphics.hpp>

class GameObjectManager {
public:
    GameObjectManager(Logger* logger, sf::Vector2i windowSize)
    {
        logger_ = logger;

        // To test, add objects here
        grass_ = new Grass(logger_, windowSize);
        fire_ = new Fire(logger_);
        player_ = new Player(logger_, windowSize);
        fire_->play();
    }

    ~GameObjectManager()
    {
        delete fire_;
        delete grass_;
        delete player_;
    }

    void down()
    {
        player_->moveDown();
    }
    void up()
    {
        player_->moveUp();
    }
    void left()
    {
        player_->moveLeft();
    }
    void right()
    {
        player_->moveRight();
    }

    void drawAll(sf::RenderWindow& window, sf::Time dT)
    {
        // Update members
        fire_->update(dT);
        player_->update(dT);
        fire_->play();
        player_->play();

        // Draw members, firstly background (grass)
        window.draw(grass_->getSprite());
        window.draw(fire_->getSprite());
        window.draw(player_->getSprite());
    }

    void handleClick(int x, int y)
    {
        logger_->info("GameObjectManager","Handle Click");
        if (fire_->getSprite().getGlobalBounds().contains(x, y)) {
            fire_->upgrade();
        }
    }

private:
    Logger* logger_;
    Grass* grass_;
    Fire* fire_;
    Player* player_;
};

#endif