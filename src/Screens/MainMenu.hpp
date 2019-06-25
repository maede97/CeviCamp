#ifndef MAINMENU_HPP
#define MAINMENU_HPP

#include "Screen.hpp"

class MainMenu : public Screen {
public:
    enum MenuResult {
        Nothing,
        KeepPlaying,
        StartGame,
        Options
    };
    MainMenu(Logger* logger, Settings* settings);
    void updateKeepPlaying(bool keepPlaying)
    {
        keepPlaying_ = keepPlaying;
    }
    void show(sf::RenderWindow& window);
    MenuResult handleClick(int x, int y);

private:
    bool keepPlaying_;

    sf::Texture logoRightImage_;
    sf::Sprite logoRightSprite_;

    sf::Text keepPlayingText_;
    sf::Text gameStartText_;
    sf::Text optionsText_;
    sf::Text versionText_;
};

MainMenu::MainMenu(Logger* logger, Settings* settings)
    : Screen(logger, settings)
{
    if (logoRightImage_.loadFromFile("res/logo_right.png") != true) {
        logger_->error("MainMenu", "file res/logo_right.png not found");
        return;
    }
    logoRightSprite_ = sf::Sprite(logoRightImage_);

    keepPlayingText_.setString("WEITERSPIELEN");
    gameStartText_.setString("SPIEL STARTEN");
    optionsText_.setString("OPTIONEN");
    versionText_.setString("VERSION 1.0.0 ALPHA");

    keepPlayingText_.setFont(settings_->font);
    gameStartText_.setFont(settings_->font);
    optionsText_.setFont(settings_->font);
    versionText_.setFont(settings_->font);

    keepPlayingText_.setScale(0.8f * settings_->scalingFactor, 0.8f * settings_->scalingFactor);
    gameStartText_.setScale(0.8f * settings_->scalingFactor, 0.8f * settings_->scalingFactor);
    optionsText_.setScale(0.8f * settings_->scalingFactor, 0.8f * settings_->scalingFactor);
    versionText_.setScale(settings_->scalingFactor, settings_->scalingFactor);
    logoRightSprite_.setScale(settings_->scalingFactor, settings_->scalingFactor);

    keepPlayingText_.setCharacterSize(200 * settings_->scalingFactor);
    gameStartText_.setCharacterSize(200 * settings_->scalingFactor);
    optionsText_.setCharacterSize(200 * settings_->scalingFactor);
    versionText_.setCharacterSize(100 * settings_->scalingFactor);

    int availableSpace = settings_->screenHeight - 400 * settings_->scalingFactor;
    int perItem = availableSpace / 5;

    keepPlayingText_.setPosition(100 * settings_->scalingFactor, 400 * settings_->scalingFactor);
    gameStartText_.setPosition(100 * settings_->scalingFactor, (400 * settings_->scalingFactor + perItem * 1));
    optionsText_.setPosition(100 * settings_->scalingFactor, (400 * settings_->scalingFactor + perItem * 2));
    versionText_.setPosition(100 * settings_->scalingFactor, settings_->screenHeight - 200 * settings_->scalingFactor);
    logoRightSprite_.setPosition(settings_->screenWidth * 3 / 4 - logoRightImage_.getSize().x * logoRightSprite_.getScale().x / 2 - 100 * settings_->scalingFactor, settings_->screenHeight / 2 - logoRightImage_.getSize().y * logoRightSprite_.getScale().y / 2);
}

void MainMenu::show(sf::RenderWindow& window)
{
    sf::Text keepPlayingText_Changed = keepPlayingText_;
    if (!keepPlaying_) {
        keepPlayingText_Changed.setFillColor(sf::Color(150, 150, 150, 100));
    }
    window.draw(keepPlayingText_Changed);
    window.draw(gameStartText_);
    window.draw(optionsText_);
    window.draw(versionText_);
    window.draw(logoRightSprite_);
}

MainMenu::MenuResult MainMenu::handleClick(int x, int y)
{
    if (keepPlaying_ && keepPlayingText_.getGlobalBounds().contains(x, y)) {
        return KeepPlaying;
    }
    if (gameStartText_.getGlobalBounds().contains(x, y)) {
        return StartGame;
    }
    if (optionsText_.getGlobalBounds().contains(x, y)) {
        return Options;
    }
    return Nothing;
}

#endif