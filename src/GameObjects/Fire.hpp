#ifndef FIRE_HPP
#define FIRE_HPP

#include "AnimatedSprite.hpp"
#include "GameObject.hpp"

class Fire : public GameObject {
public:
    Fire(Logger* logger, Settings* settings)
        : GameObject(logger, settings, GameObject::Type::Fire)
    {
        if (!image_.loadFromFile("res/CampParts/Fire.png")) {
            logger_->error("Fire", "res/CampParts/Fire.png not found");
            return;
        }
        stones_.setSpriteSheet(image_);
        wood_.setSpriteSheet(image_);
        burning_.setSpriteSheet(image_);

        // addFrame(Rect(x, y, size, size))
        stones_.addFrame(sf::IntRect(0, 0, 128, 128));
        wood_.addFrame(sf::IntRect(128, 0, 128, 128));
        burning_.addFrame(sf::IntRect(0, 128, 128, 128));
        burning_.addFrame(sf::IntRect(128, 128, 128, 128));

        // paused = false, repeat = true
        sprite_ = AnimatedSprite(sf::seconds(0.2), false, true);
        setAnimation();
    }

    bool checkClick(float x, float y)
    {
        return sprite_.getGlobalBounds().contains(x, y);
    }

    void setPosition(int x, int y)
    {
        sprite_.setPosition(x, y);
    }

    void setAnimation()
    {
        if (level_ == 0)
            current_ = &stones_;
        else if (level_ == 1)
            current_ = &wood_;
        else
            current_ = &burning_;
        sprite_.setAnimation(*current_);
    }

    void handleClick()
    {
        logger_->info("Fire", "Upgraded");
        if (current_ == &stones_) {
            current_ = &wood_;
            level_ = 1;
        } else if (current_ = &wood_) {
            current_ = &burning_;
            level_ = 2;
        }
    }

    void play() { sprite_.play(*current_); }

private:
    sf::Texture image_;

    Animation stones_;
    Animation wood_;
    Animation burning_;

    Animation* current_ = &stones_;
};

#endif