#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "../Logger.hpp"
#include "AnimatedSprite.hpp"

class Player {
public:
    Player(Logger* logger, sf::Vector2i windowSize)
    {
        windowSize_ = windowSize;
        logger_ = logger;
        if (!image_.loadFromFile("res/CampParts/Player.png")) {
            logger_->error("Player", "res/CampParts/Player.png not found");
            return;
        }
        walkingDown_.setSpriteSheet(image_);
        walkingLeft_.setSpriteSheet(image_);
        walkingRight_.setSpriteSheet(image_);
        walkingUp_.setSpriteSheet(image_);

        // addFrame(Rect(x, y, size, size))
        walkingDown_.addFrame(sf::IntRect(playerSize_, 0, playerSize_, playerSize_));
        walkingDown_.addFrame(sf::IntRect(playerSize_ * 2, 0, playerSize_, playerSize_));
        walkingDown_.addFrame(sf::IntRect(playerSize_, 0, playerSize_, playerSize_));
        walkingDown_.addFrame(sf::IntRect(0, 0, playerSize_, playerSize_));

        walkingLeft_.addFrame(sf::IntRect(playerSize_, playerSize_, playerSize_, playerSize_));
        walkingLeft_.addFrame(sf::IntRect(playerSize_ * 2, playerSize_, playerSize_, playerSize_));
        walkingLeft_.addFrame(sf::IntRect(playerSize_, playerSize_, playerSize_, playerSize_));
        walkingLeft_.addFrame(sf::IntRect(0, playerSize_, playerSize_, playerSize_));

        walkingRight_.addFrame(sf::IntRect(playerSize_, playerSize_ * 2, playerSize_, playerSize_));
        walkingRight_.addFrame(sf::IntRect(playerSize_ * 2, playerSize_ * 2, playerSize_, playerSize_));
        walkingRight_.addFrame(sf::IntRect(playerSize_, playerSize_ * 2, playerSize_, playerSize_));
        walkingRight_.addFrame(sf::IntRect(0, playerSize_ * 2, playerSize_, playerSize_));

        walkingUp_.addFrame(sf::IntRect(playerSize_, playerSize_ * 3, playerSize_, playerSize_));
        walkingUp_.addFrame(sf::IntRect(playerSize_ * 2, playerSize_ * 3, playerSize_, playerSize_));
        walkingUp_.addFrame(sf::IntRect(playerSize_, playerSize_ * 3, playerSize_, playerSize_));
        walkingUp_.addFrame(sf::IntRect(0, playerSize_ * 3, playerSize_, playerSize_));

        // paused = false, repeat = true
        sprite_ = AnimatedSprite(sf::seconds(0.2), true, false);
        sprite_.setPosition(sf::Vector2f(windowSize / 2));

        movement_ = sf::Vector2f(0.0f, 0.0f);
    }

    sf::Vector2i getPosition() const
    {
        return sf::Vector2i(sprite_.getPosition()) + sf::Vector2i(playerSize_, playerSize_) / 2;
    }

    void moveDown()
    {
        current_ = &walkingDown_;
        movement_.y += speed_;
        if (sprite_.getPosition().y > windowSize_.y - playerSize_) {
            movement_.y = 0;
        }
        noKeyWasPressed_ = false;
    }
    void moveUp()
    {
        current_ = &walkingUp_;
        movement_.y -= speed_;
        if (sprite_.getPosition().y < 0) {
            movement_.y = 0;
        }
        noKeyWasPressed_ = false;
    }
    void moveLeft()
    {
        current_ = &walkingLeft_;
        movement_.x -= speed_;
        if (sprite_.getPosition().x < 0) {
            movement_.x = 0;
        }
        noKeyWasPressed_ = false;
    }
    void moveRight()
    {
        current_ = &walkingRight_;
        movement_.x += speed_;
        if (sprite_.getPosition().x > windowSize_.x - playerSize_) {
            movement_.x = 0;
        }
        noKeyWasPressed_ = false;
    }

    void play() { sprite_.play(*current_); }

    void update(sf::Time frametime)
    {
        if (noKeyWasPressed_) {
            sprite_.stop();
        }
        sprite_.update(frametime);
        sprite_.move(movement_ * frametime.asSeconds());
        noKeyWasPressed_ = true;
        movement_ = sf::Vector2f(0.0f, 0.0f);
    }

    AnimatedSprite getSprite() const { return sprite_; }

private:
    sf::Texture image_;

    Animation walkingDown_;
    Animation walkingLeft_;
    Animation walkingRight_;
    Animation walkingUp_;

    AnimatedSprite sprite_;

    Animation* current_ = &walkingDown_;

    Logger* logger_;

    float speed_ = 300.0f;
    sf::Vector2f movement_;
    sf::Vector2i windowSize_;
    bool noKeyWasPressed_ = true;

    int playerSize_ = 256;
};

#endif