#ifndef OPTIONS_HPP
#define OPTIONS_HPP

#include "../Logger.hpp"
#include "Screen.hpp"

class Options : public Screen {
public:
    Options(Logger* logger);
    void show(sf::RenderWindow& window);

private:
    Logger* logger_;
    sf::Texture image_;
    sf::Sprite sprite_;
};

Options::Options(Logger* logger)
{
    logger_ = logger;
    if (image_.loadFromFile("res/logo_left.png") != true) {
        logger_->log("Options", "file res/logo_left.png not found");
        return;
    }
    sprite_ = sf::Sprite(image_);
    sprite_.setPosition(300,300);
}

void Options::show(sf::RenderWindow& window)
{
    window.clear(sf::Color::Black);
    window.draw(sprite_);
    window.display();
}

#endif