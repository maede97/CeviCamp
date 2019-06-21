#ifndef MOUSECURSOR_HPP
#define MOUSECURSOR_HPP

#include "../Logger.hpp"
#include <SFML/Graphics.hpp>
#include <cmath>

class MouseCursor {
public:
    MouseCursor(Logger* logger)
    {
        logger_ = logger;
        if (!image_.loadFromFile("res/CampParts/MouseCursor.png")) {
            logger_->error("MouseCursor", "res/CampParts/MouseCursor.png not found");
            return;
        }

        sprite_ = sf::Sprite(image_);
    }

    bool validClick(int mouseX, int mouseY, int playerX, int playerY)
    {
        return (std::sqrt(std::pow(mouseX - playerX, 2) + std::pow(mouseY - playerY, 2)) < interactionRadius_);
    }

    void setPosition(int mouseX, int mouseY, int playerX, int playerY)
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
    sf::Sprite getSprite() const { return sprite_; }

private:
    sf::Texture image_;
    sf::Sprite sprite_;

    Logger* logger_;

    const float interactionRadius_ = 300.0f;
};

#endif