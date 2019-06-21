#ifndef TREE_HPP
#define TREE_HPP

#include "../Logger.hpp"
#include <SFML/Graphics.hpp>

class Tree {
public:
    Tree(Logger* logger, int x, int y)
    {
        logger_ = logger;
        if (!image_.loadFromFile("res/CampParts/Tree.png")) {
            logger_->error("Tree", "res/CampParts/Tree.png not found");
            return;
        }

        sprite_ = sf::Sprite(image_);
        sprite_.setPosition(x,y);
    }

    sf::Sprite getSprite() const { return sprite_; }

private:
    sf::Texture image_;
    sf::Sprite sprite_;

    Logger* logger_;
};

#endif