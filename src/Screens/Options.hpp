#ifndef OPTIONS_HPP
#define OPTIONS_HPP

#include "Screen.hpp"

class Options : public Screen {
public:
    Options(Logger* logger, Settings* settings);
    void show(sf::RenderWindow& window);

private:
    sf::Texture image_;
    sf::Sprite sprite_;
};

Options::Options(Logger* logger, Settings* settings) : Screen(logger, settings)
{
    if (image_.loadFromFile("res/logo_right.png") != true) {
        logger_->error("Options", "file res/logo_right.png not found");
        return;
    }
    // TODO
    sprite_ = sf::Sprite(image_);
    sprite_.setPosition(300,300);
}

void Options::show(sf::RenderWindow& window)
{
    window.draw(sprite_);
}

#endif