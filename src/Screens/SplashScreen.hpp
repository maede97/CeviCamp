#ifndef SPLASHSCREEN_HPP
#define SPLASHSCREEN_HPP

#include "Screen.hpp"

class SplashScreen : public Screen {
public:
    SplashScreen(Logger* logger, Settings* settings);
    void show(sf::RenderWindow& window);

private:
    sf::Texture image_;
    sf::Sprite sprite_;
    sf::Text text_;
};

SplashScreen::SplashScreen(Logger* logger, Settings* settings)
    : Screen(logger, settings)
{
    if (image_.loadFromFile("res/logo_right.png") != true) {
        logger_->error("SplashScreen", "file res/logo_right.png not found");
        return;
    }
    sprite_ = sf::Sprite(image_);
    sprite_.setScale(settings_->scalingFactor, settings_->scalingFactor);
    sprite_.setPosition(settings_->screenWidth / 2 - image_.getSize().x * settings_->scalingFactor / 2, settings_->screenHeight / 2 - image_.getSize().y * settings_->scalingFactor / 2);
    // show a text on bottom of page
    text_.setFont(settings_->font);
    text_.setScale(settings_->scalingFactor, settings_->scalingFactor);
    text_.setString(L"BELIEBIGE TASTE DRÜCKEN");
    text_.setCharacterSize(200 * settings_->scalingFactor);
    text_.setStyle(sf::Text::Bold);
    text_.setPosition(settings_->screenWidth / 2 - text_.getLocalBounds().width*settings_->scalingFactor / 2, settings_->screenHeight - 100 - text_.getLocalBounds().height*settings_->scalingFactor);
}

void SplashScreen::show(sf::RenderWindow& window)
{
    window.draw(sprite_);
    window.draw(text_);
}

#endif