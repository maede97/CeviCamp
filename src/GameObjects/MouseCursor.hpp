#ifndef MOUSECURSOR_HPP
#define MOUSECURSOR_HPP

#include "GameObject.hpp"
#include <SFML/Graphics.hpp>
#include <cmath>

class MouseCursor : public GameObject {
public:
    MouseCursor(Logger* logger)
        : GameObject(logger, GameObject::Type::Cursor)
    {
        if (!image_.loadFromFile("res/CampParts/MouseCursor.png")) {
            logger_->error("MouseCursor", "res/CampParts/MouseCursor.png not found");
            return;
        }
        animation_.setSpriteSheet(image_);
        animation_.addFrame(sf::IntRect(0, 0, 72, 72));
        sprite_ = AnimatedSprite(sf::seconds(1), true, false);
        sprite_.setAnimation(animation_);
    }

    bool validClick(int mouseX, int mouseY, int playerX, int playerY)
    {
        return (std::sqrt(std::pow(mouseX - playerX, 2) + std::pow(mouseY - playerY, 2)) < interactionRadius_);
    }

    void play() {}
    void handleClick() {}
    bool checkClick(float, float) { return false; }

    void updateMousePlayerPosition(int mouseX, int mouseY, int playerX, int playerY)
    {
        sf::Vector2f mousePos(mouseX, mouseY);
        sf::Vector2f playerPos(playerX, playerY);
        sf::Vector2f dist = (mousePos - playerPos);
        float len = std::sqrt(dist.x * dist.x + dist.y * dist.y);
        sf::Vector2f pos;
        if (len < interactionRadius_) {
            pos = sf::Vector2f(mouseX, mouseY);
        } else {
            dist = dist * interactionRadius_ / len;
            pos = playerPos + dist;
        }

        sprite_.setPosition(pos.x - image_.getSize().x / 2, pos.y - image_.getSize().y / 2);
    }

private:
    sf::Texture image_;
    Animation animation_;
    const float interactionRadius_ = 300.0f;
};

#endif