#ifndef GAMEOBJECTMANAGER_HPP
#define GAMEOBJECTMANAGER_HPP

#include "GameObjects/Fire.hpp"
#include "GameObjects/Grass.hpp"
#include "GameObjects/Inventory.hpp"
#include "GameObjects/InventoryItem.hpp"
#include "GameObjects/InventorySlot.hpp"
#include "GameObjects/MouseCursor.hpp"
#include "GameObjects/Player.hpp"
#include "GameObjects/Stone.hpp"
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

        MouseCursor* cursor = new MouseCursor(logger_, settings_);
        Inventory* playerInventory = new Inventory(logger_, settings_, slotSize_);
        InventorySlot* inventorySlot = new InventorySlot(logger_, settings_, playerInventory->getPosition().x, playerInventory->getPosition().y, slotSize_);

        movementSpeed = settings_->movementSpeed;

        // push objects into vector
        gameObjects_.push_back(cursor);
        gameObjects_.push_back(playerInventory);
        gameObjects_.push_back(inventorySlot);

        setNewIterators(); // set all iterator
    }

    ~GameObjectManager()
    {
        for (auto gameObject : gameObjects_) {
            delete gameObject;
        }
        for (auto invItem : inventoryItems_) {
            delete invItem;
        }
    }

    void addGameObject(GameObject* object)
    {
        gameObjects_.push_back(object);
    }

    bool addInventoryItem(std::string item)
    {
        sf::Vector2i invPos = sf::Vector2i((*inventoryIterator_)->getPosition());
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
                return true;
            } else {
                slot++;
            }
        }
        return false;
    }

    std::vector<GameObject*>& getObjectVector() { return gameObjects_; }
    std::vector<InventoryItem*>& getInventoryVector() { return inventoryItems_; }

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
            if (gameObject->type != GameObject::Type::Player && gameObject->type != GameObject::Type::Inventory && gameObject->type != GameObject::Type::InventorySlot) {
                gameObject->setPosition(gameObject->getPosition().x, gameObject->getPosition().y - movementSpeed);
            } else if (gameObject->type != GameObject::Type::Player || gameObject->type == GameObject::Type::Inventory || gameObject->type == GameObject::Type::InventorySlot) {
                gameObject->getSprite().move(0, -movementSpeed);
            }
        }
        for (auto invItem : inventoryItems_) {
            invItem->getSprite().move(0, -movementSpeed);
        }
    }
    void up()
    {
        for (auto gameObject : gameObjects_) {
            gameObject->up();
            if (gameObject->type != GameObject::Type::Player && gameObject->type != GameObject::Type::Inventory && gameObject->type != GameObject::Type::InventorySlot) {
                gameObject->setPosition(gameObject->getPosition().x, gameObject->getPosition().y + movementSpeed);
            } else if (gameObject->type != GameObject::Type::Player || gameObject->type == GameObject::Type::Inventory || gameObject->type == GameObject::Type::InventorySlot) {
                gameObject->getSprite().move(0, movementSpeed);
            }
        }
        for (auto invItem : inventoryItems_) {
            invItem->getSprite().move(0, movementSpeed);
        }
    }
    void left()
    {
        for (auto gameObject : gameObjects_) {
            gameObject->left();
            if (gameObject->type != GameObject::Type::Player && gameObject->type != GameObject::Type::Inventory && gameObject->type != GameObject::Type::InventorySlot) {
                gameObject->setPosition(gameObject->getPosition().x + movementSpeed, gameObject->getPosition().y);
            } else if (gameObject->type != GameObject::Type::Player || gameObject->type == GameObject::Type::Inventory || gameObject->type == GameObject::Type::InventorySlot) {
                gameObject->getSprite().move(movementSpeed, 0);
            }
        }
        for (auto invItem : inventoryItems_) {
            invItem->getSprite().move(movementSpeed, 0);
        }
    }
    void right()
    {
        for (auto gameObject : gameObjects_) {
            gameObject->right();
            if (gameObject->type != GameObject::Type::Player && gameObject->type != GameObject::Type::Inventory && gameObject->type != GameObject::Type::InventorySlot) {
                gameObject->setPosition(gameObject->getPosition().x - movementSpeed, gameObject->getPosition().y);
            } else if (gameObject->type != GameObject::Type::Player || gameObject->type == GameObject::Type::Inventory || gameObject->type == GameObject::Type::InventorySlot) {
                gameObject->getSprite().move(-movementSpeed, 0);
            }
        }
        for (auto invItem : inventoryItems_) {
            invItem->getSprite().move(-movementSpeed, 0);
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

        // FPS
        sf::Text fps_text;
        fps_text.setString(std::to_string(1000 / (dT.asMilliseconds() + 1)));
        fps_text.setFont(settings_->font);
        fps_text.setPosition(0, 0);
        window.draw(fps_text);
    }

    void updateMousePosition(int mouseX, int mouseY)
    {
        sf::Vector2i playerPos = sf::Vector2i((*playerIterator_)->getPosition());
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
        sf::Vector2i playerPos = sf::Vector2i((*playerIterator_)->getPosition());

        if ((*inventoryIterator_)->checkClick(x, y)) {
            int xd = x - (*inventoryIterator_)->getPosition().x;
            int s = xd / slotSize_;
            selectInventorySlot(s);
        }
        valid = (*cursorIterator_)->validClick(x, y, playerPos.x, playerPos.y);

        if (!valid) {
            logger_->info("valid", "not valid");
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
            if (leftClick && gameObject->checkClick(x, y)) {
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
                Fire* fire = new Fire(logger_, settings_);
                fire->setPosition(x - fire->getSprite().getLocalBounds().width / 2, y - fire->getSprite().getLocalBounds().height / 2);
                gameObjects_.push_back(fire);
                needSorting = true;
            } else if (!leftClick && checkItemInSlot("ItemSapling") && removeInventoryItem("ItemSapling")) {
                soundManager_->playSound("PlaceSapling");
                Tree* tree = new Tree(logger_, settings_, x, y);
                tree->setPosition(x - tree->getSprite().getLocalBounds().width / 2, y - tree->getSprite().getLocalBounds().height / 2);
                gameObjects_.push_back(tree);
                needSorting = true;
            }
        }

        if (removeItem) {
            gameObjects_.erase(removeIt);
            setNewIterators();
        }
        if (needSorting) {
            orderGameObjects();
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

    void setNewIterators()
    {
        // TODO change to switch
        for (auto it = gameObjects_.begin(); it != gameObjects_.end(); it++) {
            if ((*it)->type == GameObject::Type::Player) {
                playerIterator_ = it;
            } else if ((*it)->type == GameObject::Type::Grass) {
                grassIterator_ = it;
            } else if ((*it)->type == GameObject::Type::Inventory) {
                inventoryIterator_ = it;
            } else if ((*it)->type == GameObject::Type::Cursor) {
                cursorIterator_ = it;
            }
        }
    }

    void orderGameObjects()
    {
        std::sort(gameObjects_.begin(), gameObjects_.end(), [](GameObject* left, GameObject* right) { 
                if(left->type == right->type) {
                    return left->getPosition().y < right->getPosition().y;
                }
                return left->type > right->type; });
        setNewIterators();
    }

    void selectInventorySlot(int slot)
    {
        selectedSlot_ = slot;
        sf::Vector2f invPos = (*inventoryIterator_)->getPosition();

        for (auto gameObject : gameObjects_) {
            if (gameObject->type == GameObject::Type::InventorySlot) {
                gameObject->setPosition(invPos.x + slot * slotSize_, invPos.y);
            }
        }
    }

    void createNewGame()
    {
        logger_->info("GOM", "Create New Game");
        Grass* grass = new Grass(logger_, settings_);
        Player* player = new Player(logger_, settings_);

        gameObjects_.push_back(grass);
        gameObjects_.push_back(player);

        // place x trees and stones all over the map
        for (int i = 0; i < rand() % 10 + 5; i++) {
            Tree* tree = new Tree(logger_, settings_, 50 + rand() % (settings_->mapWidth - 100), 50 + rand() % (settings_->mapHeight - 100));
            gameObjects_.push_back(tree);
        }
        for (int i = 0; i < rand() % 10 + 5; i++) {
            Stone* tree = new Stone(logger_, settings_, 50 + rand() % (settings_->mapWidth - 100), 50 + rand() % (settings_->mapHeight - 100));
            gameObjects_.push_back(tree);
        }

        orderGameObjects();

        // Give some test items
        addInventoryItem("ItemMatch");
        addInventoryItem("ItemMatch");
    }

private:
    Logger* logger_;
    Settings* settings_;

    SoundManager* soundManager_;

    unsigned int selectedSlot_ = 0;
    int slotSize_ = 112;

    int movementSpeed;

    std::vector<GameObject*> gameObjects_;
    std::vector<GameObject*>::iterator playerIterator_;
    std::vector<GameObject*>::iterator grassIterator_;
    std::vector<GameObject*>::iterator inventoryIterator_;
    std::vector<GameObject*>::iterator cursorIterator_;
    std::vector<InventoryItem*> inventoryItems_;
};

#endif