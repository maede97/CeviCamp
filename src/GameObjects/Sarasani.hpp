#ifndef SARASANI_HPP
#define SARASANI_HPP

#include "GameObject.hpp"

#include <SFML/Graphics.hpp>

class Sarasani : public GameObject {
public:
    Sarasani(Logger* logger, Settings* settings, int x, int y)
        : GameObject(logger, settings, GameObject::Type::Sarasani)
    {
        if (!image_.loadFromFile("res/CampParts/Sarasani.png")) {
            logger_->error("Sarasani", "res/CampParts/Sarasani.png not found");
            return;
        }
        animation_.setSpriteSheet(image_);
        animation_.addFrame(sf::IntRect(0, 0, image_.getSize().x, image_.getSize().y));
        sprite_ = AnimatedSprite(sf::seconds(1), true, false);

        sprite_.setAnimation(animation_);
        sprite_.setPosition(x, y);
    }

    void setAnimation()
    {
        sprite_.setAnimation(animation_);
    }

private:
    sf::Texture image_;
    Animation animation_;
};

#endif