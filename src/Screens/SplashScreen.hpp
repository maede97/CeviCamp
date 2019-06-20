#ifndef SPLASHSCREEN_HPP
#define SPLASHSCREEN_HPP

#include "../Logger.hpp"
#include "Screen.hpp"

class SplashScreen : public Screen {
public:
    SplashScreen(Logger* logger);
    void show(sf::RenderWindow& window);

private:
    Logger* logger_;
    sf::Texture image_;
    sf::Sprite sprite_;
};

SplashScreen::SplashScreen(Logger* logger)
{
    logger_ = logger;
    if (image_.loadFromFile("res/logo_right.png") != true) {
        logger_->log("SplashScreen", "file res/logo_right.png not found");
        return;
    }
    sprite_ = sf::Sprite(image_);
    sprite_.setPosition(200, 200);
}

void SplashScreen::show(sf::RenderWindow& window)
{
    window.draw(sprite_);
}

#endif