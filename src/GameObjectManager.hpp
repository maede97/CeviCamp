#ifndef GAMEOBJECTMANAGER_HPP
#define GAMEOBJECTMANAGER_HPP

#include "GameObjects/Fire.hpp"
#include "GameObjects/Grass.hpp"
#include "Logger.hpp"
#include <SFML/Graphics.hpp>

class GameObjectManager {
public:
    GameObjectManager(Logger* logger, sf::Vector2u windowSize)
    {
        logger_ = logger;

        // To test, add objects here
        grass_ = new Grass(logger_, windowSize);
        fire_ = new Fire(logger_);
        fire_->play();
    }

    ~GameObjectManager()
    {
        delete fire_;
        delete grass_;
    }

    void drawAll(sf::RenderWindow& window, sf::Time dT)
    {
        // Update members
        fire_->update(dT);
        fire_->play();

        // Draw members, firstly background (grass)
        window.draw(grass_->getSprite());
        window.draw(fire_->getSprite());
    }

    void handleClick(int x, int y)
    {
        if (fire_->getSprite().getGlobalBounds().contains(x, y)) {
            fire_->upgrade();
        }
    }

private:
    Logger* logger_;
    Grass* grass_;
    Fire* fire_;
};

#endif