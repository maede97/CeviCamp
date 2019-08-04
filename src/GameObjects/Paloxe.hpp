#ifndef PALOXE_HPP
#define PALOXE_HPP

#include "GameObject.hpp"

class Paloxe : public GameObject {
public:
    Paloxe(Logger* logger, Settings* settings, int x, int y)
        : GameObject(logger, settings, GameObject::Type::Paloxe)
    {
        if (!image_.loadFromFile("res/CampParts/Paloxe.png")) {
            logger_->error("Paloxe", "res/CampParts/Paloxe.png not found");
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
    
    bool checkClick(float x, float y)
    {
        return sprite_.getGlobalBounds().contains(x, y);
    }

private:
    sf::Texture image_;
    Animation animation_;
};

#endif