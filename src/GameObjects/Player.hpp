#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "AnimatedSprite.hpp"
#include "GameObject.hpp"

class Player : public GameObject
{
public:
    Player(Logger *logger, Settings *settings)
        : GameObject(logger, settings, GameObject::Type::Player)
    {
        if (!image_.loadFromFile("res/CampParts/Player.png"))
        {
            logger_->error("Player", "res/CampParts/Player.png not found");
            return;
        }

        // setup animation frames
        walkingDown_.setSpriteSheet(image_);
        walkingLeft_.setSpriteSheet(image_);
        walkingRight_.setSpriteSheet(image_);
        walkingUp_.setSpriteSheet(image_);

        // addFrame(Rect(x, y, size, size))
        walkingDown_.addFrame(sf::IntRect(playerSizeX_, 0, playerSizeX_, playerSizeY_));
        walkingDown_.addFrame(sf::IntRect(playerSizeX_ * 2, 0, playerSizeX_, playerSizeY_));
        walkingDown_.addFrame(sf::IntRect(playerSizeX_, 0, playerSizeX_, playerSizeY_));
        walkingDown_.addFrame(sf::IntRect(0, 0, playerSizeX_, playerSizeY_));

        walkingLeft_.addFrame(sf::IntRect(playerSizeX_, playerSizeY_, playerSizeX_, playerSizeY_));
        walkingLeft_.addFrame(sf::IntRect(playerSizeX_ * 2, playerSizeY_, playerSizeX_, playerSizeY_));
        walkingLeft_.addFrame(sf::IntRect(playerSizeX_, playerSizeY_, playerSizeX_, playerSizeY_));
        walkingLeft_.addFrame(sf::IntRect(0, playerSizeY_, playerSizeX_, playerSizeY_));

        walkingRight_.addFrame(sf::IntRect(playerSizeX_, playerSizeY_ * 2, playerSizeX_, playerSizeY_));
        walkingRight_.addFrame(sf::IntRect(playerSizeX_ * 2, playerSizeY_ * 2, playerSizeX_, playerSizeY_));
        walkingRight_.addFrame(sf::IntRect(playerSizeX_, playerSizeY_ * 2, playerSizeX_, playerSizeY_));
        walkingRight_.addFrame(sf::IntRect(0, playerSizeY_ * 2, playerSizeX_, playerSizeY_));

        walkingUp_.addFrame(sf::IntRect(playerSizeX_, playerSizeY_ * 3, playerSizeX_, playerSizeY_));
        walkingUp_.addFrame(sf::IntRect(playerSizeX_ * 2, playerSizeY_ * 3, playerSizeX_, playerSizeY_));
        walkingUp_.addFrame(sf::IntRect(playerSizeX_, playerSizeY_ * 3, playerSizeX_, playerSizeY_));
        walkingUp_.addFrame(sf::IntRect(0, playerSizeY_ * 3, playerSizeX_, playerSizeY_));

        // paused = false, repeat = true
        sprite_ = AnimatedSprite(sf::seconds(0.2), true, false);
        //sprite_.setPosition(sf::Vector2f(settings_->screenWidth / 2 - playerSize_ / 2, settings_->screenHeight / 2 - playerSize_ / 2));

        movement_ = sf::Vector2i(0, 0);

        setAnimation();
    }

    void setAnimation()
    {
        current_ = &walkingDown_;
        sprite_.setAnimation(*current_);
    }

    // move methods
    void down()
    {
        current_ = &walkingDown_;
        movement_.y += speed_;
        if (sprite_.getPosition().y > settings_->mapHeight - playerSizeY_)
        {
            movement_.y = 0;
        }
        noKeyWasPressed_ = false;
    }
    void up()
    {
        current_ = &walkingUp_;
        movement_.y += -speed_;
        if (sprite_.getPosition().y < 0)
        {
            movement_.y = 0;
        }
        noKeyWasPressed_ = false;
    }
    void left()
    {
        current_ = &walkingLeft_;
        movement_.x += -speed_;
        if (sprite_.getPosition().x < 0)
        {
            movement_.x = 0;
        }
        noKeyWasPressed_ = false;
    }
    void right()
    {
        current_ = &walkingRight_;
        movement_.x += speed_;
        if (sprite_.getPosition().x > settings_->mapWidth - playerSizeX_)
        {
            movement_.x = 0;
        }
        noKeyWasPressed_ = false;
    }

    // update player, move in set direction
    void update()
    {
        if (noKeyWasPressed_)
        {
            sprite_.stop();
        }
        sprite_.update(internalTime_.restart());
        
        sprite_.move(sf::Vector2f(movement_));//*frametime.asSeconds());
        noKeyWasPressed_ = true;
        movement_ = sf::Vector2i(0, 0);
    }

    void play() { sprite_.play(*current_); }

private:
    sf::Texture image_;

    Animation walkingDown_;
    Animation walkingLeft_;
    Animation walkingRight_;
    Animation walkingUp_;

    Animation *current_ = &walkingDown_;

    int speed_ = settings_->playerSpeed;
    sf::Vector2i movement_;
    sf::Vector2i windowSize_;
    bool noKeyWasPressed_ = true;

    int playerSizeX_ = 160;
    int playerSizeY_ = 240;
};

#endif