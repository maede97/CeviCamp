#ifndef GAMEOBJECTMANAGER_HPP
#define GAMEOBJECTMANAGER_HPP

#include "GameObjects/Blache.hpp"
#include "GameObjects/Child.hpp"
#include "GameObjects/ChildrenNames.hpp"
#include "GameObjects/ClickableMenu.hpp"
#include "GameObjects/Fire.hpp"
#include "GameObjects/Grass.hpp"
#include "GameObjects/Inventory.hpp"
#include "GameObjects/InventoryItem.hpp"
#include "GameObjects/MessageBox.hpp"
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

        // load general pointers
        cursorPointer_ = new MouseCursor(logger_, settings_);
        grassPointer_ = new Grass(logger_, settings_);
        grassPointer_->setPosition(0, 0);
        inventoryPointer_ = new Inventory(logger_, settings_, slotSize_);
        inventoryPointer_->setPosition((settings_->screenWidth - inventoryPointer_->getSprite().getGlobalBounds().width) / 2,
            settings_->screenHeight - inventoryPointer_->getSprite().getGlobalBounds().height);

        messageBox_ = new MessageBox(logger_, settings_);
        messageBox_->setPosition(10, 10);

        movementSpeed_ = settings_->movementSpeed;

        // show tutorial if not completed
        if (settings_->showTutorial) {
            hasMenu_ = true;
            ClickableMenu* box = new ClickableMenu(logger_, settings_, L"Tutorial",
                L"Bewege dich mit den Pfeiltasten oder WASD,\nbaue mit Rechtsklick,\nbaue ab mit Linksklick.\n\
                Zoome mit dem Mausrad.\n\nViel Spass!\n\nPS: Die Tastenbelegung findest du auch\nin den Einstellungen.");
            menuBox_ = box;
            settings_->showTutorial = false;
        }
    }

    ~GameObjectManager()
    {
        for (auto gameObject : gameObjects_) {
            delete gameObject;
        }
        for (auto invItem : inventoryItems_) {
            delete invItem;
        }
        delete grassPointer_;
        delete cursorPointer_;
        delete inventoryPointer_;
    }

    void addGameObject(GameObject* object) { gameObjects_.push_back(object); }

    bool addInventoryItem(std::string newItem)
    {
        sf::Vector2i invPos = sf::Vector2i(inventoryPointer_->getPosition());
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
            if ((*it)->getName() == item) {
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
        if (0 < settings_->mapHeight - movementSpeed_) {
            (*playerIterator_)->down();
            return;
        }
    }
    void up()
    {
        view_->gameView.move(0, -settings_->movementSpeed);
        if (0 > -movementSpeed_) {
            (*playerIterator_)->up();
            return;
        }
    }
    void left()
    {
        view_->gameView.move(-settings_->movementSpeed, 0);
        if (0 > -movementSpeed_) {
            (*playerIterator_)->left();
            return;
        }
    }
    void right()
    {
        view_->gameView.move(settings_->movementSpeed, 0);

        if (0 < settings_->mapWidth - movementSpeed_) {
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
        ClickableMenu* box = new ClickableMenu(logger_, settings_, L"CheatMenu", L"Klicke, um Items zu erhalten.");
        box->addButton("ItemStreichholz", L"Streichholz", [&]() { addInventoryItem("ItemStreichholz"); });
        box->addButton("ItemHolz", L"Holz", [&]() { addInventoryItem("ItemHolz"); });
        box->addButton("ItemStein", L"Stein", [&]() { addInventoryItem("ItemStein"); });
        box->addButton("ItemSetzling", L"Setzling", [&]() { addInventoryItem("ItemSetzling"); });
        box->addNewButtonRow();
        box->addButton("ItemBlache", L"Blache", [&]() { addInventoryItem("ItemBlache"); });
        box->addButton("ItemSeil", L"Seil", [&]() { addInventoryItem("ItemSeil"); });
        box->addButton("Delete", L"Löschen", [&]() { inventoryItems_.clear(); });
        menuBox_ = box;
    }

    void drawAll(sf::RenderWindow& window, sf::Time dT)
    {
        orderGameObjects();
        // Update members (all gameObjects)
        update();

        // Play Animations
        playAnimations();

        window.setView(view_->gameView);

        // Draw members
        window.draw(grassPointer_->getSprite());
        for (auto gameObject : gameObjects_) {
            window.draw(gameObject->getSprite());
        }
        window.draw(cursorPointer_->getSprite());

        // Draw static things onto original view
        window.setView(view_->originalView);

        window.draw(inventoryPointer_->getSprite());
        for (auto item : inventoryItems_) {
            item->draw(window);
        }

        if (hasMenu_) {
            menuBox_->draw(window);
        }

        messageBox_->draw(window);

        // now draw minimap
        if (settings_->showMiniMap) {
            window.setView(view_->miniMapView);
            window.draw(grassPointer_->getSprite());
            for (auto gameObject : gameObjects_) {
                if (gameObject->type == GameObject::Type::Cursor || gameObject->type == GameObject::Type::Inventory) {
                    continue;
                }
                window.draw(gameObject->getSprite());
            }
        }
    }

    void updateMousePosition(int mouseX, int mouseY)
    {
        sf::Vector2f playerPos = (*playerIterator_)->getPosition();
        cursorPointer_->updateMousePlayerPosition(mouseX, mouseY, playerPos.x, playerPos.y,
            (*playerIterator_)->getSprite().getGlobalBounds().width, (*playerIterator_)->getSprite().getGlobalBounds().height);

        for (auto gameObject : gameObjects_) {
            gameObject->updateMousePlayerPosition(
                mouseX, mouseY, playerPos.x, playerPos.y,
                (*playerIterator_)->getSprite().getGlobalBounds().width, (*playerIterator_)->getSprite().getGlobalBounds().height);
        }
    }

    bool inventorySpace(int spots) { return inventoryItems_.size() + spots <= 8; }

    bool ifMenuCloseMenu()
    {
        if (hasMenu_) {
            delete menuBox_;
            hasMenu_ = false;
            return true;
        }
        return false;
    }

    void handleClick(int x, int y, bool leftClick = true)
    {
        if (hasMenu_) {
            sf::Vector2i pos = view_->window.mapCoordsToPixel(sf::Vector2f(sf::Mouse::getPosition(view_->window)), view_->originalView);
            if (menuBox_->checkAndHandleClick(pos.x, pos.y)) {
                delete menuBox_;
                hasMenu_ = false;
            }
            return;
        }

        bool valid = false;
        sf::Vector2i playerPos = sf::Vector2i((*playerIterator_)->getPosition());

        valid = cursorPointer_
                    ->validClick(
                        x, y, playerPos.x, playerPos.y,
                        (*playerIterator_)->getSprite().getLocalBounds().width);

        if (!valid) {
            return;
        }

        // we have a valid click
        bool exit = false;
        bool removeItem = false;
        auto removeIt = gameObjects_.end();
        auto currentIterator = gameObjects_.begin();
        for (auto gameObject : gameObjects_) {
            if (exit)
                break;
            // left click = break something
            if (leftClick && gameObject->checkClick(x, y)) {
                switch (gameObject->type) {
                case GameObject::Type::Tree: {
                    if (inventorySpace(2) && addInventoryItem("ItemHolz") && addInventoryItem("ItemSetzling")) {
                        soundManager_->playSound("TreeBreak");
                        gameObject->setLevel(0);
                        removeItem = true;
                        removeIt = currentIterator;
                        exit = true;
                    }
                    break;
                }
                case GameObject::Type::Stone: {
                    if (addInventoryItem("ItemStein")) {
                        soundManager_->playSound("StoneBreak");
                        removeItem = true;
                        removeIt = currentIterator;
                        exit = true;
                    }
                    break;
                }
                case GameObject::Type::Paloxe: {
                    // show menu with Blache & Rope
                    ClickableMenu* menu = new ClickableMenu(logger_, settings_, L"J+S-Paloxe", L"Wähle aus:");
                    menu->addButton("ItemBlache", L"Blache", [&]() { addInventoryItem("ItemBlache"); });
                    menu->addButton("ItemSeil", L"Seil", [&]() { addInventoryItem("ItemSeil"); });
                    hasMenu_ = true;
                    menuBox_ = menu;
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
                        if (removeInventoryItem("ItemHolz")) {
                            soundManager_->playSound("ItemHolz");
                            gameObject->handleClick();
                            exit = true;
                        }
                        break;
                    case 1:
                        if (removeInventoryItem("ItemStreichholz")) {
                            soundManager_->playSound("ItemStreichholz");
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
                    hasMenu_ = true;
                    ClickableMenu* box = new ClickableMenu(logger_, settings_, L"Abfall-Menu", L"Klicke, um wegzuwerfen.");
                    for (auto item : inventoryItems_) {
                        std::string name = item->getName();
                        // +4 because to strip "Item"
                        box->addButton(name, std::wstring(name.begin() + 4, name.end()).c_str(), [&, name]() {
                            removeInventoryItem(name);
                            soundManager_->playSound("ThrowTrash");
                        });
                    }
                    if (box->getButtonAmount() != 0) {
                        menuBox_ = box;
                    } else {
                        delete box;
                        hasMenu_ = false;
                    }
                    exit = true;
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
            if (!leftClick) {
                // show a menu with a selection to build
                hasMenu_ = true;
                ClickableMenu* box = new ClickableMenu(logger_, settings_, L"Bau-Menu", L"Klicke, um an dieser Stelle zu bauen.");

                // add box buttons
                if (checkInventoryItemAmount("ItemBlache", 10) && checkInventoryItemAmount("ItemSeil", 3)) {
                    box->addButton("ItemSarasani", L"Sarasani", [&, x, y]() { buildMenuBuildItem(x, y, GameObject::Type::Sarasani); });
                }
                if (checkInventoryItemAmount("ItemStein", 1)) {
                    box->addButton("ItemStein", L"Feuer", [&, x, y]() { buildMenuBuildItem(x, y, GameObject::Type::Fire); });
                }
                if (checkInventoryItemAmount("ItemBlache", 1)) {
                    box->addButton("ItemBlache", L"Blache", [&, x, y]() { buildMenuBuildItem(x, y, GameObject::Type::Blache); });
                }
                if (checkInventoryItemAmount("ItemSetzling", 1)) {
                    box->addButton("ItemSetzling", L"Baum", [&, x, y]() { buildMenuBuildItem(x, y, GameObject::Type::Tree); });
                }

                if (box->getButtonAmount() != 0) {
                    menuBox_ = box;
                } else {
                    delete box;
                    hasMenu_ = false;
                }
            } else {
                // nothing to do here
            }
        }

        if (removeItem) {
            gameObjects_.erase(removeIt);
            setNewIterators();
        }
    }

    void moveCameraToPlayer()
    {
        sf::Vector2f playerPos = (*playerIterator_)->getPosition() + sf::Vector2f((*playerIterator_)->getSprite().getGlobalBounds().width / 2, (*playerIterator_)->getSprite().getGlobalBounds().height / 2);
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
            removeInventoryItemAmount("ItemSeil", 3);
            orderGameObjects();
            break;
        }
        case GameObject::Type::Fire: {
            Fire* fire = new Fire(logger_, settings_);
            fire->setPosition(x - fire->getSprite().getLocalBounds().width / 2, y - fire->getSprite().getLocalBounds().height / 2);
            gameObjects_.push_back(fire);
            removeInventoryItemAmount("ItemStein", 1);
            break;
        }
        case GameObject::Type::Blache: {
            Blache* blache = new Blache(logger_, settings_, x, y);
            blache->setPosition(x - blache->getSprite().getLocalBounds().width / 2, y - blache->getSprite().getLocalBounds().height / 2);
            gameObjects_.push_back(blache);
            removeInventoryItemAmount("ItemBlache", 1);
            break;
        }
        case GameObject::Type::Tree: {
            Tree* tree = new Tree(logger_, settings_, x, y);
            soundManager_->playSound("PlaceSapling");
            tree->setPosition(x - tree->getSprite().getLocalBounds().width / 2, y - tree->getSprite().getLocalBounds().height / 2);
            gameObjects_.push_back(tree);
            removeInventoryItemAmount("ItemSetzling", 1);
            break;
        }
        default:
            break;
        }
        orderGameObjects();
    }

    void setNewIterators()
    {
        for (auto it = gameObjects_.begin(); it != gameObjects_.end(); it++) {
            switch ((*it)->type) {
            case GameObject::Type::Player: {
                playerIterator_ = it;
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
            /*[](GameObject* left, GameObject* right) {
                if (left->type == right->type) {
                    return left->getPosition().y < right->getPosition().y;
                }
                return left->type > right->type;
            });*/
            [](GameObject* left, GameObject* right) {
                if (left->getSprite().getGlobalBounds().top + left->getSprite().getGlobalBounds().height < right->getSprite().getGlobalBounds().top + right->getSprite().getGlobalBounds().height) {
                    return 1;
                } else {
                    return 0;
                }
            });
        setNewIterators();
    }

    void createNewGame()
    {
        logger_->info("GOM", "Create New Game");
        grassPointer_ = new Grass(logger_, settings_);
        grassPointer_->setPosition(0, 0);

        // place x trees and stones all over the map
        // TODO make tree placement better
        std::uniform_int_distribution<std::mt19937::result_type> distAmount(5, 15);
        std::uniform_int_distribution<std::mt19937::result_type> distMapWidth(200, settings_->mapWidth - 1000);
        std::uniform_int_distribution<std::mt19937::result_type> distMapHeight(200, settings_->mapHeight - 1000);
        for (int i = 0; i < distAmount(settings_->rng); i++) {
            Tree* tree = new Tree(logger_, settings_, distMapWidth(settings_->rng), distMapHeight(settings_->rng));
            gameObjects_.push_back(tree);
        }
        for (int i = 0; i < distAmount(settings_->rng); i++) {
            Stone* tree = new Stone(logger_, settings_, distMapWidth(settings_->rng), distMapHeight(settings_->rng));
            gameObjects_.push_back(tree);
        }

        // add J+S-Paloxe all the way to the right
        Paloxe* paloxe = new Paloxe(logger_, settings_, settings_->mapWidth - 400, settings_->mapHeight / 2);
        gameObjects_.push_back(paloxe);

        Trash* trash = new Trash(logger_, settings_, settings_->mapWidth - 400, settings_->mapHeight / 2 + 250);
        gameObjects_.push_back(trash);

        Player* player = new Player(logger_, settings_);
        gameObjects_.push_back(player);

        player->setPosition(settings_->mapWidth / 2 - player->getSprite().getLocalBounds().width / 2,
            settings_->mapHeight / 2 - player->getSprite().getLocalBounds().height / 2);

        // create some childs
        std::uniform_int_distribution<std::mt19937::result_type> distChildLevel(3, 10);
        std::uniform_int_distribution<std::mt19937::result_type> distChildName(0, CHILDREN_NAMES.size() - 1);

        for (int i = 0; i < 20; i++) {
            Child* child = new Child(logger_, settings_, CHILDREN_NAMES[distChildName(settings_->rng)]); // temp name
            child->setPosition(distMapWidth(settings_->rng), distMapHeight(settings_->rng));
            child->setLevel(distChildLevel(settings_->rng));
            gameObjects_.push_back(child);
        }
        setNewIterators(); // used for addInventoryItem

        // Give some test items
        addInventoryItem("ItemStreichholz");
        addInventoryItem("ItemStreichholz");

        orderGameObjects();
    }

private:
    Logger* logger_;
    Settings* settings_;
    View* view_;

    SoundManager* soundManager_;

    int slotSize_ = 112;

    int movementSpeed_;

    bool hasMenu_ = false;
    ClickableMenu* menuBox_;

    std::vector<GameObject*> gameObjects_;
    std::vector<GameObject*>::iterator playerIterator_;
    GameObject* grassPointer_;
    GameObject* inventoryPointer_;
    GameObject* cursorPointer_;
    std::vector<InventoryItem*> inventoryItems_;
    MessageBox* messageBox_;
};

#endif