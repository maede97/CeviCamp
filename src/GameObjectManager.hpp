#ifndef GAMEOBJECTMANAGER_HPP
#define GAMEOBJECTMANAGER_HPP

#include "GameObjects/Fire.hpp"
#include "GameObjects/Grass.hpp"
#include "GameObjects/MouseCursor.hpp"
#include "GameObjects/Player.hpp"
#include "GameObjects/Tree.hpp"
#include "Logger.hpp"
#include "Settings.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

class GameObjectManager {
public:
    GameObjectManager(Logger* logger, Settings* settings)
    {
        logger_ = logger;
        settings_ = settings;

        grass_ = new Grass(logger_, sf::Vector2i(settings_->screenWidth, settings_->screenHeight));
        player_ = new Player(logger_, sf::Vector2i(settings_->screenWidth, settings_->screenHeight));
        cursor_ = new MouseCursor(logger_);

        buildTrees();

        // Test objects
        fire_ = new Fire(logger_);
        //fire_->play();
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
            fire_->upgrade();
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

    std::vector<Tree*> trees_;
};

#endif