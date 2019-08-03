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
    float factor = std::min(1.f / image_.getSize().x * (1.f*settings_->screenWidth), 1.f / image_.getSize().y * (1.f*settings_->screenHeight));
    logger_->info("factor", std::to_string(factor));
    sprite_.setScale(factor, factor);
    sprite_.setPosition(settings_->screenWidth / 2 - factor * image_.getSize().x / 2, settings_->screenHeight / 2 - factor * image_.getSize().y / 2);

    // show a text on bottom of page
    text_.setFont(settings_->font);
    text_.setString(L"BELIEBIGE TASTE DRÜCKEN");
    text_.setCharacterSize(48 * settings_->getGUIFactor());
    text_.setStyle(sf::Text::Bold);
    text_.setPosition(settings_->screenWidth / 2 - text_.getLocalBounds().width / 2, settings_->screenHeight - 100 - text_.getLocalBounds().height);
}

void SplashScreen::show(sf::RenderWindow& window)
{
    window.draw(sprite_);
    window.draw(text_);
}

#endif