#ifndef GAMEOBJECTMANAGER_HPP
#define GAMEOBJECTMANAGER_HPP

#include "GameObjects/Fire.hpp"
#include "GameObjects/Grass.hpp"
#include "GameObjects/InventoryItem.hpp"
#include "GameObjects/MouseCursor.hpp"
#include "GameObjects/Player.hpp"
#include "GameObjects/PlayerInventory.hpp"
#include "GameObjects/Tree.hpp"
#include "Logger.hpp"
#include "Settings.hpp"
#include "SoundManager.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

class GameObjectManager {
public:
    GameObjectManager(Logger* logger, Settings* settings, SoundManager* soundManager)
    {
        logger_ = logger;
        settings_ = settings;
        soundManager_ = soundManager;

        Grass* grass = new Grass(logger_, sf::Vector2i(settings_->screenWidth, settings_->screenHeight));
        Player* player = new Player(logger_, sf::Vector2i(settings_->screenWidth, settings_->screenHeight));
        MouseCursor* cursor = new MouseCursor(logger_);
        PlayerInventory* playerInventory = new PlayerInventory(logger_, settings_);

        // Test objects
        Fire* fire = new Fire(logger_);
        fire->setPosition(300, 300);

        Fire* fire2 = new Fire(logger_);
        fire2->setPosition(600, 600);

        // push objects into vector
        gameObjects_.push_back(grass);

        // build some trees
        int y_offset = -15;
        for (int x = 0; x < settings_->screenWidth;) {
            Tree* t = new Tree(logger_, x, y_offset);
            x += t->getSprite().getAnimation()->getSpriteSheet()->getSize().x;
            gameObjects_.push_back(t);
        }

        gameObjects_.push_back(fire);
        gameObjects_.push_back(fire2);
        gameObjects_.push_back(player);
        gameObjects_.push_back(cursor);
        gameObjects_.push_back(playerInventory);

        // Give some test items
        addInventoryItem("ItemMatch");
        addInventoryItem("ItemMatch");
    }

    ~GameObjectManager()
    {
        for (auto gameObject : gameObjects_) {
            delete gameObject;
        }
    }

    bool addInventoryItem(std::string item)
    {
        sf::Vector2i invPos;
        for (auto gameObject : gameObjects_) {
            if (gameObject->type == GameObject::Type::Inventory) {
                invPos = sf::Vector2i(gameObject->getPosition());
            }
        }
        int slot = 0;
        while (slot < 8) {
            bool empty = true;
            for (auto item : inventoryItems_) {
                if (item->getSlot() == slot) {
                    empty = false;
                    break; // slot already full
                }
            }
            if (empty) {
                inventoryItems_.push_back(new InventoryItem(logger_, item, invPos.x + slot * 112, invPos.y, slot));
                logger_->info("Inventory", "Add Item " + item);
                return true;
            } else {
                slot++;
            }
        }
        return false;
    }
    bool removeInventoryItem(std::string item)
    {
        // playSound for this Item
        std::vector<InventoryItem*>::iterator position = inventoryItems_.end();
        for (auto it = inventoryItems_.begin(); it != inventoryItems_.end(); it++) {
            if ((*it)->getName() == item) {
                position = it;
            }
        }
        if (position == inventoryItems_.end()) {
            return false;
        }
        delete *position;
        inventoryItems_.erase(position);
        logger_->info("Inventory", "Remove Item " + item);
        return true;
    }

    void down()
    {
        for (auto gameObject : gameObjects_) {
            gameObject->down();
        }
    }
    void up()
    {
        for (auto gameObject : gameObjects_) {
            gameObject->up();
        }
    }
    void left()
    {
        for (auto gameObject : gameObjects_) {
            gameObject->left();
        }
    }
    void right()
    {
        for (auto gameObject : gameObjects_) {
            gameObject->right();
        }
    }

    void playAnimations()
    {
        for (auto gameObject : gameObjects_) {
            gameObject->play();
        }
    }

    void updateDT(sf::Time dt)
    {
        for (auto gameObject : gameObjects_) {
            gameObject->update(dt);
        }
    }

    void drawAll(sf::RenderWindow& window, sf::Time dT)
    {
        // Update members
        updateDT(dT);

        // Play Animations
        playAnimations();

        // Draw members
        for (auto gameObject : gameObjects_) {
            window.draw(gameObject->getSprite());
        }
        for (auto item : inventoryItems_) {
            window.draw(item->getSprite());
        }
    }

    void updateMousePosition(int mouseX, int mouseY)
    {
        sf::Vector2i playerPos;
        for (auto gameObject : gameObjects_) {
            if (gameObject->type == GameObject::Type::Player) {
                playerPos = sf::Vector2i(gameObject->getPosition());
            }
        }
        for (auto gameObject : gameObjects_) {
            gameObject->updateMousePlayerPosition(mouseX, mouseY, playerPos.x, playerPos.y);
        }
    }

    void handleClick(int x, int y)
    {
        bool valid = false;
        sf::Vector2i playerPos;
        for (auto gameObject : gameObjects_) {
            if (gameObject->type == GameObject::Type::Player) {
                playerPos = sf::Vector2i(gameObject->getPosition());
            }
        }
        for (auto gameObject : gameObjects_) {
            if (gameObject->type == GameObject::Type::Cursor && gameObject->validClick(x, y, playerPos.x, playerPos.y)) {
                valid = true;
            }
        }
        if (!valid) {
            return;
        }
        // we have a valid click
        bool removeItem = false;
        auto removeIt = gameObjects_.end();
        auto currentIterator = gameObjects_.begin();
        for (auto gameObject : gameObjects_) {
            if (gameObject->checkClick(x, y)) {
                switch (gameObject->type) {

                case GameObject::Type::Fire:
                    switch (gameObject->getLevel()) {
                    case 0:
                        if (removeInventoryItem("ItemWood")) {
                            soundManager_->playSound("ItemWood");
                            gameObject->handleClick();
                        }
                        break;
                    case 1:
                        if (removeInventoryItem("ItemMatch")) {
                            soundManager_->playSound("ItemMatch");
                            gameObject->handleClick();
                        }
                    default:
                        break;
                    }
                    break;
                case GameObject::Type::Tree:
                    if (addInventoryItem("ItemWood")) {
                        soundManager_->playSound("TreeBreak");
                        removeItem = true;
                        removeIt = currentIterator;
                    }
                    break;
                default:
                    break;
                }
            }
            currentIterator++;
        }
        if (removeItem) {
            gameObjects_.erase(removeIt);
        }
    }

private:
    Logger* logger_;
    Settings* settings_;

    SoundManager* soundManager_;

    std::vector<GameObject*> gameObjects_;
    std::vector<InventoryItem*> inventoryItems_;
};

#endif