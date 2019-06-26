#ifndef GAMEOBJECTMANAGER_HPP
#define GAMEOBJECTMANAGER_HPP

#include "GameObjects/Fire.hpp"
#include "GameObjects/Grass.hpp"
#include "GameObjects/Inventory.hpp"
#include "GameObjects/InventoryItem.hpp"
#include "GameObjects/InventorySlot.hpp"
#include "GameObjects/MouseCursor.hpp"
#include "GameObjects/Paloxe.hpp"
#include "GameObjects/Player.hpp"
#include "GameObjects/Stone.hpp"
#include "GameObjects/Tree.hpp"
#include "GameObjects/Blache.hpp"
#include "Logger.hpp"
#include "Settings.hpp"
#include "SoundManager.hpp"
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <vector>

class GameObjectManager {
public:
    GameObjectManager(Logger* logger,
        Settings* settings,
        SoundManager* soundManager)
    {
        logger_ = logger;
        settings_ = settings;
        soundManager_ = soundManager;

        MouseCursor* cursor = new MouseCursor(logger_, settings_);
        Inventory* playerInventory = new Inventory(logger_, settings_, slotSize_);
        InventorySlot* inventorySlot = new InventorySlot(logger_, settings_, playerInventory->getPosition().x,
            playerInventory->getPosition().y, slotSize_);

        movementSpeed_ = settings_->movementSpeed;

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

    void addGameObject(GameObject* object) { gameObjects_.push_back(object); }

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
                inventoryItems_.push_back(new InventoryItem(
                    logger_, item, invPos.x + slot * 112, invPos.y, slot));
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

    void down(bool initial = false)
    {
        if (!initial && (*grassIterator_)->getPosition().y < -(settings_->mapHeight - settings_->screenHeight - movementSpeed_)) {
            (*playerIterator_)->down();
            return;
        }
        for (auto gameObject : gameObjects_) {
            gameObject->down();
            if (gameObject->type != GameObject::Type::Inventory && gameObject->type != GameObject::Type::InventorySlot) {
                gameObject->setPosition(gameObject->getPosition().x,
                    gameObject->getPosition().y - movementSpeed_);
            } else if (gameObject->type == GameObject::Type::Inventory || gameObject->type == GameObject::Type::InventorySlot) {
                gameObject->getSprite().move(
                    0, -movementSpeed_); // move other direction, to stay sticky
            }
        }
        for (auto invItem : inventoryItems_) {
            invItem->getSprite().move(0, -movementSpeed_);
        }
    }
    void up()
    {
        if ((*grassIterator_)->getPosition().y > -movementSpeed_) {
            (*playerIterator_)->up();
            return;
        }
        for (auto gameObject : gameObjects_) {
            gameObject->up();
            if (gameObject->type != GameObject::Type::Inventory && gameObject->type != GameObject::Type::InventorySlot) {
                gameObject->setPosition(gameObject->getPosition().x,
                    gameObject->getPosition().y + movementSpeed_);
            } else if (gameObject->type == GameObject::Type::Inventory || gameObject->type == GameObject::Type::InventorySlot) {
                gameObject->getSprite().move(0, movementSpeed_);
            }
        }
        for (auto invItem : inventoryItems_) {
            invItem->getSprite().move(0, movementSpeed_);
        }
    }
    void left()
    {
        if ((*grassIterator_)->getPosition().x > -movementSpeed_) {
            (*playerIterator_)->left();
            return;
        }
        for (auto gameObject : gameObjects_) {
            gameObject->left();
            if (gameObject->type != GameObject::Type::Inventory && gameObject->type != GameObject::Type::InventorySlot) {
                gameObject->setPosition(gameObject->getPosition().x + movementSpeed_,
                    gameObject->getPosition().y);
            } else if (gameObject->type == GameObject::Type::Inventory || gameObject->type == GameObject::Type::InventorySlot) {
                gameObject->getSprite().move(movementSpeed_, 0);
            }
        }
        for (auto invItem : inventoryItems_) {
            invItem->getSprite().move(movementSpeed_, 0);
        }
    }
    void right(bool initial = false)
    {
        if (!initial && (*grassIterator_)->getPosition().x < -(settings_->mapWidth - settings_->screenWidth - movementSpeed_)) {
            (*playerIterator_)->right();
            return;
        }
        for (auto gameObject : gameObjects_) {
            gameObject->right();
            if (gameObject->type != GameObject::Type::Inventory && gameObject->type != GameObject::Type::InventorySlot) {
                gameObject->setPosition(gameObject->getPosition().x - movementSpeed_,
                    gameObject->getPosition().y);
            } else if (gameObject->type == GameObject::Type::Inventory || gameObject->type == GameObject::Type::InventorySlot) {
                gameObject->getSprite().move(-movementSpeed_, 0);
            }
        }
        for (auto invItem : inventoryItems_) {
            invItem->getSprite().move(-movementSpeed_, 0);
        }
    }

    void playAnimations()
    {
        for (auto gameObject : gameObjects_) {
            gameObject->play();
        }
    }

    void update()
    {
        for (auto gameObject : gameObjects_) {
            gameObject->update();
        }
    }

    void drawAll(sf::RenderWindow& window, sf::Time dT)
    {
        // Update members (all gameObjects)
        update();

        // Play Animations
        playAnimations();

        // Draw members
        for (auto gameObject : gameObjects_) {
            gameObject->getSprite().setScale(settings_->scalingFactor,
                settings_->scalingFactor);
            window.draw(gameObject->getSprite());
            if (gameObject->type == GameObject::Type::Player) {
                // draw items after player and before cursor
                for (auto item : inventoryItems_) {
                    item->getSprite().setScale(settings_->scalingFactor, settings_->scalingFactor);
                    window.draw(item->getSprite());
                }
            }
        }

        // Draw Player pos in top left corner
        sf::Text player_pos;
        player_pos.setString(
            std::to_string(static_cast<int>((*playerIterator_)->getPosition().x - (*grassIterator_)->getPosition().x + (*playerIterator_)->getSprite().getLocalBounds().width / 2))
            + "/"
            + std::to_string(static_cast<int>((*playerIterator_)->getPosition().y - (*grassIterator_)->getPosition().y + (*playerIterator_)->getSprite().getLocalBounds().height / 2)));
        player_pos.setFont(settings_->font);
        player_pos.setPosition(10, 10); // add some small spacing

        window.draw(player_pos);
    }

    void updateMousePosition(int mouseX, int mouseY)
    {
        sf::Vector2i playerPos = sf::Vector2i((*playerIterator_)->getPosition());
        for (auto gameObject : gameObjects_) {
            gameObject->updateMousePlayerPosition(
                mouseX, mouseY, playerPos.x, playerPos.y,
                (*playerIterator_)->getSprite().getLocalBounds().width);
        }
    }

    bool inventorySpace(int spots) { return inventoryItems_.size() + spots <= 8; }

    void handleClick(int x, int y, bool leftClick = true)
    {
        bool valid = false;
        sf::Vector2i playerPos = sf::Vector2i((*playerIterator_)->getPosition());

        if ((*inventoryIterator_)->checkClick(x, y)) {
            int xd = x - (*inventoryIterator_)->getPosition().x;
            int s = xd / slotSize_;
            selectInventorySlot(s);
        }
        valid = (*cursorIterator_)
                    ->validClick(
                        x, y, playerPos.x, playerPos.y,
                        (*playerIterator_)->getSprite().getLocalBounds().width);

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
            // left click = break something
            if (leftClick && gameObject->checkClick(x, y)) {
                switch (gameObject->type) {
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
                case GameObject::Type::Paloxe: {
                    if (addInventoryItem("ItemBlache")) {
                        // soundManager_->playSound("ItemBlache"); // TODO
                        exit = true;
                    }
                }
                default:
                    break;
                }
                // right click, place something
            } else if (!leftClick && gameObject->checkClick(x, y)) {
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
                default:
                    break;
                }
            }
            currentIterator++;
        }
        // no object returned anything, we think it is grass clicked --> build
        // something
        if (!exit) {
            // place a fire on clicked position
            if (!leftClick && checkItemInSlot("ItemStone") && removeInventoryItem("ItemStone")) {
                soundManager_->playSound("PlaceFire");
                Fire* fire = new Fire(logger_, settings_);
                fire->setPosition(x - fire->getSprite().getLocalBounds().width / 2,
                    y - fire->getSprite().getLocalBounds().height / 2);
                gameObjects_.push_back(fire);
                needSorting = true;
            } else if (!leftClick && checkItemInSlot("ItemSapling") && removeInventoryItem("ItemSapling")) {
                soundManager_->playSound("PlaceSapling");
                Tree* tree = new Tree(logger_, settings_, x, y);
                tree->setPosition(x - tree->getSprite().getLocalBounds().width / 2,
                    y - tree->getSprite().getLocalBounds().height / 2);
                gameObjects_.push_back(tree);
                needSorting = true;
            } else if(!leftClick && checkItemInSlot("ItemBlache") && removeInventoryItem("ItemBlache")) {
                //soundManager_->playSound("PlaceBlache"); // TODO
                Blache* blache = new Blache(logger_, settings_,x,y);
                blache->setPosition(x - blache->getSprite().getLocalBounds().width / 2, y - blache->getSprite().getLocalBounds().height / 2);
                gameObjects_.push_back(blache);
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
        for (auto it = gameObjects_.begin(); it != gameObjects_.end(); it++) {
            switch ((*it)->type) {
            case GameObject::Type::Player: {
                playerIterator_ = it;
                break;
            }
            case GameObject::Type::Grass: {
                grassIterator_ = it;
                break;
            }
            case GameObject::Type::Inventory: {
                inventoryIterator_ = it;
                break;
            }
            case GameObject::Type::Cursor: {
                cursorIterator_ = it;
                break;
            }
            default:
                break;
            }
        }
    }

    void orderGameObjects()
    {
        std::sort(gameObjects_.begin(), gameObjects_.end(),
            [](GameObject* left, GameObject* right) {
                if (left->type == right->type) {
                    return left->getPosition().y < right->getPosition().y;
                }
                return left->type > right->type;
            });
        setNewIterators();
    }

    int getSelectedInventorySlot() const { return selectedSlot_; }

    void selectInventorySlot(int slot)
    {
        selectedSlot_ = (slot + 8) % 8;
        sf::Vector2f invPos = (*inventoryIterator_)->getPosition();

        for (auto gameObject : gameObjects_) {
            if (gameObject->type == GameObject::Type::InventorySlot) {
                gameObject->setPosition(invPos.x + selectedSlot_ * slotSize_, invPos.y);
            }
        }
    }

    void createNewGame()
    {
        logger_->info("GOM", "Create New Game");
        Grass* grass = new Grass(logger_, settings_);

        gameObjects_.push_back(grass);

        // place x trees and stones all over the map
        // TODO make tree placement better
        for (int i = 0; i < std::rand() % 10 + 5; i++) {
            Tree* tree = new Tree(logger_, settings_,
                200 + std::rand() % static_cast<int>(settings_->mapWidth - 400 * settings_->scalingFactor),
                200 + std::rand() % static_cast<int>(settings_->mapHeight - 400 * settings_->scalingFactor));
            gameObjects_.push_back(tree);
        }
        for (int i = 0; i < std::rand() % 10 + 5; i++) {
            Stone* tree = new Stone(logger_, settings_,
                200 + std::rand() % static_cast<int>(settings_->mapWidth - 400 * settings_->scalingFactor),
                200 + std::rand() % static_cast<int>(settings_->mapHeight - 400 * settings_->scalingFactor));
            gameObjects_.push_back(tree);
        }

        // add J+S-Paloxe all the way to the right
        Paloxe* paloxe = new Paloxe(logger_, settings_, settings_->mapWidth - 800 * settings_->scalingFactor, settings_->mapHeight / 2);
        gameObjects_.push_back(paloxe);

        orderGameObjects();

        // move all to center
        movementSpeed_ = settings_->mapWidth / 2 - settings_->screenWidth / 2;
        right(true);
        movementSpeed_ = settings_->mapHeight / 2 - settings_->screenHeight / 2;
        down(true);
        movementSpeed_ = settings_->movementSpeed;

        Player* player = new Player(logger_, settings_);
        gameObjects_.push_back(player);

        player->setPosition(settings_->screenWidth / 2 - player->getSprite().getLocalBounds().width * settings_->scalingFactor / 2,
            settings_->screenHeight / 2 - player->getSprite().getLocalBounds().height * settings_->scalingFactor / 2);
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

    int movementSpeed_;

    std::vector<GameObject*> gameObjects_;
    std::vector<GameObject*>::iterator playerIterator_;
    std::vector<GameObject*>::iterator grassIterator_;
    std::vector<GameObject*>::iterator inventoryIterator_;
    std::vector<GameObject*>::iterator cursorIterator_;
    std::vector<InventoryItem*> inventoryItems_;
};

#endif