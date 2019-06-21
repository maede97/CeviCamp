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
        sprite_.setPosition(300, 300);
    }

    unsigned int getLevel() const
    {
        if (current_ == &stones_)
            return 0;
        if (current_ == &wood_)
            return 1;
        if (current_ == &burning_)
            return 2;
        return 100;
    }

    void upgrade()
    {
        logger_->info("Fire", "Upgraded");
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