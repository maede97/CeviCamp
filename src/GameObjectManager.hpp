#ifndef GAMEOBJECTMANAGER_HPP
#define GAMEOBJECTMANAGER_HPP

#include "GameObjects/Blache.hpp"
#include "GameObjects/ClickableMessageBox.hpp"
#include "GameObjects/Fire.hpp"
#include "GameObjects/Grass.hpp"
#include "GameObjects/Inventory.hpp"
#include "GameObjects/InventoryItem.hpp"
#include "GameObjects/InventorySlot.hpp"
#include "GameObjects/MouseCursor.hpp"
#include "GameObjects/Paloxe.hpp"
#include "GameObjects/Player.hpp"
#include "GameObjects/Sarasani.hpp"
#include "GameObjects/Stone.hpp"
#include "GameObjects/Trash.hpp"
#include "GameObjects/Tree.hpp"
#include "Logger.hpp"
#include "Settings.hpp"
#include "SoundManager.hpp"
#include "View.hpp"
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <vector>

class GameObjectManager {
public:
    GameObjectManager(Logger* logger,
        Settings* settings,
        SoundManager* soundManager,
        View* view)
    {
        logger_ = logger;
        settings_ = settings;
        soundManager_ = soundManager;
        view_ = view;

        slotSize_ *= settings_->getGUIFactor();

        MouseCursor* cursor = new MouseCursor(logger_, settings_);

        Inventory* playerInventory = new Inventory(logger_, settings_, slotSize_);
        playerInventory->setPosition((settings_->screenWidth - playerInventory->getSprite().getGlobalBounds().width) / 2,
            settings_->screenHeight - playerInventory->getSprite().getGlobalBounds().height);

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

    bool addInventoryItem(std::string newItem)
    {
        sf::Vector2i invPos = sf::Vector2i((*inventoryIterator_)->getPosition());
        for (auto item : inventoryItems_) {
            if (item->getName() == newItem) {
                item->addItem();
                return true;
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
                inventoryItems_.push_back(new InventoryItem(logger_, settings_, newItem, invPos.x + slot * slotSize_, invPos.y, slot));
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
                break;
            }
        }
        if (position == inventoryItems_.end()) {
            return false;
        }
        if ((*position)->getAmount() == 1) {
            delete *position;
            inventoryItems_.erase(position);
        } else {
            (*position)->removeItem();
        }
        logger_->info("Inventory", "Remove Item " + item);
        return true;
    }

    bool checkInventoryItemAmount(std::string item, int amount)
    {
        for (auto it = inventoryItems_.begin(); it != inventoryItems_.end(); it++) {
            if ((*it)->getName() == item && (*it)->getAmount() >= amount) {
                return true;
            }
        }
        return false;
    }

    void removeInventoryItemAmount(std::string item, int amount)
    {
        for (auto it = inventoryItems_.begin(); it != inventoryItems_.end(); it++) {
            if ((*it)->getName() == item && (*it)->getAmount() >= amount) {
                if ((*it)->getAmount() == amount) {
                    delete *it;
                    inventoryItems_.erase(it);
                } else {
                    for (int i = 0; i < amount; i++) {
                        (*it)->removeItem();
                        // subtract 1 from this item counter
                    }
                }
                return;
            }
        }
    }

    void down()
    {
        view_->gameView.move(0, settings_->movementSpeed);
        if ((*grassIterator_)->getPosition().y < settings_->mapHeight - movementSpeed_) {
            (*playerIterator_)->down();
            return;
        }
    }
    void up()
    {
        view_->gameView.move(0, -settings_->movementSpeed);
        if ((*grassIterator_)->getPosition().y > -movementSpeed_) {
            (*playerIterator_)->up();
            return;
        }
    }
    void left()
    {
        view_->gameView.move(-settings_->movementSpeed, 0);
        if ((*grassIterator_)->getPosition().x > -movementSpeed_) {
            (*playerIterator_)->left();
            return;
        }
    }
    void right()
    {
        view_->gameView.move(settings_->movementSpeed, 0);

        if ((*grassIterator_)->getPosition().x < settings_->mapWidth - movementSpeed_) {
            (*playerIterator_)->right();
            return;
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

    void addCheatMenu()
    {
        hasMenu_ = true;
        ClickableMessageBox* box = new ClickableMessageBox(logger_, settings_, L"CheatMenu", L"Klicke, um Items zu erhalten.");
        box->addButton("ItemMatch", L"Zündholz", [&]() { addInventoryItem("ItemMatch"); });
        box->addButton("ItemWood", L"Holz", [&]() { addInventoryItem("ItemWood"); });
        box->addButton("ItemStone", L"Stein", [&]() { addInventoryItem("ItemStone"); });
        box->addButton("ItemSapling", L"Setzling", [&]() { addInventoryItem("ItemSapling"); });
        box->addNewButtonRow();
        box->addButton("ItemBlache", L"Blache", [&]() { addInventoryItem("ItemBlache"); });
        box->addButton("ItemRope", L"Seil", [&]() { addInventoryItem("ItemRope"); });
        box->addButton("Delete", L"Löschen", [&]() { inventoryItems_.clear(); });
        messageBox_ = box;
    }

    void drawAll(sf::RenderWindow& window, sf::Time dT)
    {
        // Update members (all gameObjects)
        update();

        // Play Animations
        playAnimations();

        window.setView(view_->gameView);

        // Draw members
        for (auto gameObject : gameObjects_) {
            if (gameObject->type == GameObject::Type::Inventory || gameObject->type == GameObject::Type::InventorySlot) {
                continue;
            }

            window.draw(gameObject->getSprite());
        }

        // Draw static things onto original view
        window.setView(view_->originalView);

        window.draw((*inventoryIterator_)->getSprite());
        for (auto item : inventoryItems_) {
            item->draw(window);
        }
        window.draw((*inventorySelectorIterator_)->getSprite());
        if (hasMenu_) {
            messageBox_->draw(window);
        }

        // Draw Player pos in top left corner
        sf::Text player_pos;
        player_pos.setString(
            std::to_string(static_cast<int>((*playerIterator_)->getPosition().x - (*grassIterator_)->getPosition().x + (*playerIterator_)->getSprite().getLocalBounds().width / 2))
            + "/"
            + std::to_string(static_cast<int>((*playerIterator_)->getPosition().y - (*grassIterator_)->getPosition().y + (*playerIterator_)->getSprite().getLocalBounds().height / 2)));
        player_pos.setFont(settings_->font);
        player_pos.setPosition(10, 10); // add some small spacing
        player_pos.setCharacterSize(24*settings_->getGUIFactor());

        window.draw(player_pos);

        // now draw minimap
        window.setView(view_->miniMapView);
        for (auto gameObject : gameObjects_) {
            if (gameObject->type == GameObject::Type::Cursor || gameObject->type == GameObject::Type::Inventory || gameObject->type == GameObject::Type::InventorySlot) {
                continue;
            }
            window.draw(gameObject->getSprite());
        }
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

    bool ifMenuCloseMenu()
    {
        if (hasMenu_) {
            delete messageBox_;
            hasMenu_ = false;
            return true;
        }
        return false;
    }

    void handleClick(int x, int y, bool leftClick = true)
    {
        if (hasMenu_) {
            sf::Vector2i pos = view_->window.mapCoordsToPixel(sf::Vector2f(sf::Mouse::getPosition(view_->window)), view_->originalView);
            if (messageBox_->checkAndHandleClick(pos.x, pos.y)) {
                delete messageBox_;
                hasMenu_ = false;
            }
            return;
        }

        bool valid = false;
        sf::Vector2i playerPos = sf::Vector2i((*playerIterator_)->getPosition());

        sf::Vector2i pos = view_->window.mapCoordsToPixel(sf::Vector2f(sf::Mouse::getPosition(view_->window)), view_->originalView);
        if ((*inventoryIterator_)->checkClick(pos.x, pos.y)) {
            int xd = pos.x - (*inventoryIterator_)->getPosition().x;
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
                    // show menu with Blache & Rope
                    ClickableMessageBox* menu = new ClickableMessageBox(logger_, settings_, L"J+S-Paloxe", L"Wähle aus:");
                    menu->addButton("ItemBlache", L"Blache", [&]() { addInventoryItem("ItemBlache"); });
                    menu->addButton("ItemRope", L"Seil", [&]() { addInventoryItem("ItemRope"); });
                    hasMenu_ = true;
                    messageBox_ = menu;
                    exit = true;
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
                case GameObject::Type::Trash: {
                    for (auto item : inventoryItems_) {
                        if (item->getSlot() == getSelectedInventorySlot()) {
                            if (removeInventoryItem(item->getName())) {
                                soundManager_->playSound("ThrowTrash");
                                exit = true;
                            }
                        }
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
            } else if (!leftClick && checkItemInSlot("ItemBlache") && removeInventoryItem("ItemBlache")) {
                //soundManager_->playSound("PlaceBlache"); // TODO
                Blache* blache = new Blache(logger_, settings_, x, y);
                blache->setPosition(x - blache->getSprite().getLocalBounds().width / 2, y - blache->getSprite().getLocalBounds().height / 2);
                gameObjects_.push_back(blache);
                needSorting = true;
            } else if (!leftClick) {
                // show a menu with a selection to build
                hasMenu_ = true;
                ClickableMessageBox* box = new ClickableMessageBox(logger_, settings_, L"Bau-Menu", L"Klicke, um an dieser Stelle zu bauen.");
                // TEMP item for sarasani
                if (checkInventoryItemAmount("ItemBlache", 10) && checkInventoryItemAmount("ItemRope", 3)) {
                    box->addButton("ItemWood", L"Sarasani", [&, x, y]() { buildMenuBuildItem(x, y, GameObject::Type::Sarasani); });
                }
                messageBox_ = box;
            } else {
                // nothing to do here
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

    void moveCameraToPlayer()
    {
        sf::Vector2f playerPos = (*playerIterator_)->getPosition();
        view_->gameView.setCenter(playerPos);
    }

    void buildMenuBuildItem(int x, int y, GameObject::Type build)
    {
        switch (build) {
        case GameObject::Type::Sarasani: {
            Sarasani* sarasani = new Sarasani(logger_, settings_, x, y);
            sarasani->setPosition(x - sarasani->getSprite().getLocalBounds().width / 2, y - sarasani->getSprite().getLocalBounds().height / 2);
            gameObjects_.push_back(sarasani);
            // remove items
            removeInventoryItemAmount("ItemBlache", 10);
            removeInventoryItemAmount("ItemRope", 3);
            orderGameObjects();
            break;
        }
        default:
            break;
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
            case GameObject::Type::InventorySlot: {
                inventorySelectorIterator_ = it;
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
                200 + std::rand() % static_cast<int>(settings_->mapWidth - 200),
                200 + std::rand() % static_cast<int>(settings_->mapHeight - 200));
            gameObjects_.push_back(tree);
        }
        for (int i = 0; i < std::rand() % 10 + 5; i++) {
            Stone* tree = new Stone(logger_, settings_,
                200 + std::rand() % static_cast<int>(settings_->mapWidth - 200),
                200 + std::rand() % static_cast<int>(settings_->mapHeight - 200));
            gameObjects_.push_back(tree);
        }

        // add J+S-Paloxe all the way to the right
        Paloxe* paloxe = new Paloxe(logger_, settings_, settings_->mapWidth - 400, settings_->mapHeight / 2);
        gameObjects_.push_back(paloxe);

        Trash* trash = new Trash(logger_, settings_, settings_->mapWidth - 400, settings_->mapHeight / 2 + 250);
        gameObjects_.push_back(trash);

        orderGameObjects();

        Player* player = new Player(logger_, settings_);
        gameObjects_.push_back(player);

        player->setPosition(settings_->mapWidth / 2 - player->getSprite().getLocalBounds().width / 2,
            settings_->mapHeight / 2 - player->getSprite().getLocalBounds().height / 2);
        orderGameObjects();

        // Give some test items
        addInventoryItem("ItemMatch");
        addInventoryItem("ItemMatch");
    }

private:
    Logger* logger_;
    Settings* settings_;
    View* view_;

    SoundManager* soundManager_;

    unsigned int selectedSlot_ = 0;
    int slotSize_ = 112;

    int movementSpeed_;

    bool hasMenu_ = false;
    ClickableMessageBox* messageBox_;

    std::vector<GameObject*> gameObjects_;
    std::vector<GameObject*>::iterator playerIterator_;
    std::vector<GameObject*>::iterator grassIterator_;
    std::vector<GameObject*>::iterator inventoryIterator_;
    std::vector<GameObject*>::iterator inventorySelectorIterator_;
    std::vector<GameObject*>::iterator cursorIterator_;
    std::vector<InventoryItem*> inventoryItems_;
};

#endif