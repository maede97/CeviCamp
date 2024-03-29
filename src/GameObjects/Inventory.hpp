#ifndef INVENTORY_HPP
#define INVENTORY_HPP

#include "GameObject.hpp"

class Inventory : public GameObject {
public:
    Inventory(Logger* logger, Settings* settings, int slotSize)
        : GameObject(logger, settings, GameObject::Type::Inventory)
    {
        slotSize_ = slotSize;
        if (!image_.loadFromFile("res/CampParts/InventorySlots.png")) {
            logger_->error("PlayerInv", "res/CampParts/InventorySlots.png not found");
            return;
        }
        animation_.setSpriteSheet(image_);
        animation_.addFrame(sf::IntRect(0, 0, image_.getSize().x, image_.getSize().y));
        sprite_ = AnimatedSprite(sf::seconds(1), true, false);

        sprite_.setScale(settings_->getGUIFactor(), settings_->getGUIFactor());
        sprite_.setAnimation(animation_);
    }

    // will be used to change slot based on click
    bool checkClick(float x, float y) { return sprite_.getGlobalBounds().contains(x, y); }

private:
    sf::Texture image_;
    Animation animation_;

    int slotSize_;
};

#endif