#ifndef PLAYERINVENTORY_HPP
#define PLAYERINVENTORY_HPP

#include "../Settings.hpp"
#include "GameObject.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

class PlayerInventory : public GameObject {
public:
    PlayerInventory(Logger* logger, Settings* settings)
        : GameObject(logger, GameObject::Type::Inventory)
    {
        settings_ = settings;
        if (!image_.loadFromFile("res/CampParts/InventorySlots.png")) {
            logger_->error("PlayerInv", "res/CampParts/InventorySlots.png not found");
            return;
        }
        animation_.setSpriteSheet(image_);
        animation_.addFrame(sf::IntRect(0,0,image_.getSize().x, image_.getSize().y));
        sprite_ = AnimatedSprite(sf::seconds(1), true, false);

        int x_off = settings_->screenWidth - image_.getSize().x;
        sprite_.setPosition(x_off / 2, settings_->screenHeight - image_.getSize().y);

        sprite_.setAnimation(animation_);
    }

    void play() {}
    void handleClick() {}
    bool checkClick(float x, float y) { return false; } // maybe change slot later here

private:
    sf::Texture image_;
    Animation animation_;

    Settings* settings_;
    int slotSize_ = 112;
};

#endif