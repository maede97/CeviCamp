#ifndef GRASS_HPP
#define GRASS_HPP

#include "GameObject.hpp"
#include <SFML/Graphics.hpp>

class Grass : public GameObject {
public:
    Grass(Logger* logger, Settings* settings)
        : GameObject(logger, settings, GameObject::Type::Grass)
    {
        if (!image_.loadFromFile("res/CampParts/Grass.png")) {
            logger_->error("Grass", "res/CampParts/Grass.png not found");
            return;
        }

        image_.setRepeated(true);

        animation_.setSpriteSheet(image_);
        animation_.addFrame(sf::IntRect(0, 0, settings_->mapWidth, settings_->mapHeight));
        sprite_ = AnimatedSprite(sf::seconds(1), true, false);
        sprite_.setAnimation(animation_);

        sprite_.setPosition(0,0); // map starts top left
    }

    void play() {}
    bool checkClick(float, float) { return false; }
    void handleClick() {}

private:
    sf::Texture image_;
    Animation animation_;
};

#endif