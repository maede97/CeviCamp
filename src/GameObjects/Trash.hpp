#ifndef TRASH_HPP
#define TRASH_HPP

#include "GameObject.hpp"
#include <SFML/Graphics.hpp>

class Trash : public GameObject {
public:
    Trash(Logger* logger, Settings* settings, int x, int y)
        : GameObject(logger, settings, GameObject::Type::Trash)
    {
        if (!image_.loadFromFile("res/CampParts/Trash.png")) {
            logger_->error("Trash", "res/CampParts/Trash.png not found");
            return;
        }
        animation_.setSpriteSheet(image_);
        animation_.addFrame(sf::IntRect(0, 0, image_.getSize().x, image_.getSize().y));
        sprite_ = AnimatedSprite(sf::seconds(1), true, false);

        sprite_.setAnimation(animation_);
        sprite_.setPosition(x, y);
    }

    bool checkClick(float x, float y)
    {
        return sprite_.getGlobalBounds().contains(x, y);
    }

private:
    sf::Texture image_;
    Animation animation_;
};

#endif