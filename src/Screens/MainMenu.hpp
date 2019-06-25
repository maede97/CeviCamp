#ifndef MAINMENU_HPP
#define MAINMENU_HPP

#include "Screen.hpp"

class MainMenu : public Screen
{
public:
    enum MenuResult
    {
        Nothing,
        KeepPlaying,
        StartGame,
        Options
    };
    MainMenu(Logger *logger, Settings *settings);
    void updateKeepPlaying(bool keepPlaying)
    {
        keepPlaying_ = keepPlaying;
    }
    void show(sf::RenderWindow &window);
    MenuResult handleClick(int x, int y);

private:
    bool keepPlaying_;

    sf::Texture keepPlayingImage_;
    sf::Sprite keepPlayingSprite_;
    sf::Texture gamestartImage_;
    sf::Sprite gamestartSprite_;
    sf::Texture optionsImage_;
    sf::Sprite optionsSprite_;
    sf::Texture versionImage_;
    sf::Sprite versionSprite_;
    sf::Texture logoRightImage_;
    sf::Sprite logoRightSprite_;
};

MainMenu::MainMenu(Logger *logger, Settings *settings)
    : Screen(logger, settings)
{
    if (keepPlayingImage_.loadFromFile("res/keep_playing.png") != true)
    {
        logger_->error("MainMenu", "file res/keep_playing.png not found");
        return;
    }
    if (gamestartImage_.loadFromFile("res/gamestart.png") != true)
    {
        logger_->error("MainMenu", "file res/gamestart.png not found");
        return;
    }
    if (optionsImage_.loadFromFile("res/options.png") != true)
    {
        logger_->error("MainMenu", "file res/options.png not found");
        return;
    }
    if (versionImage_.loadFromFile("res/version.png") != true)
    {
        logger_->error("MainMenu", "file res/version.png not found");
        return;
    }
    if (logoRightImage_.loadFromFile("res/logo_right.png") != true)
    {
        logger_->error("MainMenu", "file res/logo_right.png not found");
        return;
    }
    keepPlayingSprite_ = sf::Sprite(keepPlayingImage_);
    gamestartSprite_ = sf::Sprite(gamestartImage_);
    optionsSprite_ = sf::Sprite(optionsImage_);
    versionSprite_ = sf::Sprite(versionImage_);
    logoRightSprite_ = sf::Sprite(logoRightImage_);

    keepPlayingSprite_.setScale(0.8f * settings_->scalingFactor, 0.8f * settings_->scalingFactor);
    gamestartSprite_.setScale(0.8f * settings_->scalingFactor, 0.8f * settings_->scalingFactor);
    optionsSprite_.setScale(0.8f * settings_->scalingFactor, 0.8f * settings_->scalingFactor);
    versionSprite_.setScale(settings_->scalingFactor, settings_->scalingFactor);
    logoRightSprite_.setScale(settings_->scalingFactor, settings_->scalingFactor);

    int availableSpace = settings_->screenHeight - 400 * settings_->scalingFactor;
    int perItem = availableSpace / 5;
    keepPlayingSprite_.setPosition(100 * settings_->scalingFactor, 400 * settings_->scalingFactor);
    gamestartSprite_.setPosition(100 * settings_->scalingFactor, (400 * settings_->scalingFactor + perItem * 1));
    optionsSprite_.setPosition(100 * settings_->scalingFactor, (400 * settings_->scalingFactor + perItem * 2));
    versionSprite_.setPosition(100 * settings_->scalingFactor, settings_->screenHeight - 200 * settings_->scalingFactor);
    logoRightSprite_.setPosition(settings_->screenWidth * 3 / 4 - logoRightImage_.getSize().x * logoRightSprite_.getScale().x / 2 - 100 * settings_->scalingFactor, settings_->screenHeight / 2 - logoRightImage_.getSize().y * logoRightSprite_.getScale().y / 2);
}

void MainMenu::show(sf::RenderWindow &window)
{
    sf::Sprite keepPlayingSprite_Changed = keepPlayingSprite_;
    if (!keepPlaying_)
    {
        keepPlayingSprite_Changed.setColor(sf::Color(150, 150, 150, 100));
    }
    window.draw(keepPlayingSprite_Changed);

    window.draw(gamestartSprite_);
    window.draw(optionsSprite_);
    window.draw(versionSprite_);
    window.draw(logoRightSprite_);
}

MainMenu::MenuResult MainMenu::handleClick(int x, int y)
{
    if (keepPlaying_ && keepPlayingSprite_.getGlobalBounds().contains(x, y))
    {
        return KeepPlaying;
    }
    if (gamestartSprite_.getGlobalBounds().contains(x, y))
    {
        return StartGame;
    }
    if (optionsSprite_.getGlobalBounds().contains(x, y))
    {
        return Options;
    }
    return Nothing;
}

#endif