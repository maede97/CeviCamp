#ifndef MAINMENU_HPP
#define MAINMENU_HPP

#include "Screen.hpp"

class MainMenu : public Screen {
public:
    enum MenuResult {
        Nothing,
        KeepPlaying,
        StartGame,
        Options,
        Exiting
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
    sf::Text exitText_;
};

MainMenu::MainMenu(Logger* logger, Settings* settings)
    : Screen(logger, settings)
{
    if (logoRightImage_.loadFromFile("res/logo_right.png") != true) {
        logger_->error("MainMenu", "file res/logo_right.png not found");
        return;
    }
    // setup texts and sprite
    logoRightSprite_ = sf::Sprite(logoRightImage_);

    keepPlayingText_.setString("WEITERSPIELEN");
    gameStartText_.setString("SPIEL STARTEN");
    optionsText_.setString("EINSTELLUNGEN");
    exitText_.setString("BEENDEN");
    versionText_.setString("VERSION 1.0.0 ALPHA");

    keepPlayingText_.setFont(settings_->font);
    gameStartText_.setFont(settings_->font);
    optionsText_.setFont(settings_->font);
    exitText_.setFont(settings_->font);
    versionText_.setFont(settings_->font);

    float factor = std::min(logoRightImage_.getSize().x / settings_->screenWidth / 2.f, logoRightImage_.getSize().y / settings_->screenWidth / 2.f);
    logoRightSprite_.setScale(factor, factor);

    keepPlayingText_.setCharacterSize(48*settings_->getGUIFactor());
    gameStartText_.setCharacterSize(48*settings_->getGUIFactor());
    optionsText_.setCharacterSize(48*settings_->getGUIFactor());
    exitText_.setCharacterSize(48*settings_->getGUIFactor());
    versionText_.setCharacterSize(24*settings_->getGUIFactor());

    int availableSpace = settings_->screenHeight - 200;
    int perItem = availableSpace / 6;

    keepPlayingText_.setPosition(50, 200);
    gameStartText_.setPosition(50, (200 + perItem * 1));
    optionsText_.setPosition(50, (200 + perItem * 2));
    exitText_.setPosition(50, (200 + perItem * 3));
    versionText_.setPosition(50, settings_->screenHeight - 100);
    logoRightSprite_.setPosition(settings_->screenWidth * 3 / 4 - logoRightImage_.getSize().x * logoRightSprite_.getScale().x / 2 - 50, settings_->screenHeight / 2 - logoRightImage_.getSize().y * logoRightSprite_.getScale().y / 2);
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
    window.draw(exitText_);
    window.draw(versionText_);
    window.draw(logoRightSprite_);
}

MainMenu::MenuResult MainMenu::handleClick(int x, int y)
{
    // return result based on click
    if (keepPlaying_ && keepPlayingText_.getGlobalBounds().contains(x, y)) {
        return KeepPlaying;
    }
    if (gameStartText_.getGlobalBounds().contains(x, y)) {
        return StartGame;
    }
    if (optionsText_.getGlobalBounds().contains(x, y)) {
        return Options;
    }
    if (exitText_.getGlobalBounds().contains(x, y)) {
        return Exiting;
    }
    return Nothing;
}

#endif