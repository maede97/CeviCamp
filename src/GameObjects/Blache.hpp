#ifndef BLACHE_HPP
#define BLACHE_HPP

#include "GameObject.hpp"

#include <SFML/Graphics.hpp>

class Blache : public GameObject {
public:
    Blache(Logger* logger, Settings* settings, int x, int y)
        : GameObject(logger, settings, GameObject::Type::Blache)
    {
        if (!image_.loadFromFile("res/CampParts/Blache.png")) {
            logger_->error("Stone", "res/CampParts/Blache.png not found");
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