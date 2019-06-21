#ifndef INVENTORYITEM_HPP
#define INVENTORYITEM_HPP

#include "../Logger.hpp"
#include <SFML/Graphics.hpp>

class InventoryItem {
public:
    InventoryItem(Logger* logger, std::string item, int x, int y, unsigned int slot)
    {
        logger_ = logger;
        name_ = item;
        slot_ = slot;
        if (!image_.loadFromFile("res/CampParts/" + item + ".png")) {
            logger_->error("Tree", "res/CampParts/" + item + ".png");
            return;
        }

        sprite_ = sf::Sprite(image_);
        sprite_.setPosition(x, y);
    }

    std::string getName() const { return name_; }
    unsigned int getSlot() const {return slot_;}

    sf::Sprite getSprite() const { return sprite_; }


private:
    sf::Texture image_;
    sf::Sprite sprite_;
    std::string name_;
    unsigned int slot_;

    Logger* logger_;
};

#endif