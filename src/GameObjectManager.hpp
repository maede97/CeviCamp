#ifndef GAMEOBJECTMANAGER_HPP
#define GAMEOBJECTMANAGER_HPP

#include "GameObjects/Fire.hpp"
#include "GameObjects/Grass.hpp"
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

        grass_ = new Grass(logger_, sf::Vector2i(settings_->screenWidth, settings_->screenHeight));
        player_ = new Player(logger_, sf::Vector2i(settings_->screenWidth, settings_->screenHeight));
        cursor_ = new MouseCursor(logger_);
        playerInventory_ = new PlayerInventory(logger_, settings_);

        buildTrees();

        // Test objects
        fire_ = new Fire(logger_);

        // Give some test items
        playerInventory_->addItem("ItemWood");
        playerInventory_->addItem("ItemMatch");
    }

    ~GameObjectManager()
    {
        for (auto tree : trees_) {
            delete tree;
        }
        delete fire_;
        delete grass_;
        delete player_;
        delete cursor_;
        delete playerInventory_;
    }

    void down()
    {
        player_->moveDown();
    }
    void up()
    {
        player_->moveUp();
    }
    void left()
    {
        player_->moveLeft();
    }
    void right()
    {
        player_->moveRight();
    }

    void playAnimations()
    {
        fire_->play();
        player_->play();
    }

    void updateDT(sf::Time dt)
    {
        fire_->update(dt);
        player_->update(dt);
    }

    void drawAll(sf::RenderWindow& window, sf::Time dT)
    {
        // Update members
        updateDT(dT);

        // Play Animations
        playAnimations();

        // Draw members, firstly background (grass)
        window.draw(grass_->getSprite());
        for (auto tree : trees_) {
            window.draw(tree->getSprite());
        }
        window.draw(fire_->getSprite());
        window.draw(player_->getSprite());
        window.draw(cursor_->getSprite());
        window.draw(playerInventory_->getSprite());
        playerInventory_->drawItems(window);
    }

    void updateMousePosition(int mouseX, int mouseY)
    {
        cursor_->setPosition(mouseX, mouseY, player_->getPosition().x, player_->getPosition().y);
    }

    void handleClick(int x, int y)
    {
        if (!cursor_->validClick(x, y, player_->getPosition().x, player_->getPosition().y)) {
            return;
        }
        logger_->info("GameObjectManager", "Handle Click");
        
        if (fire_->getSprite().getGlobalBounds().contains(x, y)) {
            switch (fire_->getLevel()) {
            case 0:
                if (playerInventory_->removeItem("ItemWood")) {
                    soundManager_->playSound("ItemWood");
                    fire_->upgrade();
                }
                break;
            case 1:
                if (playerInventory_->removeItem("ItemMatch")) {
                    soundManager_->playSound("ItemMatch");
                    fire_->upgrade();
                }
                break;
            case 2:
                break;
            }
        }
    }

private:
    void buildTrees()
    {
        // oben
        int y_offset = -15;
        for (int x = 0; x < settings_->screenWidth;) {
            Tree* t = new Tree(logger_, x, y_offset);
            x += t->getSprite().getTexture()->getSize().x;
            trees_.push_back(t);
        }
    }
    Logger* logger_;
    Settings* settings_;
    Grass* grass_;
    Fire* fire_;
    Player* player_;
    MouseCursor* cursor_;
    PlayerInventory* playerInventory_;
    SoundManager* soundManager_;

    std::vector<Tree*> trees_;
};

#endif