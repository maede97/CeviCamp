#ifndef FIRE_HPP
#define FIRE_HPP

#include "../Logger.hpp"
#include "AnimatedSprite.hpp"

class Fire {
public:
    Fire(Logger* logger)
    {
        logger_ = logger;
        if (!image_.loadFromFile("res/CampParts/Fire.png")) {
            logger_->log("Fire", "res/CampParts/Fire.png not found");
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
        sprite_.setPosition(300, 300);
    }

    void upgrade()
    {
        logger_->log("Fire", "Upgraded");
        if (current_ == &stones_) {
            current_ = &wood_;
        } else if (current_ = &wood_) {
            current_ = &burning_;
        }
    }

    void play() { sprite_.play(*current_); }

    void update(sf::Time frametime) { sprite_.update(frametime); }

    AnimatedSprite getSprite() const { return sprite_; }

private:
    sf::Texture image_;

    Animation stones_;
    Animation wood_;
    Animation burning_;

    AnimatedSprite sprite_;

    Animation* current_ = &stones_;

    Logger* logger_;
};

#endif