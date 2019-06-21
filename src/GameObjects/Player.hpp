#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "AnimatedSprite.hpp"
#include "GameObject.hpp"

class Player : public GameObject {
public:
    Player(Logger* logger, sf::Vector2i windowSize)
        : GameObject(logger, GameObject::Type::Player)
    {
        windowSize_ = windowSize;
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

    sf::Vector2f getPosition() const
    {
        return sf::Vector2f(sprite_.getPosition()) + sf::Vector2f(playerSize_, playerSize_) / 2.0f;
    }

    void down()
    {
        current_ = &walkingDown_;
        movement_.y += speed_;
        if (sprite_.getPosition().y > windowSize_.y - playerSize_) {
            movement_.y = 0;
        }
        noKeyWasPressed_ = false;
    }
    void up()
    {
        current_ = &walkingUp_;
        movement_.y -= speed_;
        if (sprite_.getPosition().y < 0) {
            movement_.y = 0;
        }
        noKeyWasPressed_ = false;
    }
    void left()
    {
        current_ = &walkingLeft_;
        movement_.x -= speed_;
        if (sprite_.getPosition().x < 0) {
            movement_.x = 0;
        }
        noKeyWasPressed_ = false;
    }
    void right()
    {
        current_ = &walkingRight_;
        movement_.x += speed_;
        if (sprite_.getPosition().x > windowSize_.x - playerSize_) {
            movement_.x = 0;
        }
        noKeyWasPressed_ = false;
    }

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

    bool checkClick(float x, float y) { return false; } // can't click on player
    void play() { sprite_.play(*current_); }
    void handleClick() {}

private:
    sf::Texture image_;

    Animation walkingDown_;
    Animation walkingLeft_;
    Animation walkingRight_;
    Animation walkingUp_;

    Animation* current_ = &walkingDown_;

    float speed_ = 300.0f;
    sf::Vector2f movement_;
    sf::Vector2i windowSize_;
    bool noKeyWasPressed_ = true;

    int playerSize_ = 256;
};

#endif