#ifndef INVENTORYITEM_HPP
#define INVENTORYITEM_HPP

#include "../Logger.hpp"
#include <SFML/Graphics.hpp>

class InventoryItem {
public:
    InventoryItem(Logger* logger, Settings* settings, std::string item, int x, int y, unsigned int slot)
    {
        logger_ = logger;
        settings_ = settings;
        name_ = item;
        slot_ = slot;
        if (!image_.loadFromFile("res/CampParts/" + item + ".png")) {
            logger_->error("InventoryItem", "res/CampParts/" + item + ".png");
            return;
        }

        amount_ = 1;

        amountText_.setFont(settings_->font);
        amountText_.setCharacterSize(32 * settings_->getGUIFactor());
        amountText_.setString(std::to_string(amount_));

        sprite_ = sf::Sprite(image_);
        sprite_.setPosition(x, y);

        sprite_.setScale(settings_->getGUIFactor(), settings_->getGUIFactor());

        x_ = x;
        y_ = y;
    }

    void updateText()
    {
        amountText_.setPosition(x_ + sprite_.getGlobalBounds().width / 2 - amountText_.getGlobalBounds().width / 2, y_ + sprite_.getGlobalBounds().width / 2 - amountText_.getGlobalBounds().height / 2);
    }

    sf::Sprite getSprite() const { return sprite_; }

    void addItem()
    {
        amount_++;
        amountText_.setString(std::to_string(amount_));
    }
    void removeItem()
    {
        amount_--;
        amountText_.setString(std::to_string(amount_));
    }
    unsigned int getAmount() const { return amount_; }

    std::string getName() const { return name_; }
    unsigned int getSlot() const { return slot_; }
    void draw(sf::RenderWindow& window)
    {
        updateText();
        window.draw(sprite_);
        window.draw(amountText_);
    }

private:
    sf::Texture image_;
    sf::Sprite sprite_;
    std::string name_;
    unsigned int slot_;

    unsigned int amount_;
    sf::Text amountText_;

    Logger* logger_;
    Settings* settings_;

    int x_, y_;
};

#endif