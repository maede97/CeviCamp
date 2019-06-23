#ifndef GAMEOBJECTMANAGER_HPP
#define GAMEOBJECTMANAGER_HPP

#include "GameObjects/Fire.hpp"
#include "GameObjects/Grass.hpp"
#include "GameObjects/InventoryItem.hpp"
#include "GameObjects/InventorySlot.hpp"
#include "GameObjects/MouseCursor.hpp"
#include "GameObjects/Player.hpp"
#include "GameObjects/PlayerInventory.hpp"
#include "GameObjects/Stone.hpp"
#include "GameObjects/Tree.hpp"
#include "Logger.hpp"
#include "Settings.hpp"
#include "SoundManager.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

/**
 * TODO
 * Cursor and player always in front, or sort vector by enum type?
 */

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

        PlayerInventory* playerInventory = new PlayerInventory(logger_, settings_, slotSize_);
        InventorySlot* inventorySlot = new InventorySlot(logger_, playerInventory->getPosition().x, playerInventory->getPosition().y, slotSize_);

        // push objects into vector
        gameObjects_.push_back(grass);

        // build some trees and stones
        int y_offset = -15;
        for (int x = 0; x < settings_->screenWidth;) {
            if (rand() % 2 == 0) {
                Tree* t = new Tree(logger_, x, y_offset);
                x += t->getSprite().getAnimation()->getSpriteSheet()->getSize().x;
                gameObjects_.push_back(t);
            } else {
                Stone* s = new Stone(logger_, x, y_offset);
                x += s->getSprite().getAnimation()->getSpriteSheet()->getSize().x;
                gameObjects_.push_back(s);
            }
        }

        gameObjects_.push_back(player);
        gameObjects_.push_back(cursor);
        gameObjects_.push_back(playerInventory);
        gameObjects_.push_back(inventorySlot);

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
            if ((*it)->getSlot() == selectedSlot_ && (*it)->getName() == item) {
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

        sf::Text fps_text;
        fps_text.setString(std::to_string(dT.asMilliseconds()));
        fps_text.setFont(settings_->font);
        fps_text.setPosition(0,0);
        fps_text.setColor(sf::Color::Red);
        window.draw(fps_text);
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

    bool inventorySpace(int spots)
    {
        return inventoryItems_.size() + spots <= 8;
    }

    void handleClick(int x, int y, bool leftClick = true)
    {
        bool valid = false;
        sf::Vector2i playerPos;
        for (auto gameObject : gameObjects_) {
            if (gameObject->type == GameObject::Type::Player) {
                playerPos = sf::Vector2i(gameObject->getPosition());
            }
        }
        for (auto gameObject : gameObjects_) {
            if (gameObject->type == GameObject::Type::Inventory && gameObject->checkClick(x, y)) {
                int xd = x - gameObject->getPosition().x;
                int s = xd / slotSize_;
                selectInventorySlot(s);
            }
            if (gameObject->type == GameObject::Type::Cursor && gameObject->validClick(x, y, playerPos.x, playerPos.y)) {
                valid = true;
            }
        }
        if (!valid) {
            return;
        }
        // we have a valid click
        bool exit = false;
        bool removeItem = false;
        bool needSorting = false;
        auto removeIt = gameObjects_.end();
        auto currentIterator = gameObjects_.begin();
        for (auto gameObject : gameObjects_) {
            if (exit)
                break;
            if (gameObject->checkClick(x, y)) {
                switch (gameObject->type) {
                case GameObject::Type::Fire: {
                    switch (gameObject->getLevel()) {
                    case 0:
                        if (removeInventoryItem("ItemWood")) {
                            soundManager_->playSound("ItemWood");
                            gameObject->handleClick();
                            exit = true;
                        }
                        break;
                    case 1:
                        if (removeInventoryItem("ItemMatch")) {
                            soundManager_->playSound("ItemMatch");
                            gameObject->handleClick();
                            exit = true;
                        }
                        break;
                    default:
                        break;
                    }
                    break;
                }
                case GameObject::Type::Tree: {
                    if (inventorySpace(2) && addInventoryItem("ItemWood") && addInventoryItem("ItemSapling")) {
                        soundManager_->playSound("TreeBreak");
                        gameObject->setLevel(0);
                        removeItem = true;
                        removeIt = currentIterator;
                        exit = true;
                    }
                    break;
                }
                case GameObject::Type::Stone: {
                    if (addInventoryItem("ItemStone")) {
                        soundManager_->playSound("StoneBreak");
                        removeItem = true;
                        removeIt = currentIterator;
                        exit = true;
                    }
                    break;
                }
                default:
                    break;
                }
            }
            currentIterator++;
        }
        // no object returned anything, we think it is grass
        if (!exit) {
            // place a fire on clicked position
            if (!leftClick && checkItemInSlot("ItemStone") && removeInventoryItem("ItemStone")) {
                soundManager_->playSound("PlaceFire");
                Fire* fire = new Fire(logger_);
                fire->setPosition(x - fire->getSprite().getLocalBounds().width / 2, y - fire->getSprite().getLocalBounds().height / 2);
                gameObjects_.push_back(fire);
                needSorting = true;
            } else if (!leftClick && checkItemInSlot("ItemSapling") && removeInventoryItem("ItemSapling")) {
                soundManager_->playSound("PlaceSapling");
                Tree* tree = new Tree(logger_, x, y);
                tree->setPosition(x - tree->getSprite().getLocalBounds().width / 2, y - tree->getSprite().getLocalBounds().height / 2);
                gameObjects_.push_back(tree);
                needSorting = true;
            }
        }

        if (removeItem) {
            gameObjects_.erase(removeIt);
        }
        if (needSorting) {
            std::sort(gameObjects_.begin(), gameObjects_.end(), [](GameObject* left, GameObject* right) { return left->type > right->type; });
        }
    }

    bool checkItemInSlot(std::string itemName)
    {
        for (auto item : inventoryItems_) {
            if (item->getSlot() == selectedSlot_ && item->getName() == itemName) {
                return true;
            }
        }
        return false;
    }

    void selectInventorySlot(int slot)
    {
        selectedSlot_ = slot;
        sf::Vector2f invPos;
        for (auto gameObject : gameObjects_) {
            if (gameObject->type == GameObject::Type::Inventory) {
                invPos = gameObject->getPosition();
                break;
            }
        }
        for (auto gameObject : gameObjects_) {
            if (gameObject->type == GameObject::Type::InventorySlot) {
                gameObject->setPosition(invPos.x + slot * slotSize_, invPos.y);
            }
        }
    }

private:
    Logger* logger_;
    Settings* settings_;

    SoundManager* soundManager_;

    unsigned int selectedSlot_ = 0;
    int slotSize_ = 112;

    std::vector<GameObject*> gameObjects_;
    std::vector<InventoryItem*> inventoryItems_;
};

#endif