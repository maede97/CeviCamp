#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include "../Logger.hpp"
#include "../Settings.hpp"
#include "AnimatedSprite.hpp"

class GameObject {
public:
    // this order matters, because of item sorting for drawing!
    enum Type { Cursor, // 0
        Player, // 1
        InventorySlot, //2
        Inventory, //3
        Fire, // 4
        Tree, // 5
        Stone, // 6
        Grass // 7
    };
    GameObject(Logger* logger, Settings* settings, Type t)
    {
        logger_ = logger;
        settings_ = settings;
        type = t;
    }

    void setPosition(int x, int y)
    {
        sprite_.setPosition(x, y);
    };

    AnimatedSprite getSprite() const { return sprite_; };

    virtual void update(sf::Time dt)
    {
        sprite_.update(dt);
    }

    unsigned int getLevel() const { return level_; }
    void setLevel(unsigned int level) { level_ = level; }

    // These functions can be overridden to grab information
    virtual void left() {}
    virtual void right() {}
    virtual void up() {}
    virtual void down() {}
    virtual void updateMousePlayerPosition(int mouseX, int mouseY, int playerX, int playerY) {}
    virtual bool validClick(int mouseX, int mouseY, int playerX, int playerY) { return false; }
    virtual sf::Vector2f getPosition() const { return sprite_.getPosition(); };
    virtual void setAnimation() {}

    // These 4 functions need to be overridden
    virtual void play() = 0;
    virtual void handleClick() = 0;
    virtual bool checkClick(float x, float y) = 0;

    Type type;

protected:
    Logger* logger_;
    Settings* settings_;
    AnimatedSprite sprite_;
    unsigned int level_ = 0;
};

#endif