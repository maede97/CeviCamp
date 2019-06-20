#ifndef GRASS_HPP
#define GRASS_HPP

#include "../Logger.hpp"
#include <SFML/Graphics.hpp>

class Grass {
public:
    Grass(Logger* logger, sf::Vector2i windowSize)
    {
        logger_ = logger;
        if (!image_.loadFromFile("res/CampParts/Grass.png")) {
            logger_->log("Grass", "res/CampParts/Grass.png not found");
            return;
        }

        image_.setRepeated(true);
        sprite_ = sf::Sprite(image_, sf::IntRect(0, 0, windowSize.x, windowSize.y));
        sprite_.setPosition(0, 0);
    }

    sf::Sprite getSprite() const { return sprite_; }

private:
    sf::Texture image_;
    sf::Sprite sprite_;

    Logger* logger_;
};

#endif