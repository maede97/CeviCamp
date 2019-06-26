#ifndef INVENTORYSLOT_HPP
#define INVENTORYSLOT_HPP

#include "GameObject.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

class InventorySlot : public GameObject {
public:
    InventorySlot(Logger* logger, Settings* settings, int inventoryX, int inventoryY, int slotSize)
        : GameObject(logger, settings, GameObject::Type::InventorySlot)
    {
        if (!image_.loadFromFile("res/CampParts/InventorySlot.png")) {
            logger_->error("InventorySlot", "res/CampParts/InventorySlot.png not found");
            return;
        }
        animation_.setSpriteSheet(image_);
        animation_.addFrame(sf::IntRect(0, 0, image_.getSize().x, image_.getSize().y));
        sprite_ = AnimatedSprite(sf::seconds(1), true, false);

        sprite_.setPosition(inventoryX, inventoryY);

        sprite_.setAnimation(animation_);
    }

    bool checkClick(float x, float y) { return sprite_.getGlobalBounds().contains(x, y); }

private:
    sf::Texture image_;
    Animation animation_;

    int slotSize_ = 112;
};

#endif