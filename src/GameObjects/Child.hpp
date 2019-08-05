#ifndef CHILD_HPP
#define CHILD_HPP

#include "GameObject.hpp"

class Child : public GameObject {
public:
    enum Action {
        Idle,
        MovingRight,
        MovingLeft,
        MovingUp,
        MovingDown
    };
    Child(Logger* logger, Settings* settings)
        : GameObject(logger, settings, GameObject::Type::Child)
    {
        if (!image_.loadFromFile("res/CampParts/Player.png")) // TODO
        {
            logger_->error("Child", "res/CampParts/Player.png not found");
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

        movement_ = sf::Vector2f(0.f, 0.f);

        setAnimation();

        currentAction_ = getRandomAction();

        sprite_.setScale(0.75f, 0.75f);
    }

    Action getRandomAction()
    {
        int index = std::rand() % 5;
        return Action(index);
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
        if (sprite_.getPosition().y > settings_->mapHeight - playerSizeY_) {
            movement_.y = 0.f;
        }
        sprite_.move(movement_);
    }
    void up()
    {
        current_ = &walkingUp_;
        movement_.y += -speed_;
        if (sprite_.getPosition().y < 0) {
            movement_.y = 0.f;
        }
        sprite_.move(movement_);
    }
    void left()
    {
        current_ = &walkingLeft_;
        movement_.x += -speed_;
        if (sprite_.getPosition().x < 0) {
            movement_.x = 0.f;
        }
        sprite_.move(movement_);
    }
    void right()
    {
        current_ = &walkingRight_;
        movement_.x += speed_;
        if (sprite_.getPosition().x > settings_->mapWidth - playerSizeX_) {
            movement_.x = 0.f;
        }
        sprite_.move(movement_);
    }

    void update()
    {
        // use level of child as timer
        if (actionTimer_.getElapsedTime().asSeconds() > level_) {
            // choose random action
            currentAction_ = getRandomAction();
            sprite_.stop();
            actionTimer_.restart();
        }
        switch (currentAction_) {
        case Action::Idle:
            // do nothing here, just stand around
            sprite_.stop();
            break;
        case MovingDown:
            down();
            break;
        case MovingLeft:
            left();
            break;
        case MovingRight:
            right();
            break;
        case MovingUp:
            up();
            break;
        default:
            break;
        }
        movement_ = sf::Vector2f(0.f, 0.f);

        sprite_.update(internalTime_.restart());
    }

    void play() { sprite_.play(*current_); }

private:
    sf::Texture image_;

    Animation walkingDown_;
    Animation walkingLeft_;
    Animation walkingRight_;
    Animation walkingUp_;

    Animation* current_ = &walkingDown_;

    int speed_ = settings_->playerSpeed * 0.75f; // children are slower
    sf::Vector2f movement_;

    int playerSizeX_ = 160;
    int playerSizeY_ = 240;

    Action currentAction_;
    sf::Clock actionTimer_;
};

#endif