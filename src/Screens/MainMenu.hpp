#ifndef MAINMENU_HPP
#define MAINMENU_HPP

#include "../Logger.hpp"
#include "Screen.hpp"

class MainMenu : public Screen {
public:
    enum MenuResult { Nothing,
        KeepPlaying,
        StartGame,
        Options };
    MainMenu(Logger* logger, bool keepPlaying);
    void show(sf::RenderWindow& window);
    MenuResult handleClick(int x, int y);

private:
    Logger* logger_;

    bool keepPlaying_;

    sf::Texture keepPlayingImage_;
    sf::Sprite keepPlayingSprite_;
    sf::Texture gamestartImage_;
    sf::Sprite gamestartSprite_;
    sf::Texture optionsImage_;
    sf::Sprite optionsSprite_;
    sf::Texture versionImage_;
    sf::Sprite versionSprite_;
    sf::Texture logoLeftImage_;
    sf::Sprite logoLeftSprite_;
    sf::Texture logoRightImage_;
    sf::Sprite logoRightSprite_;
};

MainMenu::MainMenu(Logger* logger, bool keepPlaying)
{
    keepPlaying_ = keepPlaying;
    logger_ = logger;
    if (keepPlayingImage_.loadFromFile("res/keep_playing.png") != true) {
        logger_->log("MainMenu", "file res/keep_playing.png not found");
        return;
    }
    if (gamestartImage_.loadFromFile("res/gamestart.png") != true) {
        logger_->log("MainMenu", "file res/gamestart.png not found");
        return;
    }
    if (optionsImage_.loadFromFile("res/options.png") != true) {
        logger_->log("MainMenu", "file res/options.png not found");
        return;
    }
    if (versionImage_.loadFromFile("res/version.png") != true) {
        logger_->log("MainMenu", "file res/version.png not found");
        return;
    }
    if (logoLeftImage_.loadFromFile("res/logo_left.png") != true) {
        logger_->log("MainMenu", "file res/logo_left.png not found");
        return;
    }
    if (logoRightImage_.loadFromFile("res/logo_right.png") != true) {
        logger_->log("MainMenu", "file res/logo_right.png not found");
        return;
    }
    keepPlayingSprite_ = sf::Sprite(keepPlayingImage_);
    gamestartSprite_ = sf::Sprite(gamestartImage_);
    optionsSprite_ = sf::Sprite(optionsImage_);
    versionSprite_ = sf::Sprite(versionImage_);
    logoLeftSprite_ = sf::Sprite(logoLeftImage_);
    logoRightSprite_ = sf::Sprite(logoRightImage_);

    logoLeftSprite_.setPosition(50, 50);
    keepPlayingSprite_.setPosition(50, 250);
    gamestartSprite_.setPosition(50, 350);
    optionsSprite_.setPosition(50, 450);
    versionSprite_.setPosition(50, 600);
    logoRightSprite_.setPosition(1000, 300);
}

void MainMenu::show(sf::RenderWindow& window)
{
    window.clear(sf::Color::Black);
    if (keepPlaying_)
        window.draw(keepPlayingSprite_);
    window.draw(gamestartSprite_);
    window.draw(optionsSprite_);
    window.draw(versionSprite_);
    window.draw(logoLeftSprite_);
    window.draw(logoRightSprite_);
    window.display();
}

MainMenu::MenuResult MainMenu::handleClick(int x, int y)
{
    if (keepPlaying_ && keepPlayingSprite_.getGlobalBounds().contains(x, y)) {
        return KeepPlaying;
    }
    if (gamestartSprite_.getGlobalBounds().contains(x, y)) {
        return StartGame;
    }
    if (optionsSprite_.getGlobalBounds().contains(x, y)) {
        return Options;
    }
    return Nothing;
}

#endif