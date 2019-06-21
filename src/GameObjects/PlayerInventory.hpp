#ifndef PLAYERINVENTORY_HPP
#define PLAYERINVENTORY_HPP

#include "../Logger.hpp"
#include "../Settings.hpp"
#include "InventoryItem.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

class PlayerInventory {
public:
    ~PlayerInventory()
    {
        for (auto item : items_) {
            delete item;
        }
    }
    PlayerInventory(Logger* logger, Settings* settings)
    {
        logger_ = logger;
        settings_ = settings;
        if (!image_.loadFromFile("res/CampParts/InventorySlots.png")) {
            logger_->error("Tree", "res/CampParts/InventorySlots.png not found");
            return;
        }

        sprite_ = sf::Sprite(image_);
        int x_off = settings_->screenWidth - image_.getSize().x;
        sprite_.setPosition(x_off / 2, settings_->screenHeight - image_.getSize().y);
    }

    bool addItem(std::string item)
    {
        int slot = 0;
        while (slot < 8) {
            bool empty = true;
            for (auto item : items_) {
                if (item->getSlot() == slot) {
                    empty = false;
                    break; // slot already full
                }
            }
            if (empty) {
                items_.push_back(new InventoryItem(logger_, item, sprite_.getPosition().x + slot * slotSize_, sprite_.getPosition().y, slot));
                logger_->info("PlayerInventory", "Add Item " + item);
                logger_->info("PlayerInventory", "Elements: " + std::to_string(items_.size()));

                return true;
            } else {
                slot++;
            }
        }
        return false;
    }

    bool removeItem(std::string item)
    {
        // playSound for this Item
        std::vector<InventoryItem*>::iterator position = items_.end();
        for (auto it = items_.begin(); it != items_.end(); it++) {
            if ((*it)->getName() == item) {
                position = it;
            }
        }
        if (position == items_.end()) {
            return false;
        }
        delete *position;
        items_.erase(position);
        logger_->info("PlayerInventory", "Remove Item " + item);
        logger_->info("PlayerInventory", "Elements: " + std::to_string(items_.size()));
        return true;
    }

    sf::Sprite getSprite() const { return sprite_; }

    void drawItems(sf::RenderWindow& window)
    {
        for (auto item : items_) {
            window.draw(item->getSprite());
        }
    }

private:
    sf::Texture image_;
    sf::Sprite sprite_;

    Logger* logger_;
    Settings* settings_;
    std::vector<InventoryItem*> items_;
    int slotSize_ = 112;
};

#endif