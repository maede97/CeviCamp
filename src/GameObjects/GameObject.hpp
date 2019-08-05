#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include "../Logger.hpp"
#include "../Settings.hpp"
#include "AnimatedSprite.hpp"

class GameObject {
public:
    // this order matters, because of item sorting for drawing!
    enum Type { Cursor,
        Inventory,
        Sarasani,
        Tree,
        Player,
        Leader,
        Child,
        Fire,
        Paloxe,
        Trash,
        Blache,
        Stone,
        Grass
    };

    GameObject(Logger* logger, Settings* settings, Type t)
    {
        logger_ = logger;
        settings_ = settings;
        type = t;
    }

    // getter and setter for  position of sprite
    void setPosition(int x, int y)
    {
        sprite_.setPosition(x, y);
    };
    sf::Vector2f getPosition() const { return sprite_.getPosition(); };

    // get sprite
    AnimatedSprite getSprite() const { return sprite_; };

    // getter and setter for level
    unsigned int getLevel() const { return level_; }
    void setLevel(unsigned int level) { level_ = level; }

    // These functions can be overridden to grab information
    // move in directions
    virtual void left() {}
    virtual void right() {}
    virtual void up() {}
    virtual void down() {}
    // set mouse position
    virtual void updateMousePlayerPosition(int mouseX, int mouseY, int playerX, int playerY, int playerWidth, int playerHeight) {}
    // return true if this item is clickable and was clicked
    virtual bool validClick(int mouseX, int mouseY, int playerX, int playerY, int playerSize) { return false; }
    // set animation
    virtual void setAnimation() {}
    virtual void update() { sprite_.update(internalTime_.restart()); } // update Sprite with frametime
    virtual void play() {} // show current animation
    virtual void handleClick() {} // what to do on click
    virtual bool checkClick(float x, float y) { return false; }

    Type type; // type of this object

protected:
    Logger* logger_;
    Settings* settings_;
    AnimatedSprite sprite_;
    sf::Clock internalTime_;
    unsigned int level_ = 0;
};

#endif