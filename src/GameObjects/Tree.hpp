#ifndef TREE_HPP
#define TREE_HPP

#include "GameObject.hpp"
#include <SFML/Graphics.hpp>

class Tree : public GameObject {
public:
    Tree(Logger* logger, Settings* settings, int x, int y)
        : GameObject(logger, settings, GameObject::Type::Tree)
    {
        if (!image_.loadFromFile("res/CampParts/Tree.png")) {
            logger_->error("Tree", "res/CampParts/Tree.png not found");
            return;
        }
        animation_.setSpriteSheet(image_);
        animation_.addFrame(sf::IntRect(0, 0, image_.getSize().x, image_.getSize().y));
        sprite_ = AnimatedSprite(sf::seconds(1), true, false);

        sprite_.setAnimation(animation_);
        sprite_.setPosition(x, y);
    }

    void play() {}
    void handleClick()
    {
    }
    bool checkClick(float x, float y)
    {
        return sprite_.getGlobalBounds().contains(x, y);
    }

private:
    sf::Texture image_;
    Animation animation_;
};

#endif